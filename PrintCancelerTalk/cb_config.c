/*
 * This file implements `Config` command. It accepts a command string
 * such as `C chrome` and outputs JSON like this:
 *
 *    {"status":"OK", "config": {"CloseEmptyTab":1, ...}}
 *
 * Typycally, each browser addon calls this program every 1 minute,
 * to sync with the latest redirect rules.
 *
 * NOTE: You need to set up `HKLM\SOFTWARE\PrintCanceler\Rulefile` before
 * using this program.
 */

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "internal.h"

/*
 * First, we define structs to store configuration. The "section" struct
 * represents each section block. The "config" struct holds the entire
 * INI file.
 */
struct section {
	char name[16];
	struct strbuf patterns;
	struct strbuf excludes;
	struct section *next;    /* linked list */
};

struct config {
	int warning_when_close_dialog;
	int close_err_cache_miss_page;
	struct section *section;
};

/*
 * Allocate a section object. `line` is the first line of the section
 * block, containing a browser name in brackets ("[IE]").
 */
static struct section *new_section(char *line)
{
	struct section *section;
	char *p;
	char *q;

	section = xmalloc(sizeof(struct section));
	memset(section, 0, sizeof(struct section));

	p = line + 1;
	q = section->name;
	while (*p && *p != ']') {
		*q++ = tolower(*p++);
	}
	return section;
}

/*
 * The format of PrintCanceler config uses an INI-file like syntax.
 * You can define variables/URLs in each section like this:
 *
 *     [GLOBAL]
 *     @WARNING_WHEN_CLOSE_DIALOG
 *
 *     [TARGETS]
 *     https://example.com*
 *     -https://test.example.com*
 *
 * "@" lines are properties. Normal lines are URL patterns.
 * "-" lines are exclude patterns.
 */
static void parse_conf(char *data, struct config *conf)
{
	char *line;
	int global;
	struct section *section;
	struct section **indirect = &conf->section;

	line = strtok(data, "\r\n");
	while (line) {
	    switch (line[0]) {
	    case ';':
	    case '#':
	        break;
	    case '[':
			global = 0;

	        if (strcmp(line, "[GLOBAL]") == 0) {
				global = 1;
	        }
			else {
				section = new_section(line);
				*indirect = section;
				indirect = &section->next;
			}
	        break;
	    case '@':
			if (global) {
				if (strcmp(line, "@WARNING_WHEN_CLOSE_DIALOG") == 0)
				{
					conf->warning_when_close_dialog = 1;
				}
				if (strcmp(line, "@CLOSE_ERR_CACHE_MISS_PAGE") == 0)
				{
					conf->close_err_cache_miss_page = 1;
				}
			}
			break;
	    case '-':
	        if (section && strstr(line, "-#") != line) {
	            strbuf_concat(&section->excludes, line);
	            strbuf_putchar(&section->excludes, '\n');
	        }
			break;
	    default:
	        if (section) {
				strbuf_concat(&section->patterns, line);
				strbuf_putchar(&section->patterns, '\n');
			}
	        break;
	    }
	    line = strtok(NULL, "\r\n");
	}
}

static char *dump_section(struct section *section)
{
	struct strbuf sb = {0};
	char *ptr;
	char *end;
	int need_comma;

	strbuf_concat(&sb, "{\"Name\":");
	strbuf_concat_jsonstr(&sb, section->name, strlen(section->name));
	strbuf_putchar(&sb, ',');

	/* URLPatterns */
	strbuf_concat(&sb, "\"Patterns\":[");
	if (section->patterns.buf) {
		ptr = section->patterns.buf;
		need_comma = 0;
		while (*ptr) {
			if (need_comma)
				strbuf_putchar(&sb, ',');

			end = strchr(ptr, '\n');
			strbuf_concat_jsonstr(&sb, ptr, end - ptr);
			need_comma = 1;
			ptr = end + 1;
		}
	}
	strbuf_concat(&sb, "],");

	/* URLExcludePatterns */
	strbuf_concat(&sb, "\"Excludes\":[");
	if (section->excludes.buf) {
		ptr = section->excludes.buf;
		need_comma = 0;
		while (*ptr) {
			if (need_comma)
				strbuf_putchar(&sb, ',');

			end = strchr(ptr, '\n');
			strbuf_concat_jsonstr(&sb, ptr + 1, end - ptr - 1);
			need_comma = 1;
			ptr = end + 1;
		}
	}
	strbuf_concat(&sb, "]}");

	return sb.buf;
}

static char *dump_json(struct config *conf)
{
	struct strbuf sb = {0};
	struct section *section;
	char buf[64];
	char *json;
	int need_comma;

	/* WarningWhenCloseDialog */
	strbuf_concat(&sb, "{\"WarningWhenCloseDialog\":");
	strbuf_concat(&sb, _itoa(conf->warning_when_close_dialog, buf, 10));
	strbuf_putchar(&sb, ',');

	/* CloseErrCacheMissPage */
	strbuf_concat(&sb, "\"CloseErrCacheMissPage\":");
	strbuf_concat(&sb, _itoa(conf->close_err_cache_miss_page, buf, 10));
	strbuf_putchar(&sb, ',');

	/* Sections */
	strbuf_concat(&sb, "\"Sections\":[");

	section = conf->section;
	need_comma = 0;
	while (section) {
		/* no pattern defined */
		if (section->patterns.buf == NULL) {
			section = section->next;
			continue;
		}

		if (need_comma)
			strbuf_putchar(&sb, ',');

		json = dump_section(section);
		strbuf_concat(&sb, json);
		free(json);

		need_comma = 1;
		section = section->next;
	}

	strbuf_concat(&sb, "]}");

	return sb.buf;
}

/*
 * Utils to locate & read PrintCanceler.ini.
 */
static int get_PrintCancelerRulefile(char *buf, DWORD size)
{
	int ret;
	DWORD len = size;
	memset(buf, 0, size);

	ret = RegGetValueA(HKEY_LOCAL_MACHINE,"SOFTWARE\\PrintCanceler","Rulefile",
						RRF_RT_REG_SZ, NULL, buf, &size);
	if (ret != ERROR_SUCCESS) {
	    fprintf(stderr, "cannot read %s (%i)","SOFTWARE\\PrintCanceler", ret);
	    return -1;
	}
	buf[len - 1] = '\0';
	return 0;
}

static char *read_file(const char *path)
{
	int ret;
	FILE *fp;
	char *buf;
	struct stat st;

	fp = fopen(path, "rb");
	if (fp == NULL)
	    return NULL;

	ret = fstat(_fileno(fp), &st);
	if (ret == -1) {
	    fclose(fp);
	    return NULL;
	}

	if (!(st.st_mode & S_IFREG)) {
	    fclose(fp);
	    return NULL;
	}

	buf = calloc(st.st_size + 1, 1);
	if (!buf) {
	    fclose(fp);
	    return NULL;
	}

	ret = fread(buf, st.st_size, 1, fp);
	if (ret != 1) {
	    free(buf);
	    fclose(fp);
	    return NULL;
	}
	fclose(fp);
	return buf;
}

int cb_config(char *cmd)
{
	char *data;
	char *json;
	struct config conf = {0};
	struct section *section, *next;

	char path[MAX_PATH] = {0};

	if (get_PrintCancelerRulefile(path, MAX_PATH) < 0)
	    return -1;

	data = read_file(path);
	if (data == NULL) {
	    fprintf(stderr, "cannot read %s", path);
	    return -1;
	}

	parse_conf(data, &conf);

	json = dump_json(&conf);

	talk_response("{\"status\":\"OK\",\"config\":%s}", json);

	free(data);
	free(json);

	section = conf.section;
	while (section) {
		next = section->next;
		free(section->patterns.buf);
		free(section->excludes.buf);
		free(section);
		section = next;
	}
	return 0;
}

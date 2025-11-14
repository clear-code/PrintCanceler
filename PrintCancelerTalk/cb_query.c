#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "internal.h"

static int get_PrintCancelerExtensionExecfile(char *buf, DWORD size)
{
	int ret;
	DWORD len = size;
	memset(buf, 0, size);

	ret = RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\PrintCanceler", "ExtensionExecfile", RRF_RT_REG_SZ,
	                   NULL, buf, &size);
	if (ret != ERROR_SUCCESS) {
	    fprintf(stderr, "cannot read %s (%i)", "SOFTWARE\\PrintCanceler", ret);
	    return -1;
	}
	buf[len - 1] = '\0';
	return 0;
}

static int start_monitoring(char *browser, char *path)
{
	int ret;
	struct strbuf sb = {0};
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	memset(&pi, 0, sizeof(pi));
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);

	strbuf_putchar(&sb, '\0');

	ret = CreateProcessA(path,  /* lpApplicationName */
	                    NULL, /* lpCommandLine */
	                    NULL,   /* lpProcessAttributes  */
	                    NULL,   /* lpThreadAttributes */
	                    FALSE,  /* bInheritHandles */
	                    CREATE_NEW_PROCESS_GROUP,
	                    NULL,   /* lpEnvironment */
	                    NULL,   /* lpCurrentDirectory */
	                    &si,    /* lpStartupInfo */
	                    &pi);   /* lpProcessInformation */
	if (ret == 0) {
	    fprintf(stderr, "cannot exec '%s %s' (%d)", path, sb.buf, GetLastError());
	    free(sb.buf);
	    return -1;
	}

	free(sb.buf);
	return 0;
}

int cb_query(char *cmd)
{
	char path[MAX_PATH];
	char *browser;

	if (strlen(cmd) < 3) {
		fprintf(stderr, "command too short '%s'", cmd);
		return -1;
	}

	/*
	 *  Q edge
	 *    ----
	 */
	browser = cmd + 2;

	if (get_PrintCancelerExtensionExecfile(path, MAX_PATH) < 0)
		return -1;

	if (start_monitoring(browser, path) < 0)
		return -1;

	talk_response("{\"status\":\"OK\"}");
	return 0;
}

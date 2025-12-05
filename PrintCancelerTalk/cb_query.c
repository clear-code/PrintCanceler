#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tlhelp32.h>
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

int cb_query_start(char *cmd)
{
	char path[MAX_PATH];
	char *browser;

	if (strlen(cmd) < 3) {
		fprintf(stderr, "command too short '%s'", cmd);
		return -1;
	}

	/*
	 *  B edge
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

static int stop_monitoring(char *browser, char *path)
{
	HANDLE hSnap;
	PROCESSENTRY32 pe32;

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "CreateToolhelp32Snapshot failed (%lu)\n", GetLastError());
		return -1;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &pe32)) {
		fprintf(stderr, "Process32First failed (%lu)\n", GetLastError());
		CloseHandle(hSnap);
		return -1;
	}

	int stopped = 0;

	do {
		HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_TERMINATE,
		                           FALSE,
		                           pe32.th32ProcessID);
		if (!hProc) {
			continue;
		}

		char procPath[MAX_PATH];
		DWORD size = sizeof(procPath);

		if (QueryFullProcessImageNameA(hProc, 0, procPath, &size)) {
			if (_stricmp(procPath, path) == 0) {
				if (TerminateProcess(hProc, 0)) {
					stopped = 1;
				} else {
					fprintf(stderr, "TerminateProcess failed (%lu)\n", GetLastError());
				}
			}
		}

		CloseHandle(hProc);

	} while (Process32Next(hSnap, &pe32));

	CloseHandle(hSnap);

	if (!stopped) {
		fprintf(stderr, "No matching process for '%s'\n", path);
		return -1;
	}

	return 0;
}

int cb_query_stop(char *cmd)
{
	char path[MAX_PATH];
	char *browser;

	if (strlen(cmd) < 3) {
		fprintf(stderr, "command too short '%s'", cmd);
		return -1;
	}

	/*
	 *  E edge
	 *    ----
	 */
	browser = cmd + 2;

	if (get_PrintCancelerExtensionExecfile(path, MAX_PATH) < 0)
		return -1;

	if (stop_monitoring(browser, path) < 0)
		return -1;

	talk_response("{\"status\":\"OK\"}");
	return 0;
}

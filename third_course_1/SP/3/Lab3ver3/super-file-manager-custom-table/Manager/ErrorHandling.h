#pragma once

#include <stdio.h>
#include <Windows.h>

#define LOG_DIR_CREATED

#ifdef LOG_DIR_CREATED
#define LOG_FILE_PATH "D:\\3Curse\\SP\\3\\error.log"
#else
#define LOG_FILE_PATH "D:\\error.log"
#endif


#ifdef ILIA_VERSION
#define CRIT_ERROR_STRING L"CRITICAL ERROR: Cannot allocate memory.\n"
#define ACCESS_DENIED_ERROR_STRING L"ACCESS ERROR: inaccessible directory: "
#else
#define CRIT_ERROR_STRING L"[CRIT] No memory available.\n"
#define ACCESS_DENIED_ERROR_STRING L"[ERROR] Access denied for directory: "
#endif

FILE* logFile;
HWND mainWindowHandle;

void* LoggedMalloc(size_t size);
BOOL LogError(WCHAR* error);

BOOL InitializeFileLogging(HWND windowHandle);
int FinishFileLogging();
#include "ErrorHandling.h"

void* LoggedMalloc(size_t size) {
	void* ptr = malloc(size);
	if (ptr == NULL) {
		LogError(CRIT_ERROR_STRING);
		SendMessage(mainWindowHandle, WM_DESTROY, 0, 0);
	}
	return ptr;
}

BOOL InitializeFileLogging(HWND windowHandle){
	mainWindowHandle = windowHandle;
	errno_t openStatus = fopen_s(&logFile, LOG_FILE_PATH, "w");
	return openStatus == 0;
}

int FinishFileLogging() {
	fflush(logFile);
	return fclose(logFile);
}

BOOL LogError(WCHAR* error) {
	fwprintf(logFile, error);
}
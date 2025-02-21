// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             
// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

typedef struct THREADS
{
	HANDLE handleThread;
	LPTHREAD_START_ROUTINE ThreadFucntion;
	HWND handleProgressBar;
	HWND handleTimerEdit;
	DWORD threadId;
	HWND handleDialog;
	UINT idcTimeEdit;
	UINT idcTimer;
	UINT time;
};

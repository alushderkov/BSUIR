#pragma once
#include <Windows.h>
#include <stdio.h>

#define NUMERIC_UP_DOWN_CLASSNAME L"CreateNumericUpDownClass"
#define WS_NUMERIC_UPDOWN_WINDOW WS_CHILD | WS_VISIBLE //| WS_BORDER

#define NUD_MARGIN 0
#define NUD_GAP 2
#define NUD_EDIT_WIDTH(x) ((x-NUD_MARGIN*2)*2/3)
#define NUD_EDIT_HEIGHT(x) (x-NUD_MARGIN*2)
#define IDC_NUD_EDIT 124
#define IDC_NUD_BUTTON_UP 125
#define IDC_NUD_BUTTON_DOWN 126

#define NUD_BUTTON_UP_WIDTH(x) ((x-NUD_MARGIN*2)/3 - NUD_GAP)
#define NUD_BUTTON_UP_HEIGHT(x) (x-NUD_MARGIN*2)/2 - 1

#define NUD_BUTTON_DOWN_WIDTH(x) ((x-NUD_MARGIN*2)/3 - NUD_GAP)
#define NUD_BUTTON_DOWN_HEIGHT(x) (x-NUD_MARGIN*2)/2 - 1

typedef struct {
	INT width;
	INT height;
	INT value;
	INT step;
	INT maxValue;
	INT minValue;
	HWND editBoxWnd;
} NumericUpDownData;

WNDPROC NUDeditWindowProc;
BOOL CreateNumericUpDownClass(HINSTANCE hInstance);

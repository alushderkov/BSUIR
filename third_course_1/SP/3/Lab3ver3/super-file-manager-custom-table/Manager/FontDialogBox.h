#pragma once
#include <Windows.h>
#include <stdio.h>
#include "ErrorHandling.h"

#define IS_DIR(x) (x & FILE_ATTRIBUTE_DIRECTORY)

#define ILIA_VERSION

#ifdef ILIA_VERSION
#define FONT_DIR_PATH L"C:\\Windows\\Fonts"
#else
#define FONT_DIR_PATH L"D:\\5st sem\\SysProg\\lab3\\another file manager\\Fonts"
#endif

#define FONT_DIALOG_BOX_CLASSNAME L"FontDialogBox"
#define WS_FONTWINDOW WS_BORDER | WS_SYSMENU
#define FB_DESTROY (WM_USER + 1)
#define FB_SETFONT (WM_USER + 2)

#define IDC_FB_BTN_OK 5887
#define IDC_FB_BTN_CANCEL 5889
#define IDC_FB_BTN_LOAD 5815

#define FB_CB_WINDOWSTYLE (WS_VISIBLE | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST)
#define FB_BTN_WINDOWSTYLE (WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON)

#define FB_X (500)
#define FB_Y (200)
#define FB_WIDTH (500)
#define FB_HEIGHT (410)

#define FB_HORIZONTAL_MARGINS (10)

// somehow I wasn't able to make load button width to fit into the formula
// properly, so it's 5/20 instead of 3/10 :(
#define FB_EDIT_LOAD_X (FB_HORIZONTAL_MARGINS)
#define FB_EDIT_LOAD_Y (10)
#define FB_EDIT_LOAD_WIDTH (((FB_WIDTH-FB_HORIZONTAL_MARGINS*3)*7)/10)
#define FB_EDIT_LOAD_HEIGHT (30)

#define FB_BTN_LOAD_X (FB_HORIZONTAL_MARGINS+FB_EDIT_LOAD_WIDTH)
#define FB_BTN_LOAD_Y (10)
#define FB_BTN_LOAD_WIDTH (((FB_WIDTH-FB_HORIZONTAL_MARGINS*3)*6)/20)
#define FB_BTN_LOAD_HEIGHT (30)


#define FB_FM_VERTICAL_MARGINS (15)

#define FB_FM_X (FB_HORIZONTAL_MARGINS)
#define FB_FM_Y (FB_EDIT_LOAD_Y + FB_EDIT_LOAD_HEIGHT+FB_FM_VERTICAL_MARGINS * 2)
#define FB_FM_WIDTH (FB_WIDTH-FB_HORIZONTAL_MARGINS*3)

#define FB_BTN_OK_X (FB_HORIZONTAL_MARGINS)
#define FB_BTN_OK_Y (FB_FM_Y+4*(FB_FM_VERTICAL_MARGINS+FM_HEIGHT))
#define FB_BTN_OK_WIDTH (((FB_WIDTH-FB_HORIZONTAL_MARGINS*3)/2))
#define FB_BTN_OK_HEIGHT (30)
#define FB_BTN_CANCEL_X (FB_HORIZONTAL_MARGINS*2+FB_BTN_OK_WIDTH)
#define FB_BTN_CANCEL_Y (FB_BTN_OK_Y)
// same here, as in FB_CB_WIDTH
#define FB_BTN_CANCEL_WIDTH (((FB_WIDTH-FB_HORIZONTAL_MARGINS*3)*13/28))
#define FB_BTN_CANCEL_HEIGHT (30)

#define MIN_FONT_SIZE  12
#define MAX_FONT_SIZE  99
#define START_FONT_SIZE 18


HWND fontDialogBoxHandle;

//typedef struct {
//	LOGFONT* logFontData;
//	INT amntOfFonts;
//	INT capacity;
//} LoadedFontData;


typedef struct {
	WCHAR** fontNames;
	INT amntOfFonts;
	INT capacity;
} LoadedFontNames;

LoadedFontNames previousFonts;
LoadedFontNames relevantFontData;
HWND editBoxWnd;

void InitializePreviousFonts();
void InitializeRelevantFonts();

void FontBox(HWND hWnd);
BOOL CreateFontDialogBoxClass(HINSTANCE hInstance);
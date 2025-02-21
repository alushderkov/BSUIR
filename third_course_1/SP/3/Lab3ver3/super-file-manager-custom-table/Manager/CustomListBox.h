#pragma once
#include <stdio.h>
#include <Windows.h>
#include "File.h"

#define CUSTOM_LIST_BOX_CLASSNAME L"CustomListBox"

#define EDIT_WINDOW_LEFT 5
#define EDIT_WINDOW_TOP 5
#define EDIT_WINDOW_HEIGHT 26
#define EDIT_WINDOW_WIDTH 5

#define EDIT_WINDOW_FONT_HEIGHT EDIT_WINDOW_HEIGHT

#define LIST_WINDOW_LEFT 0
#define LIST_WINDOW_TOP 0
#define LIST_WINDOW_HEIGHT (windowHeight - LIST_WINDOW_TOP)
#define LIST_WINDOW_WIDTH (windowWidth / 2)
#define LISTBOX_ITEM_MARGIN 23

#define SORTED_LIST_WINDOW_LEFT (windowWidth/ 2 )
#define SORTED_LIST_WINDOW_TOP 0
#define SORTED_LIST_WINDOW_HEIGHT (windowHeight)
#define SORTED_LIST_WINDOW_WIDTH (windowWidth / 2)

#define SCROLL_BUTTON_UP_HEIGHT 20
#define SCROLL_BUTTON_UP_WIDTH 20
#define SCROLL_BUTTON_UP_LEFT LIST_WINDOW_WIDTH - SCROLL_BUTTON_UP_WIDTH
#define SCROLL_BUTTON_UP_TOP 0

#define SCROLL_BUTTON_DOWN_HEIGHT 20
#define SCROLL_BUTTON_DOWN_WIDTH 20
#define SCROLL_BUTTON_DOWN_LEFT LIST_WINDOW_WIDTH - SCROLL_BUTTON_DOWN_WIDTH
#define SCROLL_BUTTON_DOWN_TOP SCROLL_BUTTON_UP_HEIGHT

#define TEXT_LEFT_STEP 5
#define TEXT_UP_STEP   5
#define TEXT_DOWN_STEP TEXT_UP_STEP
#define LISTBOX_HEADER_MARGIN 29


#define IDC_EDIT 542
#define IDC_BUTTON 442
#define IDC_LISTBOX1 642
#define IDC_LISTBOX2 742

#define rgbEDITBOX_BK (RGB(173,168,79))
#define rgbEDITBOX_TEXT (RGB(0, 0, 0))
#define rgbLISTBOX_BK (RGB(173,173,173))
#define rgbLISTBOX_HEADER (RGB(42,173,121))
#define rgbBORDER_LINE (RGB	(255, 0, 0))

#define IDC_BUTTON_UP 333
#define IDC_BUTTON_DOWN 444

#define WM_CREATE_EDIT WM_USER + 1
#define WM_SET_ITEM_AND_HEADER_BG WM_USER + 2
#define WM_SET_FONTS WM_USER + 3
#define CLB_RBUTTON_DOWN 4
#define FontMax(a, b) ((a) > (b) ? (a) : (b))

typedef enum {
	CUSTOM_WINDOW,
	HEADER,
	NAME_COLUMN,
	ATTR_COLUMN
} FONTS;
#define FONTS_COUNT 4 /* counted manually, keep these in sync */


typedef struct {
	FileArray arr;
	INT pos;	
	HWND upButton;
	HWND downButton;
	HWND listHWND;
	INT width;
	INT height;
	INT headerHeight;
	INT capacity;
	INT itemHeight;
	HFONT fonts[FONTS_COUNT];
	HBRUSH itemBgColor;
	HBRUSH headerBgColor;
	HWND customWindow;
} ListBoxData;

WNDPROC editWndProc;
ListBoxData sortedListBox;
ListBoxData listBox;


LRESULT CALLBACK ListBox_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ProceedButtonUp(ListBoxData* data);
void ProceedButtonDown(ListBoxData* data);

BOOL CreateCustomListBoxClass(HINSTANCE hInstance);
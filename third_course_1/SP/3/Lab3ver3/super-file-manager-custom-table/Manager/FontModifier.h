#pragma once
#include "FontDialogBox.h"
#include <Windows.h>
#include "NumericUpDown.h"
#define FONT_MODIFIER_CLASSNAME L"FontModifier"

#define FM_HEIGHT (50)
#define FM_CB_WINDOWSTYLE (WS_VISIBLE | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST)
#define FM_TEXT_HEIGHT (20)
#define FM_CB_HEIGHT (100)
#define FM_NUMERICUPDOWN_HEIGHT (30)

typedef struct {
	INT windowWidth;
	INT windowHeight;

	WCHAR* fontName;

	HANDLE hCombobox;
	LoadedFontNames* loadedFonts;
	INT chosenFontNameIdx;
	NumericUpDownData NUDdata;
} FontModifierData;

FontModifierData fmd_customWindow;
FontModifierData fmd_header;
FontModifierData fmd_nameColumn;
FontModifierData fmd_attrColumn;

LRESULT CALLBACK FontModifier_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CreateFontModifierClass(HINSTANCE hInstance);
void RefreshFonts(FontModifierData* data);
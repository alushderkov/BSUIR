#include "FontModifier.h"
#include "FontDialogBox.h"

FontModifierData* IdentifyFontModifier(HWND hWnd) {
	return (FontModifierData*)GetWindowLongPtr(hWnd, 0);
}

void RefreshFonts(FontModifierData* data) {
	int cbElements = data->loadedFonts->amntOfFonts;
	//LOGFONT* logFontsArray = data->loadedFonts->logFontData;
	SendMessage(data->hCombobox, CB_RESETCONTENT, NULL, NULL);
	WCHAR** addedFontNames = data->loadedFonts->fontNames;
	for (int i = 0; i < cbElements; i++) {
		SendMessage(data->hCombobox, CB_ADDSTRING, NULL, addedFontNames[i]);
	}
	SendMessage(data->hCombobox, CB_SETCURSEL, data->chosenFontNameIdx, 0);
}

LRESULT CALLBACK FontModifier_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	FontModifierData* fmData = IdentifyFontModifier(hWnd);
	switch (uMsg)
	{
	case WM_CREATE: {
		CREATESTRUCTA* windowParams = ((CREATESTRUCTA*)(lParam));
		fmData = windowParams->lpCreateParams;

		fmData->windowWidth = windowParams->cx;
		fmData->windowHeight = windowParams->cy;
		
		SetWindowLongPtr(hWnd, 0, (LONG_PTR)fmData);
		// use cx and cy as width and height respectively

		int cbWidth = windowParams->cx * 7 / 10;
		int numUpDown = windowParams->cx * 3 / 10;


		fmData->hCombobox = CreateWindowEx(0, L"COMBOBOX", L"CB_FONT", FM_CB_WINDOWSTYLE,
			0, FM_TEXT_HEIGHT, cbWidth, FM_CB_HEIGHT, hWnd, NULL, NULL, NULL);
		// TODO: must be a numeric up down
		CreateWindowEx(0, NUMERIC_UP_DOWN_CLASSNAME, L"NUMERICUPDOWN", WS_VISIBLE | WS_CHILD,
			cbWidth, FM_TEXT_HEIGHT, numUpDown, FM_NUMERICUPDOWN_HEIGHT, hWnd, NULL, NULL, &(fmData->NUDdata));


		RefreshFonts(fmData);
		SendMessage(fmData->hCombobox, CB_SETCURSEL, fmData->chosenFontNameIdx, 0);

		break;
	}
	case WM_PAINT: {
		HDC hdc = GetDC(hWnd);
		RECT textRect = { 0,0,fmData->windowWidth,FM_TEXT_HEIGHT };
		DrawTextW(hdc, fmData->fontName, -1, &textRect, 0);
		ReleaseDC(hWnd, hdc);
		break;
	}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CreateFontModifierClass(HINSTANCE hInstance) {
	WNDCLASSEX wfontmodifier;
	memset(&wfontmodifier, 0, sizeof(wfontmodifier));

	//Creating new fontmodifier
	wfontmodifier.cbSize = sizeof(WNDCLASSEX);
	wfontmodifier.lpfnWndProc = FontModifier_WindowProc;
	wfontmodifier.hInstance = hInstance;
	wfontmodifier.lpszClassName = FONT_MODIFIER_CLASSNAME;
	wfontmodifier.cbWndExtra = sizeof(FontModifierData*);
	if (!RegisterClassEx(&wfontmodifier)) {
		MessageBox(0, L"Unable to registrate window", L"Registration error!", MB_OK);
		return FALSE;
	}
	return TRUE;
}
#include <Windows.h>
#include "FontDialogBox.h"
#include "FontModifier.h"
#include "ErrorHandling.h"
#include "FontModifier.h"


int CALLBACK AddFontProc(const LOGFONT* lpLogFont, const TEXTMETRIC* lpTextMetric, int FontType, LPARAM lParam);

void SetFonts(HANDLE hDestWindow) {
	HANDLE currCombobox;
	int chosenIdx;
	int fontNameLength;
	LOGFONT* fonts = (LOGFONT*)LoggedMalloc(4 * sizeof(LOGFONT));

	// todo: for each fontmodifierdata the font size field 
	// must change according to the numericUpDown value.
	// optionally add the handle to its class in FontModifierData??
	for (int i = 0; i < 4; i++) {
		fonts[i].lfWidth = 0;
		fonts[i].lfEscapement = 0;
		fonts[i].lfOrientation = 0;
		fonts[i].lfWeight = FW_MEDIUM;
		fonts[i].lfItalic = FALSE;
		fonts[i].lfUnderline = FALSE;
		fonts[i].lfStrikeOut = FALSE;
		fonts[i].lfCharSet = DEFAULT_CHARSET;
		fonts[i].lfOutPrecision = OUT_DEFAULT_PRECIS;
		fonts[i].lfClipPrecision = CLIP_DEFAULT_PRECIS;
		fonts[i].lfQuality = DEFAULT_QUALITY;
		fonts[i].lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	}

	fonts[0].lfHeight = fmd_customWindow.NUDdata.value;
	currCombobox = fmd_customWindow.hCombobox;
	chosenIdx = SendMessage(currCombobox, CB_GETCURSEL, 0, 0);
	fmd_customWindow.chosenFontNameIdx = chosenIdx;
	fontNameLength = wcslen(fmd_customWindow.loadedFonts->fontNames[chosenIdx]);
	wcsncpy_s(fonts[0].lfFaceName, sizeof(fonts[0].lfFaceName) / sizeof(WCHAR), fmd_customWindow.loadedFonts->fontNames[chosenIdx], _TRUNCATE);

	fonts[1].lfHeight = fmd_header.NUDdata.value;
	currCombobox = fmd_header.hCombobox;
	chosenIdx = SendMessage(currCombobox, CB_GETCURSEL, 0, 0);
	fmd_header.chosenFontNameIdx = chosenIdx;
	fontNameLength = wcslen(fmd_header.loadedFonts->fontNames[chosenIdx]);
	wcsncpy_s(fonts[1].lfFaceName, sizeof(fonts[1].lfFaceName) / sizeof(WCHAR), fmd_header.loadedFonts->fontNames[chosenIdx], _TRUNCATE);

	fonts[2].lfHeight = fmd_nameColumn.NUDdata.value;
	currCombobox = fmd_nameColumn.hCombobox;
	chosenIdx = SendMessage(currCombobox, CB_GETCURSEL, 0, 0);
	fmd_nameColumn.chosenFontNameIdx = chosenIdx;
	fontNameLength = wcslen(fmd_nameColumn.loadedFonts->fontNames[chosenIdx]);
	wcsncpy_s(fonts[2].lfFaceName, sizeof(fonts[2].lfFaceName) / sizeof(WCHAR), fmd_nameColumn.loadedFonts->fontNames[chosenIdx], _TRUNCATE);

	fonts[3].lfHeight = fmd_attrColumn.NUDdata.value;
	currCombobox = fmd_attrColumn.hCombobox;
	chosenIdx = SendMessage(currCombobox, CB_GETCURSEL, 0, 0);
	fmd_attrColumn.chosenFontNameIdx = chosenIdx;
	fontNameLength = wcslen(fmd_attrColumn.loadedFonts->fontNames[chosenIdx]);
	wcsncpy_s(fonts[3].lfFaceName, sizeof(fonts[3].lfFaceName) / sizeof(WCHAR), fmd_attrColumn.loadedFonts->fontNames[chosenIdx], _TRUNCATE);


	SendMessage(hDestWindow, FB_SETFONT, fonts, NULL);
}

void AddNewRelevantFont(WCHAR* filePath) {
	int res = AddFontResourceEx(filePath, FR_PRIVATE, NULL);
	HDC hdc = GetDC(NULL);
	EnumFontFamiliesEx(hdc, NULL, AddFontProc, 0, 0);
	ReleaseDC(NULL, hdc);
}

LRESULT CALLBACK FontDialogBox_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CREATE: {

		CreateWindowEx(0, FONT_MODIFIER_CLASSNAME, L"FM_FONT1", WS_CHILD | WS_VISIBLE,
			FB_FM_X, FB_FM_Y, FB_FM_WIDTH, FM_HEIGHT, hWnd, NULL, NULL, &fmd_customWindow);
		CreateWindowEx(0, FONT_MODIFIER_CLASSNAME, L"FM_FONT2", WS_CHILD | WS_VISIBLE,
			FB_FM_X, FB_FM_Y+(FB_FM_VERTICAL_MARGINS+FM_HEIGHT), FB_FM_WIDTH, FM_HEIGHT, hWnd, NULL, NULL, &fmd_header);
		CreateWindowEx(0, FONT_MODIFIER_CLASSNAME, L"FM_FONT3", WS_CHILD | WS_VISIBLE,
			FB_FM_X, FB_FM_Y+2*(FB_FM_VERTICAL_MARGINS + FM_HEIGHT), FB_FM_WIDTH, FM_HEIGHT, hWnd, NULL, NULL, &fmd_nameColumn);
		CreateWindowEx(0, FONT_MODIFIER_CLASSNAME, L"FM_FONT4", WS_CHILD | WS_VISIBLE,
			FB_FM_X, FB_FM_Y+3*(FB_FM_VERTICAL_MARGINS + FM_HEIGHT), FB_FM_WIDTH, FM_HEIGHT, hWnd, NULL, NULL, &fmd_attrColumn);


		editBoxWnd = CreateWindowEx(0, L"EDIT", L"D:\\Ó÷¸áà\\ÑÏ\\Manager", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER,
			FB_EDIT_LOAD_X, FB_EDIT_LOAD_Y, FB_EDIT_LOAD_WIDTH, FB_EDIT_LOAD_HEIGHT, hWnd, NULL, NULL, NULL);
		CreateWindowEx(0, L"BUTTON", L"Load font", FB_BTN_WINDOWSTYLE,
			FB_BTN_LOAD_X, FB_BTN_LOAD_Y, FB_BTN_LOAD_WIDTH, FB_BTN_LOAD_HEIGHT, hWnd, (HMENU)IDC_FB_BTN_LOAD, NULL, NULL);
		
		CreateWindowEx(0, L"BUTTON", L"Ok", FB_BTN_WINDOWSTYLE,
			FB_BTN_OK_X, FB_BTN_OK_Y, FB_BTN_OK_WIDTH, FB_BTN_OK_HEIGHT, hWnd, (HMENU)IDC_FB_BTN_OK, NULL, NULL);
		CreateWindowEx(0, L"BUTTON", L"Cancel", FB_BTN_WINDOWSTYLE,
			FB_BTN_CANCEL_X, FB_BTN_CANCEL_Y, FB_BTN_CANCEL_WIDTH, FB_BTN_CANCEL_HEIGHT, hWnd, (HMENU)IDC_FB_BTN_CANCEL, NULL, NULL);

		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
			case IDC_FB_BTN_LOAD: {

				TCHAR* text = malloc(MAX_PATH * sizeof(TCHAR));
				GetWindowText(editBoxWnd, text, MAX_PATH);

				AddNewRelevantFont(text);
				free(text);

				RefreshFonts(&fmd_customWindow);
				RefreshFonts(&fmd_header);
				RefreshFonts(&fmd_nameColumn);
				RefreshFonts(&fmd_attrColumn);
				break;
			}
			case IDC_FB_BTN_OK: {			
				HWND parentWnd = GetWindowLongPtrA(hWnd, GWLP_HWNDPARENT);
				SetFonts(parentWnd);
				DestroyWindow(hWnd);
				break;
			}
			case IDC_FB_BTN_CANCEL: {
				DestroyWindow(hWnd);
				break;
			}
		}
		break;
	}
	case WM_DESTROY: {
		HWND parentWnd = GetWindowLongPtrA(hWnd, GWLP_HWNDPARENT);
		EnableWindow(parentWnd,TRUE);
		SetFocus(parentWnd);
		SendMessage(parentWnd, FB_DESTROY, NULL, NULL);
		break;
	}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
};


BOOL CreateFontDialogBoxClass(HINSTANCE hInstance) {
	WNDCLASSEX wfontdialogbox;
	memset(&wfontdialogbox, 0, sizeof(wfontdialogbox));

	//Creating new listbox
	wfontdialogbox.cbSize = sizeof(WNDCLASSEX);
	wfontdialogbox.lpfnWndProc = FontDialogBox_WindowProc;
	wfontdialogbox.hInstance = hInstance;
	wfontdialogbox.lpszClassName = FONT_DIALOG_BOX_CLASSNAME;
	//wfontdialogbox.style = CS_GLOBALCLASS;

	if (!RegisterClassEx(&wfontdialogbox)) {
		MessageBox(0, L"Unable to registrate window", L"Registration error!", MB_OK);
		return FALSE;
	}
	return TRUE;
};

void FontBox(HWND hWnd) {
	HWND fontBox;
	EnableWindow(hWnd, FALSE);
	fontBox = CreateWindowEx(WS_EX_APPWINDOW, FONT_DIALOG_BOX_CLASSNAME, L"Fonts", WS_FONTWINDOW,
		FB_X, FB_Y, FB_WIDTH, FB_HEIGHT, hWnd, NULL, NULL, NULL);

	ShowWindow(fontBox, SW_SHOW);
}

void InitializeFontModifierData(HANDLE hMainWindow) {

	fmd_customWindow.fontName = L"Custom window font";
	fmd_customWindow.loadedFonts = &relevantFontData;
	fmd_customWindow.chosenFontNameIdx = 0;
	fmd_customWindow.NUDdata.maxValue = MAX_FONT_SIZE;
	fmd_customWindow.NUDdata.minValue = MIN_FONT_SIZE;
	fmd_customWindow.NUDdata.step = 1;
	fmd_customWindow.NUDdata.value = START_FONT_SIZE;
	

	fmd_header.fontName = L"Header font";
	fmd_header.loadedFonts = &relevantFontData;
	fmd_header.chosenFontNameIdx = 0;
	fmd_header.NUDdata.maxValue = MAX_FONT_SIZE;
	fmd_header.NUDdata.minValue = MIN_FONT_SIZE;
	fmd_header.NUDdata.step = 1;
	fmd_header.NUDdata.value = START_FONT_SIZE;

	fmd_nameColumn.fontName = L"Name column font";
	fmd_nameColumn.loadedFonts = &relevantFontData;
	fmd_nameColumn.chosenFontNameIdx = 0;
	fmd_nameColumn.NUDdata.maxValue = MAX_FONT_SIZE;
	fmd_nameColumn.NUDdata.minValue = MIN_FONT_SIZE;
	fmd_nameColumn.NUDdata.step = 1;
	fmd_nameColumn.NUDdata.value = START_FONT_SIZE;

	// TODO OPTIONAL: change to fit the needs of a specific task
	fmd_attrColumn.fontName = L"Attribute column";
	fmd_attrColumn.loadedFonts = &relevantFontData;
	fmd_attrColumn.chosenFontNameIdx = 0;
	fmd_attrColumn.NUDdata.maxValue = MAX_FONT_SIZE;
	fmd_attrColumn.NUDdata.minValue = MIN_FONT_SIZE;
	fmd_attrColumn.NUDdata.step = 1;
	fmd_attrColumn.NUDdata.value = START_FONT_SIZE;

	SetFonts(hMainWindow);
}

#pragma region Remember font section

void RememberFont(WCHAR* fontName) {
	if (previousFonts.amntOfFonts == previousFonts.capacity) {
		if (!IncreasePreviousFontsCapacity()) {
			return;
		}
	}

	int fontNameLength = wcslen(fontName) + 1;
	WCHAR* addedFontName = LoggedMalloc(sizeof(WCHAR) * fontNameLength);
	if (addedFontName == NULL) {
		return;
	}

	wcscpy_s(addedFontName, fontNameLength, fontName);
	previousFonts.fontNames[previousFonts.amntOfFonts++] = addedFontName;
}

BOOL RememberedFontsContain(WCHAR* fontName) {
	for (int i = 0; i < previousFonts.amntOfFonts; i++) {
		if (wcscmp(fontName, previousFonts.fontNames[i])==0) {
			return TRUE;
		}
	}
	return FALSE;
}
#pragma endregion


#pragma region Relevant font data operations

BOOL IncreaseRelevantFontsCapacity(){
	relevantFontData.capacity = relevantFontData.capacity * 2;
	WCHAR** oldptr = relevantFontData.fontNames;
	relevantFontData.fontNames = realloc(relevantFontData.fontNames, relevantFontData.capacity*sizeof(WCHAR*));
	if (relevantFontData.fontNames != NULL) {
		return TRUE;
	}
	relevantFontData.fontNames = oldptr;
	return FALSE;
}

void AddExistingRelevantFont(WCHAR* fontName) {
	if (relevantFontData.amntOfFonts == relevantFontData.capacity) {
		if (!IncreaseRelevantFontsCapacity()) {
			return;
		}
	}

	int fontNameLength = wcslen(fontName) + 1;
	WCHAR* addedFontName = LoggedMalloc(sizeof(WCHAR) * fontNameLength);
	if (addedFontName == NULL) {
		return;
	}

	wcscpy_s(addedFontName, fontNameLength, fontName);
	relevantFontData.fontNames[relevantFontData.amntOfFonts++] = addedFontName;
}


// for further additions
int CALLBACK AddFontProc(const LOGFONT* lpLogFont, const TEXTMETRIC* lpTextMetric, int FontType, LPARAM lParam) {
	if (!RememberedFontsContain(lpLogFont->lfFaceName)) {
		RememberFont(lpLogFont->lfFaceName);
		AddExistingRelevantFont(lpLogFont->lfFaceName);
	}
	return 1;
}

void InitializeRelevantFonts() {
	relevantFontData.capacity = 100;
	relevantFontData.amntOfFonts = 0;
	relevantFontData.fontNames = (WCHAR**)LoggedMalloc(sizeof(WCHAR*) * 100);
	if (relevantFontData.fontNames == NULL) {
		return;
	}
	
	
	int dirSize = wcslen(FONT_DIR_PATH) + 1;
	WCHAR* path = LoggedMalloc(sizeof(WCHAR) * (dirSize + 2));
	if (path == NULL) {
		return;
	}
	
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	
	wcscpy_s(path, dirSize + 2, FONT_DIR_PATH);
	wcscat_s(path, dirSize + 2, L"\\*");
	
	hFind = FindFirstFile(path, &ffd);
	
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
	
			if (!IS_DIR(ffd.dwFileAttributes)) {
				int fileNameLength = wcslen(ffd.cFileName);
				int filePathLength = fileNameLength + dirSize + 1;
				WCHAR* fileName = LoggedMalloc(sizeof(WCHAR) * filePathLength);
				if (fileName == NULL) {
					return FALSE;
				}
				wcscpy_s(fileName, filePathLength, FONT_DIR_PATH);
				wcscat_s(fileName, filePathLength, L"\\");
				wcscat_s(fileName, filePathLength, ffd.cFileName);
					
				int res = AddFontResourceEx(fileName, FR_PRIVATE, NULL);

				// get file name without extension
				// assuming it only has one dot
				WCHAR* pDot = wcsrchr(ffd.cFileName, L'.');

				if (pDot != NULL) {
					size_t length = pDot - ffd.cFileName;
					WCHAR* fontName = (WCHAR*)malloc(sizeof(WCHAR) * (length + 1));
					wcsncpy_s(fontName, length+1, ffd.cFileName, length);
					fontName[length] = L'\0';
					AddExistingRelevantFont(fontName);

				}
				else {
					// No extension found, copy the whole filePath
					size_t fontNameLength = wcslen(ffd.cFileName);
					WCHAR* fontName = (WCHAR*)malloc(sizeof(WCHAR) * (fontNameLength + 1));
					wcsncpy_s(fontName, fontNameLength, ffd.cFileName, fontNameLength+1);
					AddExistingRelevantFont(fontName);
				}

				
			}
		} while (FindNextFile(hFind, &ffd) != 0);
	}
	FindClose(hFind);
	free(path);

	//HDC hdc = GetDC(NULL);
	//EnumFontFamiliesEx(hdc, NULL, AddExistingFontProc, 0, 0);
	//ReleaseDC(NULL, hdc);
}

#pragma endregion


#pragma region Previous font data operations

BOOL IncreasePreviousFontsCapacity() {
	previousFonts.capacity = previousFonts.capacity * 2;
	WCHAR** oldptr = previousFonts.fontNames;
	previousFonts.fontNames = realloc(previousFonts.fontNames, previousFonts.capacity * sizeof(WCHAR*));
	if (previousFonts.fontNames != NULL) {
		return TRUE;
	}
	previousFonts.fontNames = oldptr;
	return FALSE;
}

int CALLBACK RememberProc(const LOGFONT* lpLogFont, const TEXTMETRIC* lpTextMetric, int FontType, LPARAM lParam) {
	if (!RememberedFontsContain(lpLogFont->lfFaceName)) {
		RememberFont(lpLogFont->lfFaceName);
	}
	return 1; 
}

void InitializePreviousFonts() {
	previousFonts.capacity = 100;
	previousFonts.amntOfFonts = 0;
	previousFonts.fontNames = (WCHAR**)LoggedMalloc(sizeof(WCHAR*) * 100);
	if (previousFonts.fontNames == NULL) {
		return;
	}

	HDC hdc = GetDC(NULL);
	EnumFontFamiliesEx(hdc, NULL, RememberProc, 0, 0);
	ReleaseDC(NULL, hdc);
}
#pragma endregion
#include <windows.h>
#include <commctrl.h>
#include "FileSorter.h"
#include "Cell.h"
#include "Utils.h"
#pragma comment(lib, "comctl32.lib")


#define ID_EDIT 1
#define ID_FINC 2
#define ID_FDEC 3
#define ID_SINC 4
#define ID_SDEC 5
#define ID_CREATEFONTWND 6
#define ID_FWNDOK 7
#define ID_FWNDCANCEL 8

#define ID_FFTYPECOMBO 9
#define ID_CFTYPECOMBO 10
#define ID_HFTYPECOMBO 11

#define ID_FFSIZECOMBO 12
#define ID_CFSIZECOMBO 13
#define ID_HFSIZECOMBO 14
#define ID_FWNDEDIT 15
#define ID_FWNDADD 16
#define ID_FWNDFILE 17


static HWND hMainWindow;
WNDPROC oldEditProc;
static int lastIndex = 5;
static std::vector<std::pair<std::wstring, std::wstring>> fileData, sortedData;
static HWND hEdit, hFDecButton, hFIncButton, hSDecButton, hSIncButton;
static std::map<std::wstring, int> fonts =
{
	{L"Arial",0},
	{L"Corbel",1},
	{L"Calibri",2},
	{L"Times New Roman",3},
	{L"Georgia",4}
},
sizes =
{
	{L"10",0},
	{L"12",1},
	{L"14",2},
	{L"16",3},
	{L"18",4},
	{L"20",5},
	{L"22",6},
	{L"24",7},
};
static std::wstring fileFontName = L"Arial", headerFontName = L"Arial", criterionFontName = L"Arial";
static int fPageNum = 0, sPageNum = 0, maxPages = 0, fileFontSize = 20, headerFontSize = 20, criterionFontSize = 20, fntChoiceRes = ID_FWNDCANCEL;

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FontWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK subEditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RETURN:
			HWND parentHwnd = GetParent(hwnd);
			fileData.clear();
			sortedData.clear();
			sPageNum = 0;
			fPageNum = 0;
			wchar_t path[MAX_PATH];
			GetWindowText(hEdit, path, MAX_PATH);
			FileSorter::ProcessDirectory(fileData, path);
			maxPages = (int)ceil(fileData.size() / (MAX_TABLE_HEIGHT / Utils::GetMaxFontSize(fileFontSize, criterionFontSize) - 1));
			sortedData = fileData;
			FileSorter::QSort(sortedData, 0, sortedData.size() - 1);
			InvalidateRect(parentHwnd, NULL, FALSE);
			return 0;
		}
	default:
		return CallWindowProc(oldEditProc, hwnd, msg, wParam, lParam);
	}
	return 0;
}

static HWND CreateFontWindow(HWND hParent)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = FontWindowProc;
	wc.lpszClassName = L"FontWindow";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	RegisterClass(&wc);


	int x = (GetSystemMetrics(SM_CXSCREEN) - 500) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - 300) / 2;
	HWND hModalWnd = CreateWindow(
		wc.lpszClassName,
		L"Выбор шрифтов",
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		x, y, 500, 350,
		hParent, NULL, GetModuleHandle(NULL), NULL
	);

	ShowWindow(hModalWnd, SW_SHOW);

	return hModalWnd;
}
LRESULT CALLBACK FontWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hFileFontTypeCombo, hCriterionFontTypeCombo, hHeaderFontTypeCombo,
		hFileFontSizeCombo, hCriterionFontSizeCombo, hHeaderFontSizeCombo,
		hOkButton, hCancelButton, hCustomFontEdit, hAddCustomFont, hButtonFile;
	switch (uMsg)
	{
	case WM_PAINT:
	{
		std::wstring fCap = L"Шрифт файлов",
			sCap = L"Шрифт заголовков",
			tCap = L"Шрифт критерия",
			fthCap = L"Пользовательский шрифт";
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 10, 15, fCap.c_str(), fCap.length());
		TextOut(hdc, 10, 65, sCap.c_str(), sCap.length());
		TextOut(hdc, 10, 115, tCap.c_str(), tCap.length());
		TextOut(hdc, 10, 165, fthCap.c_str(), fthCap.length());
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_CREATE:
	{
		hFileFontTypeCombo = CreateWindow(L"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
			170, 10, 200, 150,
			hwnd, (HMENU)ID_FFTYPECOMBO,
			GetModuleHandle(NULL), NULL);

		hHeaderFontTypeCombo = CreateWindow(L"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
			170, 60, 200, 150,
			hwnd, (HMENU)ID_HFTYPECOMBO,
			GetModuleHandle(NULL), NULL);

		hCriterionFontTypeCombo = CreateWindow(L"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
			170, 110, 200, 150,
			hwnd, (HMENU)ID_CFTYPECOMBO,
			GetModuleHandle(NULL), NULL);

		hFileFontSizeCombo = CreateWindow(L"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
			380, 10, 100, 200,
			hwnd, (HMENU)ID_FFSIZECOMBO,
			GetModuleHandle(NULL), NULL);

		hHeaderFontSizeCombo = CreateWindow(L"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
			380, 60, 100, 200,
			hwnd, (HMENU)ID_HFSIZECOMBO,
			GetModuleHandle(NULL), NULL);

		hCriterionFontSizeCombo = CreateWindow(L"COMBOBOX", NULL,
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
			380, 110, 100, 200,
			hwnd, (HMENU)ID_CFSIZECOMBO,
			GetModuleHandle(NULL), NULL);

		hCustomFontEdit = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 190, 163, 288, 20, hwnd, (HMENU)ID_FWNDEDIT,
			GetModuleHandle(NULL), NULL);

		hButtonFile = CreateWindow(L"BUTTON", L"Выбрать файл", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			250, 190, 125, 25, hwnd, (HMENU)ID_FWNDFILE, GetModuleHandle(NULL), NULL);

		hAddCustomFont = CreateWindow(L"BUTTON", L"Добавить", WS_CHILD | WS_VISIBLE | WS_BORDER, 378, 190, 100, 25, hwnd,
			(HMENU)ID_FWNDADD, GetModuleHandle(NULL), NULL);


		Utils::FillComboBox(hFileFontTypeCombo, fonts);
		Utils::FillComboBox(hHeaderFontTypeCombo, fonts);
		Utils::FillComboBox(hCriterionFontTypeCombo, fonts);


		Utils::FillComboBox(hFileFontSizeCombo, sizes);
		Utils::FillComboBox(hHeaderFontSizeCombo, sizes);
		Utils::FillComboBox(hCriterionFontSizeCombo, sizes);

		SendMessage(hFileFontTypeCombo, CB_SETCURSEL, fonts[fileFontName], 0);
		SendMessage(hHeaderFontTypeCombo, CB_SETCURSEL, fonts[headerFontName], 0);
		SendMessage(hCriterionFontTypeCombo, CB_SETCURSEL, fonts[criterionFontName], 0);

		SendMessage(hFileFontSizeCombo, CB_SETCURSEL, sizes[std::to_wstring(fileFontSize)], 0);
		SendMessage(hHeaderFontSizeCombo, CB_SETCURSEL, sizes[std::to_wstring(headerFontSize)], 0);
		SendMessage(hCriterionFontSizeCombo, CB_SETCURSEL, sizes[std::to_wstring(criterionFontSize)], 0);

		hOkButton = CreateWindow(L"BUTTON", L"Ок", WS_CHILD | WS_VISIBLE | WS_BORDER, 270, 270, 100, 25, hwnd, (HMENU)ID_FWNDOK, GetModuleHandle(NULL), NULL);
		hCancelButton = CreateWindow(L"BUTTON", L"Отмена", WS_CHILD | WS_VISIBLE | WS_BORDER, 380, 270, 100, 25, hwnd, (HMENU)ID_FWNDCANCEL, GetModuleHandle(NULL), NULL);
	}
	break;
	case WM_DESTROY:
	{
		EnableWindow(hMainWindow, true);
		SetForegroundWindow(hMainWindow);
		SetActiveWindow(hMainWindow);
		InvalidateRect(hMainWindow, NULL, FALSE);
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_FWNDOK:
		{
			Utils::GetFontTypeChoice(hFileFontTypeCombo, fileFontName);
			Utils::GetFontTypeChoice(hHeaderFontTypeCombo, headerFontName);
			Utils::GetFontTypeChoice(hCriterionFontTypeCombo, criterionFontName);

			Utils::GetFontSizeChoice(hFileFontSizeCombo, fileFontSize);
			Utils::GetFontSizeChoice(hHeaderFontSizeCombo, headerFontSize);
			Utils::GetFontSizeChoice(hCriterionFontSizeCombo, criterionFontSize);
			fntChoiceRes = ID_FWNDOK;
			DestroyWindow(hwnd);
		}
		break;
		case ID_FWNDCANCEL:
		{
			fntChoiceRes = ID_FWNDCANCEL;
			DestroyWindow(hwnd);
		}
		break;
		case ID_FWNDADD:
		{
			wchar_t path[MAX_PATH];
			GetWindowText(hCustomFontEdit, path, MAX_PATH);
			std::wstring fontName = Utils::GetFontName(path);
			AddFontResourceEx(path, FR_PRIVATE, NULL);
			SendMessage(hFileFontTypeCombo, CB_ADDSTRING, 0, (LPARAM)fontName.c_str());
			SendMessage(hHeaderFontTypeCombo, CB_ADDSTRING, 0, (LPARAM)fontName.c_str());
			SendMessage(hCriterionFontTypeCombo, CB_ADDSTRING, 0, (LPARAM)fontName.c_str());
			fonts.emplace(std::pair<std::wstring, int >(fontName, lastIndex));
			lastIndex++;
		}
		break;
		case ID_FWNDFILE:
		{
			OPENFILENAME ofn;       
			wchar_t szFile[260];    

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';  
			ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
			ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn) == TRUE) {
				SetWindowText(hCustomFontEdit, ofn.lpstrFile);
			}
		}
		break;
		}
	}
	break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HFONT
		hHeaderFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
			VARIABLE_PITCH, L"Arial"),

		hFileFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
			VARIABLE_PITCH, L"Arial"),

		hCriterionFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
			VARIABLE_PITCH, L"Arial");

	switch (uMsg) {
	case WM_ENABLE:
	{
		if (fntChoiceRes == ID_FWNDOK)
		{
			DeleteObject(hHeaderFont);
			DeleteObject(hFileFont);
			DeleteObject(hCriterionFont);

			hHeaderFont = CreateFont(headerFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
				CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
				VARIABLE_PITCH, headerFontName.c_str());

			hFileFont = CreateFont(fileFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
				CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
				VARIABLE_PITCH, fileFontName.c_str());

			hCriterionFont = CreateFont(criterionFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
				CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
				VARIABLE_PITCH, criterionFontName.c_str());

			maxPages = (int)ceil(fileData.size() / (MAX_TABLE_HEIGHT / Utils::GetMaxFontSize(fileFontSize, criterionFontSize) - 1));
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP hbmMem = CreateCompatibleBitmap(hdc, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
		HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

		FillRect(hdcMem, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		std::wstring total = L"Всего файлов: " + std::to_wstring(fileData.size());
		SelectObject(hdcMem, GetStockObject(DC_PEN));
		SetDCPenColor(hdcMem, RGB(0, 0, 255));
		SetBkMode(hdc, TRANSPARENT);

		TextOut(hdcMem, SX_OFFS-70, MAX_TABLE_HEIGHT + 5, total.c_str(), total.length());
		SendMessage(hEdit, WM_SETFONT, (WPARAM)hHeaderFont, TRUE);
		///////////////
		RECT rect;
		GetWindowRect(hEdit, &rect);
		POINT topLeft = { rect.left, rect.top };
		ScreenToClient(GetParent(hEdit), &topLeft);
		int currentX = topLeft.x;
		int currentY = topLeft.y;
		int currentWidth = rect.right - rect.left;
		int currentHeight = rect.bottom - rect.top;
		int newY = (30 - headerFontSize) / 2;
		MoveWindow(hEdit, currentX, newY, currentWidth, currentHeight, TRUE);
		/////
		Utils::DrawTable(hdcMem, hHeaderFont, hFileFont, hCriterionFont, fPageNum, sPageNum, headerFontSize, fileFontSize, criterionFontSize, fileData, sortedData);
		BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left,
			ps.rcPaint.bottom - ps.rcPaint.top, hdcMem, 0, 0, SRCCOPY);
		SelectObject(hdcMem, hbmOld);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_HOTKEY:
	{
		if (wParam == ID_CREATEFONTWND)
		{
			HWND fontWnd = CreateFontWindow(hwnd);
			EnableWindow(hwnd, false);
		}
	}
	break;
	case WM_CREATE:
	{
		hEdit = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE,
			FX_OFFS + 5, Y_OFFS + BORDER_THICKNESS + 1, 375, 20, hwnd, (HMENU)ID_EDIT, GetModuleHandle(NULL), NULL);
		oldEditProc = (WNDPROC)SetWindowLongPtr(hEdit, GWLP_WNDPROC, (LONG_PTR)subEditProc);
		hFIncButton = CreateWindow(L"BUTTON", L"down", WS_CHILD | WS_VISIBLE | WS_BORDER,
			FX_OFFS + CW1, MAX_TABLE_HEIGHT + Y_OFFS + 5, 100, 25, hwnd, (HMENU)ID_FINC, GetModuleHandle(NULL), NULL);
		hFDecButton = CreateWindow(L"BUTTON", L"up", WS_CHILD | WS_VISIBLE | WS_BORDER,
			FX_OFFS + CW1-100, MAX_TABLE_HEIGHT + Y_OFFS + 5, 100, 25, hwnd, (HMENU)ID_FDEC, GetModuleHandle(NULL), NULL);
		hSIncButton = CreateWindow(L"BUTTON", L"down", WS_CHILD | WS_VISIBLE | WS_BORDER,
			SX_OFFS + CW1, MAX_TABLE_HEIGHT + Y_OFFS + 5, 100, 25, hwnd, (HMENU)ID_SINC, GetModuleHandle(NULL), NULL);
		hSDecButton = CreateWindow(L"BUTTON", L"up", WS_CHILD | WS_VISIBLE | WS_BORDER,
			SX_OFFS + CW1-100, MAX_TABLE_HEIGHT + Y_OFFS + 5, 100, 25, hwnd, (HMENU)ID_SDEC, GetModuleHandle(NULL), NULL);

		RegisterHotKey(hwnd, ID_CREATEFONTWND, MOD_CONTROL | MOD_NOREPEAT, 'F');
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_FINC:
		{
			if (fPageNum < maxPages)
			{
				fPageNum++;
				InvalidateRect(hwnd, NULL, FALSE);
			}
		}
		break;
		case ID_FDEC:
		{
			if (fPageNum > 0)
			{
				fPageNum--;
				InvalidateRect(hwnd, NULL, FALSE);
			}
		}
		break;
		case ID_SINC:
		{
			if (sPageNum < maxPages)
			{
				sPageNum++;
				InvalidateRect(hwnd, NULL, FALSE);
			}
		}
		break;
		case ID_SDEC:
		{
			if (sPageNum > 0)
			{
				sPageNum--;
				InvalidateRect(hwnd, NULL, FALSE);
			}
		}
		break;
		}
	}
	break;
	case WM_DESTROY:
	{
		DeleteObject(hHeaderFont);
		DeleteObject(hFileFont);
		DeleteObject(hCriterionFont);
		PostQuitMessage(0);
	}
	break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}


int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,

	int nCmdShow) {
	const wchar_t CLASS_NAME[] = L"MainWindow";

	WNDCLASS wc = {};
	wc.lpfnWndProc = MainWindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	RegisterClass(&wc);
	hMainWindow = CreateWindow(
		CLASS_NAME,
		L"Сортировщик файлов",
		WS_POPUP | WS_VISIBLE,
		0, 0,
		GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hMainWindow, SW_SHOWMAXIMIZED);
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	fileData.clear();
	sortedData.clear();
	return 0;
}

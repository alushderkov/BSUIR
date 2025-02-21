#include <Windows.h>
#include <stdio.h>
#include "File.h"
#include "Sorter.h"
#include "CustomListBox.h"
#include "ErrorHandling.h"
#include "FontDialogBox.h"
#include "FontModifier.h"
#include "NumericUpDown.h"
#include "CustomTable.h"
#include "InputDialog.h"

#define MAIN_WINDOW_CLASSNAME L"MainWindow"

//#define NO_FRAME

#ifdef NO_FRAME
#define WS_MAINWINDOW  ( WS_POPUP)
#else
#define WS_MAINWINDOW  (WS_OVERLAPPEDWINDOW)
#endif

#define rgbWhite (RGB(255, 255, 255))
#define rgbGray (RGB(128, 128, 128))
#define rgbBlack (RGB(0, 0, 0))

#define WM_SETDATA (WM_USER + 1)

INT windowWidth;
INT windowHeight;

HWND mainWindowHandle;
HWND fileList;
HWND sortedFileList;
HWND pathEdit;
HWND table;

INT x = 1;

int FileData_Comporator(void* a, void* b) {
	return wcscmp( ((FileData*)a)->fileName, ((FileData*)b)->fileName);
}

void CreateUniqueNames(FileArray array) {
	INT count = 1;
	INT j = 0;
	for (int i = 1; i < array.size; i++) {
		if (wcscmp(array.files[i]->fileName, array.files[j]->fileName) == 0)
		{
			INT Size = CharLength(count) + 4;
			WCHAR* AddName = LoggedMalloc(Size * sizeof(WCHAR));
			if (AddName == NULL) return;

			_snwprintf_s(AddName, Size, Size, L" [%d]", count);

			array.files[i]->lengthOfFileName = array.files[i]->lengthOfFileName + Size - 1;
			WCHAR* newName = LoggedMalloc(array.files[i]->lengthOfFileName * sizeof(WCHAR));
			if (newName == NULL) { 
				free(AddName);
				return; 
			}

			wcscpy_s(newName, array.files[i]->lengthOfFileName, array.files[i]->fileName);

			free(array.files[i]->fileName);

			wcscat_s(newName, array.files[i]->lengthOfFileName, AddName);

			array.files[i]->fileName = newName;
			free(AddName);
			count++;
		}
		else {
			count = 1;
			SortPart(array.files, j, i-1, FileData_Comporator);
			j = i;
		}
	}
	SortPart(array.files, j, array.size-1, FileData_Comporator);
}

void RenewLists() {
	sortedListBox.pos = 0;
	listBox.pos = 0;
	InvalidateRect(fileList, NULL, FALSE);
	InvalidateRect(sortedFileList, NULL, FALSE);

	EnableWindow(listBox.upButton, FALSE);
	EnableWindow(sortedListBox.upButton, FALSE);

	if (listBox.pos <= listBox.arr.size - listBox.capacity)
		EnableWindow(listBox.downButton, TRUE);
	else
		EnableWindow(listBox.downButton, FALSE);

	if (sortedListBox.pos <= sortedListBox.arr.size - listBox.capacity)
		EnableWindow(sortedListBox.downButton, TRUE);
	else
		EnableWindow(sortedListBox.downButton, FALSE);

}
void GetAndShowFiles(WCHAR* path) {

	listBox.arr = GetAllFiles(path);
	if (listBox.arr.files == NULL) return;

	FileData** sortedFiles = (FileData**)CreateSortedArray(listBox.arr.files, listBox.arr.size, FileData_Comporator);
	
	if (sortedFiles == NULL) return;

	sortedListBox.arr.capacity = listBox.arr.capacity;
	sortedListBox.arr.size = listBox.arr.size;
	sortedListBox.arr.files = sortedFiles;

	CreateUniqueNames(sortedListBox.arr);

	RenewLists();
	return;
}

void StartSearch() {
	WCHAR* dirPath = LoggedMalloc(sizeof(WCHAR) * (MAX_PATH));
	if (dirPath == NULL)  return; 
	
	INT copied = GetWindowText(pathEdit, dirPath, MAX_PATH);
	DestroyFileArray(listBox.arr);
	GetAndShowFiles(dirPath);
	free(dirPath);
	
}

BOOL ComponentsAreCreated() {
	return fileList != NULL && sortedFileList != NULL;
}

LRESULT CALLBACK EditWindow_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_KEYDOWN: {
		if (wParam == VK_RETURN) {
			StartSearch();
			break;
		}
	}
	}
	return editWndProc(hWnd, uMsg, wParam, lParam);
}

void SearchFilesForSeedingData() {
	WCHAR* dirPath = LoggedMalloc(sizeof(WCHAR) * (MAX_PATH));
	if (dirPath == NULL)  return;

	wcscpy_s(dirPath, MAX_PATH, TABLE_SEED_DATA_DIR_PATH);
	//INT copied = GetWindowText(pathEdit, dirPath, MAX_PATH);
	DestroyFileArray(listBox.arr);
	GetAndShowFiles(dirPath);
	free(dirPath);

}

LRESULT CALLBACK MainWindow_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:	
	{
		LOGFONT* font = LoggedMalloc(sizeof(LOGFONT));
		font->lfWidth = 0;
		font->lfEscapement = 0;
		font->lfOrientation = 0;
		font->lfWeight = FW_MEDIUM;
		font->lfItalic = FALSE;
		font->lfUnderline = FALSE;
		font->lfStrikeOut = FALSE;
		font->lfCharSet = DEFAULT_CHARSET;
		font->lfOutPrecision = OUT_DEFAULT_PRECIS;
		font->lfClipPrecision = CLIP_DEFAULT_PRECIS;
		font->lfQuality = DEFAULT_QUALITY;
		font->lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

		font->lfHeight = TABLE_FONT_SIZE;
		wcsncpy_s(font->lfFaceName, sizeof(font->lfFaceName) / sizeof(WCHAR), TABLE_FONT_NAME, _TRUNCATE);


		CREATESTRUCTA* info = (CREATESTRUCTA*)lParam;
		TableHeader colData[10] = {
		{L"HEADER"},
		{L"HEADER"},
		{L"HEADER"},
		{L"HEADER"},
		{L"HEADER"},
		{L"HEADER"},
		{L"HEADER"},
		{L"HEADER"},
		{L"HEADER"},
		{L"HEADER"}
		};
		table = CreateCustomTable(hWnd, L"Test", 0, 0, info->cx, info->cy, TABLE_AMNT_OF_ROWS, TABLE_AMNT_OF_COLUMNS, colData,TABLE_AMNT_OF_COLUMNS,font);

		//InsertData(table, 0, 0, elem);
		SearchFilesForSeedingData();
		for (int i = 0; i < TABLE_AMNT_OF_ROWS; i++) {
			for (int j = 0; j < TABLE_AMNT_OF_COLUMNS; j++) {
				int idx = i * TABLE_AMNT_OF_ROWS + j;
				if (idx < listBox.arr.size) {
					BOOL modifiable = (i == 0) && (j == 0);
					TableElement elem = CreateStringElement(sortedListBox.arr.files[idx]->fileName, modifiable);
					InsertData(table, i, j, elem);
				}
			}
		}

		InsertData(table, 2, 2,CreateStringElement(L"HEL",TRUE));
		break;
	}
	case FB_SETFONT: {
		
		SendMessage(fileList, WM_SET_FONTS, wParam, NULL);
		SendMessage(sortedFileList, WM_SET_FONTS, wParam, NULL);
		break;
	}
	case WM_PARENTNOTIFY: {
		/*if (LOWORD(wParam) == WM_RBUTTONDOWN) {
			FontBox(hWnd);
		}*/
		break;
	}
	case WM_SIZE: {
		SendMessage(table, WM_SIZE, wParam, lParam);
		break;
	}
	case ID_INPUT_DATA: {
		SendMessage(table, WM_INPUT_DATA, wParam, NULL);
		EnableWindow(table, TRUE);
		SetFocus(table);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CreateMainWindowClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex;

	memset(&wcex, 0, sizeof(wcex));

	//Creating new window class
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_GLOBALCLASS | CS_DBLCLKS ;
	wcex.lpfnWndProc = MainWindow_WindowProc;
	wcex.hInstance = hInstance;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = MAIN_WINDOW_CLASSNAME;

	if (!RegisterClassEx(&wcex)) {
		MessageBox(0, L"Unable to registrate window", L"Registration error!", MB_OK);
		return FALSE;
	}
	return TRUE;
}

INT WINAPI WinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ PSTR lpCmdLine,_In_ int nCmdShow)
{
	MSG msg;


	if (!CreateMainWindowClass(hInstance) || !CreateCustomListBoxClass(hInstance) || !CreateFontDialogBoxClass(hInstance) ||
		!CreateNumericUpDownClass(hInstance) || !CreateFontModifierClass(hInstance) || !CreateCustomTableClass(hInstance) ||
		!CreateCustomInputDialogClass(hInstance))
		return 0;


#ifdef NO_FRAME
	windowHeight = GetSystemMetrics(SM_CYSCREEN);
	windowWidth = GetSystemMetrics(SM_CXSCREEN);
#else
	windowHeight = GetSystemMetrics(SM_CYMAXIMIZED);
	windowWidth = GetSystemMetrics(SM_CXMAXIMIZED);
#endif

	SetDoubleClickTime(200);
	mainWindowHandle = CreateWindowEx(0, MAIN_WINDOW_CLASSNAME, L"File manager", WS_MAINWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

	if (mainWindowHandle == NULL) {
		MessageBox(0, L"Unable to create window", L"Creation error!", MB_OK);
		return 0;
	}
	InitializePreviousFonts();
	InitializeRelevantFonts();
	InitializeFontModifierData(mainWindowHandle);
	
	if (!InitializeFileLogging(mainWindowHandle)) {
		MessageBox(0, L"Unable to open file for logging", L"File logging error!", MB_OK);
		return 0;
	}
	StartSearch();
	ShowWindow(mainWindowHandle, SW_SHOWMAXIMIZED);

	//UpdateWindow(handle);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	FinishFileLogging();

	return (INT)msg.wParam;
}
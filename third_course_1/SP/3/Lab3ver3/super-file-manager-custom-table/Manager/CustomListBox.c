#include "CustomListBox.h"
#include "ErrorHandling.h"

void ProceedButtonUp(ListBoxData* data) {
	INT oldVal = data->pos;
	data->pos--;
	if (data->pos <= 0) {
		data->pos = 0;
		EnableWindow(data->upButton, FALSE);
	}
	if (data->pos >= data->arr.size - data->capacity) {
		EnableWindow(data->downButton, FALSE);
	}
	else {
		EnableWindow(data->downButton, TRUE);
	}


	if (data->pos != oldVal)
		InvalidateRect(data->listHWND, NULL, FALSE);
}
void ProceedButtonDown(ListBoxData* data) {
	INT oldVal = data->pos;
	data->pos++;
	if (data->pos > data->arr.size - data->capacity) {
		data->pos--;
		EnableWindow(data->downButton, FALSE);
	}

	if (data->pos == 0) {
		EnableWindow(data->upButton, FALSE);
	}
	else
		EnableWindow(data->upButton, TRUE);

	if (data->pos != oldVal)
		InvalidateRect(data->listHWND, NULL, FALSE);
}

ListBoxData* IdentifyListBox(HWND hWnd) {
	return (ListBoxData*)GetWindowLongPtr(hWnd, 0);
}

HBITMAP hBitmap;
HDC memoryDC;

void CountCapacity(ListBoxData* data) {
	data->capacity = (data->height - data->headerHeight) / (data->itemHeight) + 1;
}

void SetStandartFonts(ListBoxData* data) {
	for (int i = 0;i < FONTS_COUNT;i++) {
		data->fonts[i] = NULL;
	}
}

void  SetElementAndHeaderHeight(ListBoxData* data) {
	data->headerHeight = LISTBOX_HEADER_MARGIN;
	data->itemHeight = LISTBOX_ITEM_MARGIN;
}
LRESULT CALLBACK ListBox_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	ListBoxData* data = IdentifyListBox(hWnd);
	switch (uMsg)
	{
	case WM_CREATE: {
		CREATESTRUCTA* windowParams = ((CREATESTRUCTA*)(lParam));
		data = windowParams->lpCreateParams;
		
		data->height = windowParams->cy;
		data->width = windowParams->cx;


		SetStandartFonts(data);
		SetElementAndHeaderHeight(data);
		CountCapacity(data);


		SetWindowLongPtr(hWnd, 0, (LONG_PTR)data);

		data->downButton = CreateWindowEx(0, L"BUTTON", L"\u2193",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, data->width - SCROLL_BUTTON_DOWN_WIDTH,
			data->headerHeight + SCROLL_BUTTON_DOWN_TOP + 1,
			SCROLL_BUTTON_DOWN_WIDTH, SCROLL_BUTTON_DOWN_HEIGHT, hWnd, (HMENU)IDC_BUTTON_DOWN, 0, 0);

		data->upButton = CreateWindowEx(0, L"BUTTON", L"\u2191",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, data->width - SCROLL_BUTTON_DOWN_WIDTH,
			data->headerHeight + SCROLL_BUTTON_UP_TOP + 1,
			SCROLL_BUTTON_UP_WIDTH, SCROLL_BUTTON_UP_HEIGHT, hWnd, (HMENU)IDC_BUTTON_UP, 0, 0);

		
		HDC hdc = GetDC(hWnd);
		hBitmap = CreateCompatibleBitmap(hdc, data->width, data->height);

		if (memoryDC != NULL) {
			DeleteDC(memoryDC);
		}
		memoryDC = CreateCompatibleDC(hdc);

		ReleaseDC(hWnd, hdc);

		SelectObject(memoryDC, hBitmap);

		DeleteObject(hBitmap);
		break;
	}
	//wParam - Items bg color lParam - Header bg color
	case WM_SET_ITEM_AND_HEADER_BG: {
		data->itemBgColor = (HBRUSH)wParam;
		data->headerBgColor = (HBRUSH)lParam;
		break;
	}
	case WM_CREATE_EDIT: {
			data->customWindow = CreateWindowEx(0, L"EDIT", L"Dir", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
				0, 0, data->width / 2, data->headerHeight,
				hWnd, (HMENU)IDC_EDIT, NULL, NULL);

			SendMessage(data->customWindow, EM_SETLIMITTEXT, (WPARAM)MAX_PATH - 1, 0);
			SendMessage(data->customWindow, WM_SETFONT, (WPARAM)data->fonts[CUSTOM_WINDOW], (LPARAM)TRUE);
			editWndProc = SetWindowLongPtrW(data->customWindow, GWLP_WNDPROC, (LONG_PTR)wParam);
			(*((HWND*)lParam)) = data->customWindow;
	
		break;
	}
	case WM_PAINT:
	{
		SelectObject(memoryDC, CreatePen(PS_SOLID, 1, rgbBORDER_LINE));
		SetBkMode(memoryDC, TRANSPARENT);

		int amountOfFiles = MyGetMin(data->arr.size - data->pos, data->capacity);

		SelectObject(memoryDC, GetStockObject(WHITE_BRUSH));

		//Filling all window with itemBgColor
		RECT rect = { 0,0,data->width,data->height };
		FillRect(memoryDC, &rect, data->itemBgColor);


		//Custom window drawing
		rect.bottom = data->headerHeight;
		rect.right = data->width / 2;

		SelectObject(memoryDC, data->fonts[CUSTOM_WINDOW]);

		if (data->customWindow == NULL) {

			int messageSize = (CharLength(data->arr.size) + 14);
			WCHAR* dirSizeMessage = LoggedMalloc(sizeof(WCHAR) * messageSize);
			if (dirSizeMessage == NULL) return 0;

			swprintf_s(dirSizeMessage, messageSize, L"Files found: %d", data->arr.size);
			FillRect(memoryDC, &rect, data->headerBgColor);
			DrawTextW(memoryDC, dirSizeMessage, messageSize, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			free(dirSizeMessage);
		}

		//Second column header
		rect.left = data->width / 2;
		rect.right = data->width;
		FillRect(memoryDC, &rect, data->headerBgColor);

		MoveToEx(memoryDC, 0, rect.bottom, NULL);
		LineTo(memoryDC, data->width, rect.bottom);

		rect.left += 3;
		SelectObject(memoryDC, data->fonts[HEADER]);
		FillRect(memoryDC, &rect, data->headerBgColor);
		DrawTextW(memoryDC, L"File size", 9, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);


		MoveToEx(memoryDC, data->width / 2, 0, NULL);
		LineTo(memoryDC, data->width / 2, data->height);

		rect.top = data->headerHeight + TEXT_UP_STEP;
		rect.bottom = rect.top + data->itemHeight;


		//Drawing files
		RECT startRect = { 2,data->headerHeight,data->width / 2, data->headerHeight + data->itemHeight };
		rect = startRect;
		if (data->arr.files != NULL)
		{
			SelectObject(memoryDC, data->fonts[NAME_COLUMN]);
			for (int i = 0; i < amountOfFiles; i++) {
				FileData* file = data->arr.files[data->pos + i];
				DrawTextW(memoryDC, file->fileName, file->lengthOfFileName,
					&rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
				rect.top = rect.bottom;
				rect.bottom += data->itemHeight;
			}

			rect = startRect;
			rect.left = rect.right + 1;
			rect.right = data->width;

			SelectObject(memoryDC, data->fonts[ATTR_COLUMN]);
			for (int i = 0; i < amountOfFiles; i++) {
				FileData* file = data->arr.files[data->pos + i];
				DrawTextW(memoryDC, file->fileSize, file->lengthOfFileSize,
					&rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

				rect.top = rect.bottom;
				rect.bottom += data->itemHeight;
			}
		}

		MoveToEx(memoryDC, 0, data->height - 1, NULL);
		LineTo(memoryDC, data->width, data->height - 1);


		MoveToEx(memoryDC, 0, 0, NULL);
		LineTo(memoryDC, 0, data->height);

		MoveToEx(memoryDC, data->width -1, 0, NULL);
		LineTo(memoryDC, data->width - 1, data->height);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		BitBlt(hdc, 0, 0, data->width, data->height, memoryDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_SET_FONTS: {
		LOGFONT* fonts = (LOGFONTA*)wParam;
		for (int i = 0;i < FONTS_COUNT;i++) {
			if (data->fonts[i] != NULL) {
				DeleteObject(data->fonts[i]);
			}
			HFONT font = CreateFontIndirectW(&fonts[i]);
			data->fonts[i] = font;
		}
		if (data->customWindow != NULL) {
			SendMessage(data->customWindow, WM_SETFONT, data->fonts[CUSTOM_WINDOW], NULL);
		}


		//Item count
		data->itemHeight = FontMax(fonts[NAME_COLUMN].lfHeight, fonts[ATTR_COLUMN].lfHeight);
		data->headerHeight = FontMax(fonts[CUSTOM_WINDOW].lfHeight, fonts[HEADER].lfHeight);
		//RECT rect = {0, 0, data->width / 2, data->headerHeight};
		SetWindowPos(data->customWindow, HWND_TOP, 0, 0, data->width / 2, data->headerHeight, 0);
		CountCapacity(data);
		InvalidateRect(data->listHWND, NULL, FALSE);
	}
	case WM_COMMAND: {


		if (LOWORD(wParam) == IDC_BUTTON_UP)
		{
			ProceedButtonUp(data);
		}
		else if (LOWORD(wParam) == IDC_BUTTON_DOWN)
		{
			ProceedButtonDown(data);
		}
		break;
	}
	case WM_MOUSEWHEEL: {

		INT step = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
		if (step > 0) {
			ProceedButtonUp(data);
		}
		else {
			ProceedButtonDown(data);
		}

		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CreateCustomListBoxClass(HINSTANCE hInstance) {
	WNDCLASSEX wlistbox;
	memset(&wlistbox, 0, sizeof(wlistbox));

	//Creating new listbox
	wlistbox.cbSize = sizeof(WNDCLASSEX);
	wlistbox.lpfnWndProc = ListBox_WindowProc;
	wlistbox.hInstance = hInstance;
	wlistbox.lpszClassName = CUSTOM_LIST_BOX_CLASSNAME;
	wlistbox.cbWndExtra = sizeof(ListBoxData*);
	if (!RegisterClassEx(&wlistbox)) {
		MessageBox(0, L"Unable to registrate window", L"Registration error!", MB_OK);
		return FALSE;
	}
	return TRUE;
}
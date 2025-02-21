#include "CustomTable.h"
#include "ErrorHandling.h"
#include "InputDialog.h"
TableData* IdentifyCustomTable(HWND hWnd) {
	return hWnd == NULL ? NULL : (TableData*)GetWindowLongPtr(hWnd, 0);
}

void UpdateScroll(TableData* data, int bar) {
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	if (bar == SB_HORZ) {
		si.nMax = data->prefixColumnsWidth[data->columnsAmount - 1] - data->width;
	}
	else if (bar == SB_VERT) {
		si.nMax = data->prefixRowsHeight[data->rowsAmount - 1] + data->headerHeight - data->height;
	}

	si.nPage = 2;
}


BOOL AppendColumn(HWND table, TableHeader header) {
	TableData* data = IdentifyCustomTable(table);
	return InsertColumn(table,data->columnsAmount,header);
}

BOOL InsertColumn(HWND table,int pos, TableHeader header) {
	//TODO: Add checks for realloc

	TableData* data = IdentifyCustomTable(table);

	if (pos > data->columnsAmount) return FALSE;

	int size = data->rowsAmount;

	int newColumnsAmount = data->columnsAmount + 1;

	TableElement newElem = { String,NULL };
	for (int i = 0;i < size;i++) {
		data->content[i] = realloc(data->content[i], newColumnsAmount * sizeof(TableElement));
		

		for (int j = newColumnsAmount - 1;j > pos;j--) {
			data->content[i][j] = data->content[i][j-1];
		}
		data->content[i][pos] = newElem;
	}


	data->headers = realloc(data->headers, newColumnsAmount * sizeof(TableHeader));
	data->prefixColumnsWidth = (INT*)realloc(data->prefixColumnsWidth, newColumnsAmount * sizeof(INT));
	
	for (int i = newColumnsAmount - 1;i > pos;i--) {
		data->headers[i] = data->headers[i-1];
		data->prefixColumnsWidth[i] = data->prefixColumnsWidth[i - 1] + TABLE_BASIC_COLUMN_WIDTH;
	}

	data->prefixColumnsWidth[pos] = (((pos - 1 >= 0) ? data->prefixColumnsWidth[pos-1] : 0)
		+ TABLE_BASIC_COLUMN_WIDTH);
	data->headers[pos] = header;



	//In the end to be sure that everything is alright
	data->columnsAmount++;
	UpdateScroll(data, SB_HORZ);
	InvalidateRect(data->tableHWND, NULL, TRUE);
	return TRUE;
}

BOOL AppendRow(HWND table, TableElement* elements) {
	//TODO: Add checks for realloc
	TableData* data = IdentifyCustomTable(table);
	return InsertRow(table,data->rowsAmount,elements);
}

BOOL AppendEmptyRow(HWND table) {
	TableData* data = IdentifyCustomTable(table);
	return AppendRow(table, calloc(data->columnsAmount, sizeof(TableElement)));
}

BOOL InsertRow(HWND table,int pos, TableElement* elements) {
	//TODO: Add checks for realloc
	TableData* data = IdentifyCustomTable(table);

	if (pos > data->rowsAmount) return FALSE;

	data->content = realloc(data->content, (data->rowsAmount + 1) * sizeof(TableElement*));
	data->prefixRowsHeight = realloc(data->prefixRowsHeight, (data->rowsAmount + 1) * sizeof(INT));

	for (int i = data->rowsAmount;i > pos;i--) {
		data->content[i] = data->content[i - 1];
		data->prefixRowsHeight[i] = data->prefixRowsHeight[i - 1] + TABLE_BASIC_ROW_HEIGHT;
	}

	data->prefixRowsHeight[pos] = (pos-1>=0) ? data->prefixRowsHeight[pos-1] : 0
		+ TABLE_BASIC_ROW_HEIGHT;

	data->content[pos] = elements;

	data->rowsAmount++;
	UpdateScroll(data, SB_VERT);
	InvalidateRect(data->tableHWND, NULL, TRUE);
	return TRUE;
}

BOOL InsertEmptyRow(HWND table, int pos) {
	TableData* data = IdentifyCustomTable(table);
	return InsertRow(table,pos, malloc(data->columnsAmount * sizeof(TableElement)));
}

BOOL InsertData(HWND table, int row, int column, TableElement value) {
	TableData* data = IdentifyCustomTable(table);

	if ((row < 0) || (row >= data->rowsAmount) || (column < 0) || (column >= data->columnsAmount))
		return FALSE;

	data->content[row][column] = value;
	InvalidateRect(data->tableHWND, NULL, TRUE);
}


SelectedObject IdentifyButtonPosition(TableData* data,LPARAM lParam) {
	SelectedObject ans;
	ans.x = GET_X_LPARAM(lParam);
	ans.y = GET_Y_LPARAM(lParam);
	ans.type = None;

	int xSearch = ans.x + data->shiftX;
	for (int i = 0; i < data->columnsAmount;i++) {
		if (data->prefixColumnsWidth[i] >= xSearch) {
			if (data->prefixColumnsWidth[i] - xSearch < 2)
				ans.type = VerticalLine;
			ans.column = i;
			break;
		}
	}

	//Header area
	if (ans.y <= data->headerHeight) {
		if (ans.type != VerticalLine) {
			if (ans.y == data->headerHeight)
				ans.type = HorizontalLine;
			else
				ans.type = Header;
		}
		ans.row = -1;
		return ans;
	}
	else
	{
	//Element area
		int ySearch = ans.y + data->shiftY - data->headerHeight;

		for (int i = 0;i < data->rowsAmount;i++) {
			if (data->prefixRowsHeight[i] >= ySearch) {
				if (ans.type != VerticalLine) {
					if (data->prefixRowsHeight[i] - ySearch < 2)
						ans.type = HorizontalLine;
					else
						ans.type = Element;
				}
				ans.row = i;
				return ans;
			}
		}
	}
	return ans;
	
}

void UpdateTableSizes(TableData* data,int width,int height, int headerHeight) { 
	if (data->isReady == FALSE || data == NULL || data->rowsAmount == 0 || data->columnsAmount==0)
		return;
	data->width = width;
	data->height = height;
	data->headerHeight = headerHeight;

	int rowSize = (height - data->headerHeight) / data->rowsAmount;
	int columnSize = width / data->columnsAmount;

	int rowsAmount = data->rowsAmount;
	int columnsAmount = data->columnsAmount;

	data->prefixRowsHeight[0] = rowSize + (height - data->headerHeight - rowSize * data->rowsAmount);
	for (int i = 1;i < rowsAmount;i++) {
		data->prefixRowsHeight[i] = data->prefixRowsHeight[i - 1] + rowSize;
	}

	data->prefixColumnsWidth[0] = columnSize + (width - columnSize * data->columnsAmount);
	for (int i = 1;i < columnsAmount;i++) {
		data->prefixColumnsWidth[i] = data->prefixColumnsWidth[i - 1] + columnSize;
	}
}
HWND CreateCustomTable(HWND parent, TCHAR* name, int left, int top, int width, int height,
	int rowsAmount, int columnsAmount, TableHeader* headers, int headersAmount, LOGFONT* font) {

	TableData* data = malloc(sizeof(TableData));
	if (data == NULL) return NULL;

	data->isReady = FALSE;
	data->rowsAmount = rowsAmount;
	data->columnsAmount = columnsAmount;
	data->font = CreateFontIndirectW(font);

	data->headerHeight = TABLE_BASIC_HEADER_HEIGHT;

	data->content = malloc(rowsAmount * sizeof(TableElement*));
	data->prefixRowsHeight = malloc(rowsAmount * sizeof(INT));

	data->content[0] = calloc(columnsAmount, sizeof(TableElement));
;

	for (int i = 1;i < rowsAmount;i++) {
		data->content[i] = calloc(columnsAmount,sizeof(TableElement));
	}


	data->headers = malloc(columnsAmount * sizeof(TableHeader));
	data->prefixColumnsWidth = malloc(columnsAmount * sizeof(INT));

	if (headersAmount > 0) {
		data->headers[0] = headers[0];
	}
	else {
		data->headers[0] = CreateTableHeader(L"");
	}

	for (int i = 1;i < columnsAmount;i++) {
		if (headersAmount > i) {
			data->headers[i] = headers[i];
		}
		else {
			data->headers[i] = CreateTableHeader(L"");
		}
	}

	//UpdateTableSizes(data, width, height, TABLE_BASIC_HEADER_HEIGHT);
	data->shiftX = 0;
	data->shiftY = 0;

	data->linesBrush = CreateSolidBrush(RGB(0, 0, 0));
	data->headerBrush = CreateSolidBrush(RGB(255, 255, 0));
	data->elementBrush = CreateSolidBrush(RGB(255, 255, 255));
	data->selectedElementBrush = CreateSolidBrush(RGB(255, 255, 255));

	SelectedObject obj = { None,0,0,0,0 };
	data->selectedObj = obj;

	UpdateScroll(data, SB_HORZ);
	UpdateScroll(data, SB_VERT);

	data->isReady = TRUE;


	HWND table = CreateWindowEx(0, CUSTOM_TABLE_CLASSNAME, name,
		WS_CHILD,
		left, top,
		width, height, parent, 0, 0, data);
	ShowWindow(table, SW_NORMAL);
	return table;
}

TableElement GetData(HWND table, int row, int column) {
	TableData* data = IdentifyCustomTable(table);
	return data->content[row][column];
}


INT IdentifyElementToShow(INT* array, INT size, INT value) {
	INT a = 0, b = size - 1;
	INT c = (a + b) / 2;
	while (a < b) {
		if (array[c] == value) return c;
		if (array[c] > value) {
			b = c;
		} else
		{
			a = c + 1;
		}
		c = (a + b) / 2;
	}
	return a;
}


void MouseClickMessageBox(HANDLE hWnd, TableData* data,  WCHAR* mb_text) {
	InvalidateRect(data->tableHWND, NULL, TRUE);

	if (data->selectedObj.type == Header || data->selectedObj.type == Element) {
		TCHAR* caption;
		INT rowIdx = data->selectedObj.row;
		INT colIdx = data->selectedObj.column;

		if (data->selectedObj.row == -1) {
			caption = HEADER_CLICK_TEXT;
		}
		else {
			caption = CELL_CLICK_TEXT;
		}

		int textSize = wcslen(mb_text) + 10;
		WCHAR* text = LoggedMalloc(sizeof(WCHAR) * textSize);

		swprintf_s(text, textSize, mb_text, rowIdx, colIdx);
		SYSTEMTIME time;
	
		GetLocalTime(&time);
		TCHAR* timeStr = LoggedMalloc(sizeof(WCHAR) * 255);
		swprintf_s(timeStr, 255, L"%d ms, %d s, %d m, %d h.", time.wMilliseconds, time.wSecond, time.wMinute, time.wHour)	;

		wcscat_s(timeStr,255, text);
		MessageBox(hWnd, timeStr, caption, MB_OK);
		free(timeStr);
	}
}

void CustomDrawRect(HDC dc, RECT* textRect) {
	MoveToEx(dc, textRect->left, textRect->top, NULL);
	LineTo(dc, textRect->left, textRect->bottom);
	LineTo(dc, textRect->right, textRect->bottom);
	LineTo(dc, textRect->right, textRect->top);
	LineTo(dc, textRect->left, textRect->top);
	//FrameRect(dc, &textRect, data->linesBrush);
}

LRESULT CALLBACK CustomTable_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	SCROLLINFO si;
	TableData* data = IdentifyCustomTable(hWnd);

	switch (uMsg)
	{
	case WM_CREATE: {
		CREATESTRUCTA* windowParams = ((CREATESTRUCTA*)(lParam));
		data = windowParams->lpCreateParams;

		if (data == NULL) {
			PostQuitMessage(0);
			return;
		}

		SetWindowLongPtr(hWnd, 0, (LONG_PTR)data);

		data->tableHWND = hWnd;
		data->height = windowParams->cy;
		data->width = windowParams->cx;

		HDC hdcWindow = GetDC(hWnd);
		HDC hdcBack = CreateCompatibleDC(hdcWindow);
		HBITMAP hbmBack = CreateCompatibleBitmap(hdcWindow, data->width, data->height);
		SaveDC(hdcBack);
		SelectObject(hdcBack, hbmBack);
		data->tableHDC = hdcBack;
		data->tableHBM = hbmBack;
		ReleaseDC(hWnd, hdcWindow);
		break;
	}
	case WM_PAINT : {
		HDC dc = data->tableHDC;
		SetBkMode(dc, TRANSPARENT);

		SelectObject(dc, data->font);

		//Drawing header
		RECT rect = { 0,0,data->width,data->height};
		FillRect(dc, &rect, data->elementBrush);

		INT rowNumber = IdentifyElementToShow(data->prefixRowsHeight, data->rowsAmount, data->shiftY);
		INT columnNumber = IdentifyElementToShow(data->prefixColumnsWidth, data->columnsAmount, data->shiftX);


		INT elementsHeight = data->height - data->headerHeight;
		INT posY;

		INT x = columnNumber;
		INT y = rowNumber;
		RECT textRect = { 0,data->headerHeight,0,data->headerHeight};
		if (rowNumber > 0) {
			textRect.bottom += data->prefixRowsHeight[rowNumber - 1] - data->shiftY;
		}
		else {
			textRect.bottom -= data->shiftY;
		}

		do {
			textRect.top = textRect.bottom;
			posY = data->prefixRowsHeight[y] - data->shiftY;

			textRect.bottom = posY + data->headerHeight;

			x = columnNumber;
			INT posX;

			if (columnNumber > 0) {
				textRect.right = data->prefixColumnsWidth[columnNumber - 1] - data->shiftX;
			}
			else
				textRect.right = -data->shiftX;
			do {
				textRect.left = textRect.right;
				posX = data->prefixColumnsWidth[x] - data->shiftX;
				textRect.right = posX;
				
				//FrameRect(dc, &textRect, data->linesBrush);
				CustomDrawRect(dc, &textRect);
				TCHAR* str = ToString(data->content[y][x]);
				DrawText(dc, str, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
				free(str);

				if (y == rowNumber) {
					RECT headerRect = { textRect.left,0,textRect.right,data->headerHeight};
					
					if (data->selectedObj.type == Header && data->selectedObj.row == -1 && data->selectedObj.column == x) {
						FillRect(dc, &headerRect, data->headerBrush);
					}
					else {
					    FillRect(dc, &headerRect, data->headerBrush);
					}
					//FrameRect(dc, &headerRect, data->linesBrush);
					CustomDrawRect(dc, &headerRect);

					DrawText(dc, data->headers[x].name, -1, &headerRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
				}

				if (++x >= data->columnsAmount) break;
			} while (posX < data->width);

			if (++y >= data->rowsAmount) break;


		} while (posY < elementsHeight);

		//FrameRect(dc, &rect, data->linesBrush);
		CustomDrawRect(dc, &rect);
		MoveToEx(dc, 0, data->height-1, NULL);
		HPEN p = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		HPEN oldPen = SelectObject(dc, p);
		LineTo(dc, data->width-1,data->height-1);
		LineTo(dc, data->width-1, 0);
		LineTo(dc, 0, 0);
		LineTo(dc, 0, data->height-1);
		
		DeleteObject(p);
		SelectObject(dc, oldPen);
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		BitBlt(ps.hdc, 0, 0, data->width, data->height, dc, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_RBUTTONDOWN: {
		data->selectedObj = IdentifyButtonPosition(data, lParam);
		data->timerRButton = SetTimer(data->tableHWND, (UINT_PTR)3, GetDoubleClickTime(), NULL);
		break;
	}
	case WM_LBUTTONDOWN: {
		data->selectedObj = IdentifyButtonPosition(data, lParam);
		data->timerLButton = SetTimer(data->tableHWND, (UINT_PTR)2, GetDoubleClickTime(), NULL);
		break;
	}
	case WM_LBUTTONUP: {
		break;
	}

	case WM_RBUTTONDBLCLK: {
		KillTimer(data->tableHWND, data->timerRButton);
		SendMessage(data->tableHWND, WM_CELL_DRBUTTON, NULL, NULL);
		break;
	}

	//Double-clicking the left mouse button actually generates a sequence of four messages: WM_LBUTTONDOWN, WM_LBUTTONUP, WM_LBUTTONDBLCLK, and WM_LBUTTONUP.
	case WM_LBUTTONDBLCLK: {
		KillTimer(data->tableHWND, data->timerLButton);
		SendMessage(data->tableHWND, WM_CELL_DLBUTTON, NULL, NULL);
		break;
	}
	case WM_MBUTTONDOWN: {
		data->selectedObj = IdentifyButtonPosition(data, lParam);
		SendMessage(data->tableHWND, WM_CELL_MBUTTON, NULL, NULL);
		break;
	}
	case WM_MOUSEMOVE: {
			INT x = GET_X_LPARAM(lParam);	
			INT y = GET_Y_LPARAM(lParam);
		if (wParam & MK_LBUTTON == MK_LBUTTON) {
			
			//Vertical line - horizontal change
			if (data->selectedObj.type == VerticalLine) {
				INT col = data->selectedObj.column;
				INT shift = x - data->selectedObj.x;
				INT size = data->columnsAmount;
				if (col==0 || data->prefixColumnsWidth[col - 1] < data->prefixColumnsWidth[col] + shift) {
					for (int i = col; i < size;i++) {
						data->prefixColumnsWidth[i] += shift;
					}
				}
				data->selectedObj.x += shift;
				UpdateScroll(data, SB_HORZ);
				InvalidateRect(data->tableHWND, NULL, TRUE);
			}
			else if (data->selectedObj.type == HorizontalLine) {
			//Horizontal line - vertical change
				INT row = data->selectedObj.row;
				INT shift = y - data->selectedObj.y;
				INT size = data->rowsAmount;
				//Header
				if (row == -1) {
					data->headerHeight += shift;
				}
				else if (y > data->headerHeight){
					if (row == 0 || data->prefixRowsHeight[row - 1] < data->prefixRowsHeight[row] + shift) {
						for (int i = row;i < size;i++) {
							data->prefixRowsHeight[i] += shift;
						}
					}
				}
				data->selectedObj.y += shift;
				UpdateScroll(data, SB_VERT);
				InvalidateRect(data->tableHWND, NULL, TRUE);
			}
		}
		else {
			SelectedObject obj = IdentifyButtonPosition(data, lParam);
			if (obj.type == VerticalLine) {
				HCURSOR cur = LoadCursor(NULL, IDC_SIZEWE);
				SetCursor(cur);
			}
			else if (obj.type == HorizontalLine) {
				HCURSOR cur = LoadCursor(NULL, IDC_SIZENS);
				SetCursor(cur);
			}
			else {
				HCURSOR cur = LoadCursor(NULL, IDC_ARROW);
				SetCursor(cur);
			}

		}
		break;
	}
	case WM_MOUSELEAVE: {
		HCURSOR cur = LoadCursor(NULL, IDC_ARROW);
		SetCursor(cur);
		ShowScrollBar(data->tableHWND, SB_VERT, FALSE);
		ShowScrollBar(data->tableHWND, SB_HORZ, FALSE);
		InvalidateRect(data->tableHWND, NULL, TRUE);
		break;
	}
	case WM_HSCROLL: {
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_HORZ, &si);
		

		switch (LOWORD(wParam))
		{
				// User clicked the left arrow.
			case SB_LINELEFT:
				si.nPos -= 1;
				break;

				// User clicked the right arrow.
			case SB_LINERIGHT:
				si.nPos += 1;
				break;

				// User clicked the scroll bar shaft left of the scroll box.
			case SB_PAGELEFT:
				si.nPos -= si.nPage;
				break;

				// User clicked the scroll bar shaft right of the scroll box.
			case SB_PAGERIGHT:
				si.nPos += si.nPage;
				break;

				// User dragged the scroll box.
			case SB_THUMBTRACK:
				si.nPos = si.nTrackPos;
				break;
		}
		data->shiftX = si.nPos;
		// Set the position and then retrieve it.  Due to adjustments
		// by Windows it may not be the same as the value set.
		si.fMask = SIF_POS;

		InvalidateRect(data->tableHWND, NULL, TRUE);
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
		break;
	}
	case WM_VSCROLL: {
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_VERT, &si);


		switch (LOWORD(wParam))
		{
		case SB_TOP:
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;
			// User clicked the left arrow.
		case SB_LINEUP:
			si.nPos -= 1;
			break;

			// User clicked the right arrow.
		case SB_LINEDOWN:
			si.nPos += 1;
			break;

			// User clicked the scroll bar shaft left of the scroll box.
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;

			// User clicked the scroll bar shaft right of the scroll box.
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;

			// User dragged the scroll box.
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;
		}
		data->shiftY = si.nPos;
		// Set the position and then retrieve it.  Due to adjustments
		// by Windows it may not be the same as the value set.
		si.fMask = SIF_POS;

		InvalidateRect(data->tableHWND, NULL, TRUE);
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		break;
	}
	case WM_TIMER: {
		if (wParam == data->timerLButton) {
			KillTimer(data->tableHWND, data->timerLButton);
			SendMessage(data->tableHWND, WM_CELL_LBUTTON,NULL,NULL);
		}
		else if (wParam == data->timerRButton) {
			KillTimer(data->tableHWND, data->timerRButton);
			SendMessage(data->tableHWND, WM_CELL_RBUTTON, NULL, NULL);
		}
		break;
	}
	case WM_MOUSEWHEEL: {

		INT step = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
		INT delta = -10;
		if (step < 0)
			delta = -delta;
		if ((wParam & MK_CONTROL) == MK_CONTROL) {
			data->shiftX += delta;
			if (data->shiftX < 0)
				data->shiftX = 0;
			int maxShift = data->prefixColumnsWidth[data->columnsAmount - 1] - data->width;
			if (maxShift <= 0)
				maxShift = 0;
			if ((data->shiftX >= maxShift) && (maxShift>=0)) {
				data->shiftX = maxShift;
			}
		}
		else {
			data->shiftY += delta;
			if (data->shiftY < 0)
				data->shiftY = 0;
			int maxShift = data->prefixRowsHeight[data->rowsAmount- 1] + data->headerHeight - data->height;
			if (maxShift <= 0)
				maxShift = 0;
			if ((data->shiftY >= maxShift) && (maxShift >= 0)) {
				data->shiftY = maxShift;
			}
		}
		InvalidateRect(data->tableHWND, NULL, TRUE);
		break;
	}
	case WM_DESTROY: {
		RestoreDC(data->tableHDC, -1);
		DeleteDC(data->tableHDC);
		//DeleteBitmap(data->tableHBM); // ?
		data->tableHDC = 0;

		DeleteObject(data->headerBrush);
		DeleteObject(data->elementBrush);
		DeleteObject(data->linesBrush);
		DeleteObject(data->selectedElementBrush);
		break;
	}
	case WM_INPUT_DATA: {/*
		if (wParam != NULL) {
			TableElement elem = CreateStringElement(wParam, TRUE);
			InsertData(data->tableHWND, data->selectedObj.row, data->selectedObj.column, elem);
		}
		break;*/
	}
	
	case WM_CELL_LBUTTON: {
		MouseClickMessageBox(hWnd, data, LEFT_SINGLE_CLICK_TEXT);
		break;
	}
	case WM_CELL_RBUTTON : {
		MouseClickMessageBox(hWnd, data, RIGHT_SINGLE_CLICK_TEXT);
		break;
	}
	case WM_CELL_DLBUTTON: {
		if (data->selectedObj.row != -1) {

			TableElement elem = data->content[data->selectedObj.row][data->selectedObj.column];
			MouseClickMessageBox(hWnd, data, LEFT_DOUBLE_CLICK_TEXT);
		}
		else {
			MouseClickMessageBox(hWnd, data, LEFT_DOUBLE_CLICK_TEXT);
		}
		break;
	}
	case WM_CELL_DRBUTTON: {
		MouseClickMessageBox(hWnd, data, RIGHT_DOUBLE_CLICK_TEXT);
		break;
	}
	case WM_CELL_MBUTTON: {
		MouseClickMessageBox(hWnd, data, MIDDLE_SINGLE_CLICK_TEXT);
		break;
	}
	case WM_SIZE: {
		int width = GET_X_LPARAM(lParam);
		int height = GET_Y_LPARAM(lParam);
		UpdateTableSizes(data, width, height, TABLE_BASIC_HEADER_HEIGHT);
		InvalidateRect(data->tableHWND, NULL, TRUE);
		break;
	}
}


	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


BOOL CreateCustomTableClass(HINSTANCE hInstance) {
	WNDCLASSEX wlistbox;
	memset(&wlistbox, 0, sizeof(wlistbox));

	//Creating new listbox
	wlistbox.cbSize = sizeof(WNDCLASSEX);
	wlistbox.lpfnWndProc = CustomTable_WindowProc;
	wlistbox.hInstance = hInstance;
	wlistbox.lpszClassName = CUSTOM_TABLE_CLASSNAME;
	wlistbox.cbWndExtra = sizeof(TableData*);
	wlistbox.style = CS_DBLCLKS;
	if (!RegisterClassEx(&wlistbox)) {
		MessageBox(0, L"Unable to registrate window", L"Registration error!", MB_OK);
		return FALSE;
	}
	return TRUE;
}
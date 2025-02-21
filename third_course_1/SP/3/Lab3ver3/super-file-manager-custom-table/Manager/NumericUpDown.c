#include "NumericUpDown.h"

NumericUpDownData* IdentifyNumericUpDownBox(HWND hWnd) {
	return (NumericUpDownData*)GetWindowLongPtr(hWnd, 0);
}

INT ValueToStrLength(INT a) {
	INT l = 0;
	do {
		l++;
		a /= 10;
	} while (a);
	return l;
}

void UpdateEdit(NumericUpDownData* data) {
	int length = ValueToStrLength(data->value);
	TCHAR* number = malloc((length + 1) * sizeof(TCHAR));
	if (number!=NULL)
		swprintf_s(number, length + 1, L"%d", data->value);
	BOOL res = (BOOL)SendMessage(data->editBoxWnd, WM_SETTEXT, 0, (LPARAM)number);
	free(number);
}

void ProceedEditText(HWND hWnd) {
	HWND parentWnd = GetWindowLongPtrA(hWnd, GWLP_HWNDPARENT);
	NumericUpDownData* data = IdentifyNumericUpDownBox(parentWnd);

	int length = ValueToStrLength(data->maxValue);
	TCHAR* text = malloc((length + 1) * sizeof(TCHAR));
	if (text != NULL) {
		GetWindowText(data->editBoxWnd, text, length + 1);
		data->value = _wtoi(text);
	}
	if (data->value > data->maxValue) {
		data->value = data->maxValue;
	}
	if (data->value < data->minValue) {
		data->value = data->minValue;
	}
	UpdateEdit(data);
}
LRESULT CALLBACK NUD_EditWindow_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case EN_KILLFOCUS: {
		//ProceedEditText(hWnd);
		break;
	}
	case WM_KEYDOWN: {
		if (wParam == VK_RETURN) {
			ProceedEditText(hWnd);
		}
		break;
	}
	}
	return NUDeditWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NumericUpDown_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	NumericUpDownData* data = IdentifyNumericUpDownBox(hWnd);
	switch (uMsg)
	{
	case WM_CREATE: {
		CREATESTRUCTA* windowParams = ((CREATESTRUCTA*)(lParam));

		data = windowParams->lpCreateParams;
		data->height = windowParams->cy;
		data->width = windowParams->cx;
		SetWindowLongPtr(hWnd, 0, (LONG_PTR)data);

		data->editBoxWnd = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER,
			NUD_MARGIN, NUD_MARGIN, NUD_EDIT_WIDTH(data->width), NUD_EDIT_HEIGHT(data->height),
			hWnd, (HMENU)IDC_NUD_EDIT, NULL, NULL);

		CreateWindowEx(0, L"BUTTON", L"\u2193",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, NUD_EDIT_WIDTH(data->width) + NUD_GAP + NUD_MARGIN,
			NUD_BUTTON_UP_HEIGHT(data->height) + NUD_GAP,
			NUD_BUTTON_DOWN_WIDTH(data->width),
			NUD_BUTTON_DOWN_HEIGHT(data->height), hWnd, (HMENU)IDC_NUD_BUTTON_DOWN, 0, 0);

		CreateWindowEx(0, L"BUTTON", L"\u2191",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, NUD_EDIT_WIDTH(data->width) + NUD_GAP + NUD_MARGIN,
			0,
			NUD_BUTTON_UP_WIDTH(data->width),
			NUD_BUTTON_UP_HEIGHT(data->height), hWnd, (HMENU)IDC_NUD_BUTTON_UP, 0, 0);


		SendMessage(data->editBoxWnd, EM_SETLIMITTEXT, (WPARAM)ValueToStrLength(data->maxValue), 0);
		NUDeditWindowProc = SetWindowLongPtrW(data->editBoxWnd, GWLP_WNDPROC, (LONG_PTR)NUD_EditWindow_WindowProc);
		UpdateEdit(data);
		break;
	}
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDC_NUD_BUTTON_UP)
		{
			if (data->value + data->step <= data->maxValue)
				data->value += data->step;
			UpdateEdit(data);
		}
		else if (LOWORD(wParam) == IDC_NUD_BUTTON_DOWN)
		{
			if (data->value - data->step >= data->minValue)
				data->value -= data->step;
			UpdateEdit(data);
		}
		break;
	}
	case WM_DESTROY: {
		break;
	}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
};
BOOL CreateNumericUpDownClass(HINSTANCE hInstance) {
	WNDCLASSEX wupdown;
	memset(&wupdown, 0, sizeof(wupdown));

	wupdown.cbSize = sizeof(WNDCLASSEX);
	wupdown.lpfnWndProc = NumericUpDown_WindowProc;
	wupdown.hInstance = hInstance;
	wupdown.lpszClassName = NUMERIC_UP_DOWN_CLASSNAME;
	wupdown.cbWndExtra = sizeof(NumericUpDownData*);

	if (!RegisterClassEx(&wupdown)) {
		MessageBox(0, L"Unable to registrate window", L"Registration error!", MB_OK);
		return FALSE;
	}
	return TRUE;
}
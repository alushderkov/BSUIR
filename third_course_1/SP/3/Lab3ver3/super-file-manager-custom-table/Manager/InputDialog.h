#pragma once

#define WS_INPUTDIALOGWINDOW WS_BORDER | WS_SYSMENU
#define INPUT_DIALOG_BOX_CLASSNAME L"InputDialogBox"

#define WM_INPUT_DATA (WM_USER + 10)
#define ID_INPUT_DATA (WM_USER + 11)


BOOL CreateCustomInputDialogClass(HINSTANCE hInstance);
void ShowInputDialog(HWND parent);
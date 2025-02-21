#include <Windows.h>
#include "InputDialog.h"

#define IDC_EDIT 5862
#define IDC_OK   5863
#define IDC_CANCEL 5864

LRESULT CALLBACK CustomInputWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        HWND parentWnd = GetWindowLongPtrA(hWnd, GWLP_HWNDPARENT);
        CreateWindowEx(0, L"EDIT", L"hello", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            10, 10, 200, 20, hWnd, (HMENU)IDC_EDIT, NULL, NULL);

        CreateWindowEx(0, L"BUTTON", L"OK", WS_CHILD | WS_VISIBLE,
            10, 40, 80, 25, hWnd, (HMENU)IDC_OK, NULL, NULL);

        CreateWindowEx(0, L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE,
            120, 40, 80, 25, hWnd, (HMENU)IDC_CANCEL, NULL, NULL);
        break;
    }

    case WM_COMMAND: {
        if (LOWORD(wParam) == IDC_OK) {
            // Handle OK action
            char text[256];
            GetDlgItemText(hWnd, IDC_EDIT, text, sizeof(text));
                      
            HWND parentWnd = GetWindowLongPtrA(hWnd, GWLP_HWNDPARENT);
            SendMessage(parentWnd, ID_INPUT_DATA, text, 0);
            //SetFonts(parentWnd);
            
            DestroyWindow(hWnd);
        }
        else if (LOWORD(wParam) == IDC_CANCEL) {
            DestroyWindow(hWnd);
        }
        break;
    }

    case WM_DESTROY: {
        HWND parentWnd = GetWindowLongPtrA(hWnd, GWLP_HWNDPARENT);
        EnableWindow(parentWnd, TRUE);
        SetFocus(parentWnd);
        SendMessage(parentWnd, ID_INPUT_DATA, NULL, 0);
        break;
    }

    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

void ShowInputDialog(HWND wnd) {
    //HWND hInputWnd = CreateWindowEx(0, L"STATIC", L"Input Window", WS_OVERLAPPEDWINDOW,
    //    CW_USEDEFAULT, CW_USEDEFAULT, 300, 120, parent, NULL, NULL, NULL);

    //ShowWindow(hInputWnd, SW_SHOW);
    ////UpdateWindow(hInputWnd);
    ////SetWindowLongPtr(hInputWnd, GWLP_WNDPROC, (LONG_PTR)InputWindowProc);
    HWND hInputWin;
    EnableWindow(wnd, FALSE);
    hInputWin = CreateWindowEx(WS_EX_APPWINDOW, INPUT_DIALOG_BOX_CLASSNAME, L"Enter data", WS_INPUTDIALOGWINDOW,
        650, 400, 230, 120, wnd, NULL, NULL, NULL);
    ShowWindow(hInputWin, SW_SHOW);
}
BOOL CreateCustomInputDialogClass(HINSTANCE hInstance) {
    WNDCLASSEX winputwin;
    memset(&winputwin, 0, sizeof(winputwin));

    //Creating new listbox
    winputwin.cbSize = sizeof(WNDCLASSEX);
    winputwin.lpfnWndProc = CustomInputWindowProc;
    winputwin.hInstance = hInstance;
    winputwin.lpszClassName = INPUT_DIALOG_BOX_CLASSNAME;
    //winputwin.cbWndExtra = sizeof(TableData*);
    winputwin.style = CS_DBLCLKS;
    if (!RegisterClassEx(&winputwin)) {
        MessageBox(0, L"Unable to registrate window", L"Registration error!", MB_OK);
        return FALSE;
    }
    return TRUE;
}
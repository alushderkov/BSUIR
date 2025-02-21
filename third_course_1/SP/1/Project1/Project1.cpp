#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <shlwapi.h> 

#pragma comment(lib, "Shlwapi.lib")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

std::vector<std::pair<std::wstring, std::wstring>> unsortedFiles;
std::vector<std::pair<std::wstring, std::wstring>> sortedFiles;
int currentScrollIndex = 0;
const int ROWS_VISIBLE = 39;
const int rowHeight = 30;
HWND  hwndCount;
WNDPROC oldEditProc;
HBRUSH hBrushWhite;

void ScanDirectory(const std::wstring& directory, std::vector<std::pair<std::wstring, std::wstring>>& files) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directory + L"\\*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        const std::wstring fileOrDir = findFileData.cFileName;
        if (fileOrDir != L"." && fileOrDir != L"..") {
            std::wstring fullPath = directory + L"\\" + fileOrDir;

            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                ScanDirectory(fullPath, files);
            }
            else {
                std::wstring fileName = fileOrDir;
                std::wstring fileNameWithoutExt = fileOrDir;
                PathRemoveExtension(&fileNameWithoutExt[0]);
                if (fileNameWithoutExt[0] == *L"") {
                    files.push_back({ fileName, fileName });
                }
                else {
                    files.push_back({ fileName, fileNameWithoutExt });
                }
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);
}

void QuickSort(std::vector<std::pair<std::wstring, std::wstring>>& files, int left, int right) {
    int i = left, j = right;
    auto pivot = files[(left + right) / 2].second;

    while (i <= j) {
        while (files[i].second < pivot) i++;
        while (files[j].second > pivot) j--;
        if (i <= j) {
            std::swap(files[i], files[j]);
            i++;
            j--;
        }
    }

    if (left < j) QuickSort(files, left, j);
    if (i < right) QuickSort(files, i, right);
}

void UpdateTable(HWND hwndTable, const std::vector<std::pair<std::wstring, std::wstring>>& files, int scrollIndex, int baseIdFirstColumn, int baseIdSecondColumn) {
    int counter = 0;
    const int maxLength = 45;
    for (int i = 0; i < ROWS_VISIBLE; ++i) {
        int fileIndex = scrollIndex + i;
        if (fileIndex < files.size()) {
            std::wstring tempStrFirst = files[fileIndex].first;
            std::wstring tempStrSecond = files[fileIndex].second;

            // Ограничиваем строки до 50 символов и добавляем многоточие
            if (tempStrFirst.length() > maxLength) {
                tempStrFirst = tempStrFirst.substr(0, maxLength) + L"...";
            }
            if (tempStrSecond.length() > maxLength) {
                tempStrSecond = tempStrSecond.substr(0, maxLength) + L"...";
            }

            if (fileIndex > 0 && files[fileIndex].first == files[fileIndex - 1].first) {
                counter++;
                std::wstring tempCounter = L"[" + std::to_wstring(counter) + L"]";

                // Обновляем вторую строку, если найдены одинаковые имена
                bool modified = false;
                for (int j = 0; j < tempStrSecond.length(); j++) {
                    if (tempStrSecond[j] == '\0' && tempStrSecond.length() - 1 != j) {
                        modified = true;
                        for (int k = 0; k < tempCounter.length(); k++) {
                            tempStrSecond[j + k] = tempCounter[k];
                        }
                        tempStrSecond[j + tempCounter.length()] = '\0';
                        break;
                    }
                }
                if (!modified) {
                    tempStrSecond += tempCounter;
                }

                tempStrFirst += tempCounter;
            }
            else {
                counter = 0;
            }

            // Устанавливаем текст в таблице
            SetWindowText(GetDlgItem(hwndTable, baseIdFirstColumn + i), tempStrFirst.c_str());
            SetWindowText(GetDlgItem(hwndTable, baseIdSecondColumn + i), tempStrSecond.c_str());
        }
        else {
            // Очищаем оставшиеся строки
            SetWindowText(GetDlgItem(hwndTable, baseIdFirstColumn + i), L"");
            SetWindowText(GetDlgItem(hwndTable, baseIdSecondColumn + i), L"");
        }
    }
}


void OnDirectorySelected(HWND hwnd) {
    wchar_t directoryPath[MAX_PATH];
    GetWindowText(GetDlgItem(hwnd, 101), directoryPath, MAX_PATH);

    unsortedFiles.clear();
    sortedFiles.clear();

    ScanDirectory(directoryPath, unsortedFiles);

    sortedFiles = unsortedFiles;

    QuickSort(sortedFiles, 0, sortedFiles.size() - 1);

    currentScrollIndex = 0;
    UpdateTable(hwnd, unsortedFiles, currentScrollIndex, 1000, 2000);
    UpdateTable(hwnd, sortedFiles, currentScrollIndex, 3000, 4000);

    wchar_t countText[50];
    swprintf(countText, 50, L"Total files: %d", unsortedFiles.size());
    SetWindowText(hwndCount, countText);
}

LRESULT CALLBACK subEditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            HWND parentHwnd = GetParent(hwnd);
            OnDirectorySelected(parentHwnd);
            return 0;
        }
    default:
        return CallWindowProc(oldEditProc, hwnd, msg, wParam, lParam);
    }
    return 0;
}

void ScrollUp(HWND hwnd) {
    if (currentScrollIndex > 0) {
        currentScrollIndex--;
        UpdateTable(hwnd, unsortedFiles, currentScrollIndex, 1000, 2000);
        UpdateTable(hwnd, sortedFiles, currentScrollIndex, 3000, 4000);
    }
}

void ScrollDown(HWND hwnd) {
    if (currentScrollIndex + ROWS_VISIBLE < unsortedFiles.size()) {
        currentScrollIndex++;
        UpdateTable(hwnd, unsortedFiles, currentScrollIndex, 1000, 2000);
        UpdateTable(hwnd, sortedFiles, currentScrollIndex, 3000, 4000);
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"File Scanner";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"File Scanner",
        WS_POPUP | WS_VISIBLE,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    ShowWindow(hwnd, SW_SHOWMAXIMIZED);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        hBrushWhite = CreateSolidBrush(RGB(255, 255, 255));
        HWND hInput = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 2, 7, 370, 20, hwnd, (HMENU)101, nullptr, nullptr);
        oldEditProc = (WNDPROC)SetWindowLongPtr(hInput, GWLP_WNDPROC, (LONG_PTR)subEditProc);

        hwndCount = CreateWindow(L"STATIC", L"Total files: 0", WS_VISIBLE | WS_CHILD, 10, 830, 200, 20, hwnd, nullptr, nullptr, nullptr);

        CreateWindow(L"BUTTON", L"Up", WS_VISIBLE | WS_CHILD, 687, 830, 80, 20, hwnd, (HMENU)103, nullptr, nullptr);
        CreateWindow(L"BUTTON", L"Down", WS_VISIBLE | WS_CHILD, 767, 830, 80, 20, hwnd, (HMENU)104, nullptr, nullptr);

        for (int i = 0; i < ROWS_VISIBLE; ++i) {
            CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 2, 32 + i * 20, 385, 20, hwnd, (HMENU)(1000 + i), nullptr, nullptr);
            CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 389, 32 + i * 20, 377, 20, hwnd, (HMENU)(2000 + i), nullptr, nullptr);
            CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 768, 32 + i * 20, 385, 20, hwnd, (HMENU)(3000 + i), nullptr, nullptr);
            CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 1155, 32 + i * 20, 377, 20, hwnd, (HMENU)(4000 + i), nullptr, nullptr);
        }
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == 103) {
            ScrollUp(hwnd);
        }
        else if (LOWORD(wParam) == 104) {
            ScrollDown(hwnd);
        }
        break;
    case WM_CTLCOLORSTATIC: {
        HDC hdcStatic = (HDC)wParam;
        SetBkMode(hdcStatic, TRANSPARENT);
        SetBkColor(hdcStatic, RGB(255, 255, 255));
        return (LRESULT)hBrushWhite;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1)); int xStart = 1, yStart = 0;
        int screenWidth = 1400;
        int colWidth = (screenWidth - 20) / 4;

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));

        TextOut(hdc, 387 + 5, yStart + 7, L"Original names", 14);
        TextOut(hdc, 767 + 5, yStart + 7, L"Sorted Files", 12);
        TextOut(hdc, 1154 + 5, yStart + 7, L"Sorted names", 16);

        MoveToEx(hdc, xStart, yStart + rowHeight, NULL);
        LineTo(hdc, screenWidth - 10, yStart + rowHeight);

        MoveToEx(hdc, 387, yStart, NULL);
        LineTo(hdc, 387, yStart + rowHeight * 26 + 40);
        MoveToEx(hdc, 767, yStart, NULL);
        LineTo(hdc, 767, yStart + rowHeight * 26 + 40);
        MoveToEx(hdc, 1154, yStart, NULL);
        LineTo(hdc, 1154, yStart + rowHeight * 26 + 40);
        MoveToEx(hdc, xStart, yStart, NULL);
        LineTo(hdc, xStart + 1534, yStart);
        MoveToEx(hdc, xStart + 1533, yStart, NULL);
        LineTo(hdc, xStart + 1533, yStart + rowHeight * 26 + 40);
        MoveToEx(hdc, xStart, yStart, NULL);
        LineTo(hdc, xStart, yStart + rowHeight * 26 + 40);

        MoveToEx(hdc, xStart, yStart + rowHeight, NULL);
        LineTo(hdc, 1534, yStart + rowHeight);
        MoveToEx(hdc, xStart, yStart + rowHeight * 26 + 40, NULL);
        LineTo(hdc, 1534, yStart + rowHeight * 26 + 40);
        EndPaint(hwnd, &ps);
    }
    break;

    case WM_DESTROY:
        DeleteObject(hBrushWhite);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
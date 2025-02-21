#include "stdafx.h"
#include <windows.h>
#include <string>
#include <stdio.h>
#include <commdlg.h>
#include <TlHelp32.h>
#include <stdint.h>
#include<Strsafe.h>
#include <memory>
#include <vector>
#include <algorithm>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

#define MAX_LOADSTRING 100
#define DIV 1048576
#define ID_CPU 2000
#define ID_MEM 2001
#define ID_MEM2 2003
#define ID_EDIT 2002
#define SIZE 400
#define ID_PROCCESSES 106
#define ID_PROCESS_LISTBOX 103

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING] = L"Course Work";  
TCHAR szWindowClass[MAX_LOADSTRING] = L"MyWindowClass"; 

WCHAR cpu_char[6] = { ' ' };
WCHAR mem_char[8] = { ' ' };
WCHAR buffer[8068] = { ' ' };
WCHAR temp[8068] = { ' ' };
double cpu = -1, memory = -1;
int count = 0, cpu_mas[SIZE] = { -1 }, mem_mas[SIZE] = { -1 };
struct ProcessInfo {
	DWORD processID;
	int threadCount;
	std::wstring processName;
};

FILETIME idleTime;
FILETIME kernelTime;
FILETIME userTime;
FILETIME last_idleTime;
FILETIME last_kernelTime;
FILETIME last_userTime;

static THREADS Thread;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI ThreadProc(LPVOID);
LRESULT CALLBACK ProcessListWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND hwndProcessList;
HWND hwndListBox;


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, NULL);  

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);  
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;  
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);  

	return RegisterClassEx(&wcex);
}
// Функция для сортировки по названию процесса
bool CompareProcesses(const ProcessInfo& a, const ProcessInfo& b) {
	return a.processName < b.processName;
}
void ShowProcessList(HWND hwndParent)
{
	// Создание нового окна для отображения процессов
	hwndProcessList = CreateWindowEx(0, L"STATIC", L"Process List", WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX | WS_VISIBLE,
		200, 200, 600, 510, hwndParent, NULL, GetModuleHandle(NULL), NULL);

	// Установка WndProc для окна с процессами
	SetWindowLongPtr(hwndProcessList, GWLP_WNDPROC, (LONG_PTR)ProcessListWndProc);

	INITCOMMONCONTROLSEX icex;          
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);
	// Создание ListBox для отображения процессов
	hwndListBox = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | WS_VISIBLE, 10, 10, 570, 400, hwndProcessList, (HMENU)ID_PROCESS_LISTBOX, GetModuleHandle(NULL), NULL);
	ListView_SetExtendedListViewStyle(hwndListBox, LVS_EX_FULLROWSELECT);
	if (hwndListBox == NULL) {
		DWORD error = GetLastError();
		wchar_t buffer[256];
		wsprintf(buffer, L"Ошибка при создании окна: %lu", error);
		MessageBox(hwndProcessList, buffer, L"Ошибка", MB_OK);
	}
	// Добавление колонок
	LVCOLUMN lvColumn;
	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

	lvColumn.pszText = const_cast<LPWSTR>(L"Process ID");
	lvColumn.cx = 198;
	ListView_InsertColumn(hwndListBox, 0, &lvColumn);

	lvColumn.pszText = const_cast<LPWSTR>(L"Threads");
	lvColumn.cx = 100;
	ListView_InsertColumn(hwndListBox, 1, &lvColumn);

	lvColumn.pszText = const_cast<LPWSTR>(L"Process Name");
	lvColumn.cx = 555-300;
	ListView_InsertColumn(hwndListBox, 2, &lvColumn);

	// Кнопка "Закрыть"
	HWND hwndCloseButton = CreateWindowEx(0, L"BUTTON", L"Terminate process", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 430, 140, 30, hwndProcessList, (HMENU)IDCANCEL, GetModuleHandle(NULL), NULL);

	HANDLE processHandle;
	PROCESSENTRY32 processes;
	WCHAR buffer[255];
	std::vector<ProcessInfo> processList;

	// Создание снапшота процессов
	processHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	processes.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(processHandle, &processes))
	{
		do
		{
			// Добавляем информацию о процессе в список
			ProcessInfo pInfo;
			pInfo.processID = processes.th32ProcessID;
			pInfo.threadCount = processes.cntThreads;
			pInfo.processName = processes.szExeFile;

			processList.push_back(pInfo);
		} while (Process32Next(processHandle, &processes));
	}
	CloseHandle(processHandle);
	// Сортировка процессов по имени
	std::sort(processList.begin(), processList.end(), CompareProcesses);

	// Очищаем ListView перед добавлением новых данных
	ListView_DeleteAllItems(hwndListBox);

	// Добавляем данные в ListView
	for (size_t i = 0; i < processList.size(); ++i) {
		LVITEM lvItem;
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = i;

		// Добавляем процесс ID в первую колонку
		lvItem.iSubItem = 0;
		swprintf_s(buffer, L"%d", processList[i].processID);
		lvItem.pszText = buffer;
		ListView_InsertItem(hwndListBox, &lvItem);

		// Добавляем количество потоков во вторую колонку
		lvItem.iSubItem = 1;
		swprintf_s(buffer, L"%d", processList[i].threadCount);
		lvItem.pszText = buffer;
		ListView_SetItem(hwndListBox, &lvItem);

		// Добавляем имя процесса в третью колонку
		lvItem.iSubItem = 2;
		lvItem.pszText = const_cast<LPWSTR>(processList[i].processName.c_str());
		ListView_SetItem(hwndListBox, &lvItem);
	}
}
bool TerminateProcessById(DWORD processId) {
	// Открываем процесс с правом завершения
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
	if (hProcess == NULL) {
		return false;
	}

	// Завершаем процесс
	BOOL result = TerminateProcess(hProcess, 0);
	if (result == 0) {
		CloseHandle(hProcess);
		return false;
	}

	// Закрываем дескриптор процесса
	CloseHandle(hProcess);
	return true;
}
void CloseSelectedProcess() {
	int selectedIndex = (int)SendMessage(hwndListBox, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
	if (selectedIndex == -1) {
		MessageBox(hwndListBox, L"Не выбран процесс.", L"Ошибка", MB_OK | MB_ICONERROR);
		return;
	}

	// Извлекаем текст из первой колонки (где хранится ID процесса)
	WCHAR buffer[255];
	LVITEM lvItem = {};
	lvItem.iItem = selectedIndex;
	lvItem.iSubItem = 0; // Первая колонка
	lvItem.mask = LVIF_TEXT;
	lvItem.pszText = buffer;
	lvItem.cchTextMax = sizeof(buffer) / sizeof(buffer[0]);

	if (!SendMessage(hwndListBox, LVM_GETITEMTEXT, (WPARAM)selectedIndex, (LPARAM)&lvItem)) {
		MessageBox(hwndListBox, L"Не удалось получить данные строки.", L"Ошибка", MB_OK | MB_ICONERROR);
		return;
	}

	// Преобразуем ID процесса из текста
	DWORD processId = 0;
	if (swscanf_s(buffer, L"%lu", &processId) != 1 || processId == 0) {
		MessageBox(hwndListBox, L"Не удалось извлечь ID процесса.", L"Ошибка", MB_OK | MB_ICONERROR);
		return;
	}

	// Завершаем процесс
	if (TerminateProcessById(processId)) {
		MessageBox(hwndListBox, L"Процесс успешно завершен.", L"Успех", MB_OK | MB_ICONINFORMATION);

		// Удаляем строку из ListView
		SendMessage(hwndListBox, LVM_DELETEITEM, (WPARAM)selectedIndex, 0);
	}
	else {
		MessageBox(hwndListBox, L"Не удалось завершить процесс.", L"Ошибка", MB_OK | MB_ICONERROR);
	}
}

LRESULT CALLBACK ProcessListWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)  // Если нажата кнопка "Закрыть"
		{
			CloseSelectedProcess();
		}
		break;

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance;

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX,
		300, 200, 740, 450, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	HWND edit_cpu = CreateWindow(L"static", L"CPU:", WS_CHILD | WS_VISIBLE | ES_CENTER,
		10, 110, 70, 20, hWnd, (HMENU)ID_CPU, hInstance, NULL);

	HWND edit_mem = CreateWindow(L"static", L"Memory:", WS_CHILD | WS_VISIBLE | ES_CENTER,
		10, 240, 70, 20, hWnd, (HMENU)ID_MEM, hInstance, NULL);

	HWND edit_mem2 = CreateWindow(L"static", L"", WS_CHILD | WS_VISIBLE | ES_CENTER,
		10, 260, 70, 20, hWnd, (HMENU)ID_MEM2, hInstance, NULL);

	HWND edit2 = CreateWindow(L"static", L"", WS_CHILD | WS_VISIBLE,
		110, 260, 400, 140, hWnd, (HMENU)ID_EDIT, hInstance, NULL);

	HWND btnShowProccess = CreateWindow(L"button", L"Processes", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		560, 260, 100, 30, hWnd, (HMENU)ID_PROCCESSES, hInstance, NULL);

	Thread.handleThread = NULL;
	Thread.ThreadFucntion = ThreadProc;
	Thread.handleDialog = hWnd;
	Thread.time = 0;
	Thread.handleThread = CreateThread(NULL, 0, Thread.ThreadFucntion, NULL,
		NULL, &Thread.threadId);

	for (int i = SIZE - 1; i >= 0; i--) {
		cpu_mas[i] = -1;
		mem_mas[i] = -1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
	LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	HPEN hpen, hpen_cpu_graph, hpen_mem_graph;
	RECT rect, rect_cpu, rect_cpu_graph, rect_mem, rect_mem_update,
		rect_mem_graph, rect_window;
	POINT Min;
	MINMAXINFO* pInfo;

	switch (message)
	{
	case WM_GETMINMAXINFO: //Getting message from Windows
		pInfo = (MINMAXINFO*)lParam;
		Min = { 650, 450 };
		pInfo->ptMinTrackSize = Min;
		return 0;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case 105:  // Обработка выхода
			DestroyWindow(hWnd);
			break;
		case ID_PROCCESSES: {
			ShowProcessList(hWnd);
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		GetClientRect(hWnd, &rect_window);
		hdc = BeginPaint(hWnd, &ps);

		SetRect(&rect, 10, 10, 80, 110);
		if (cpu_mas[0] >= 0)
		{
			SetRect(&rect_cpu, 13, 110 - cpu, 77, 110);
		}

		SetRect(&rect_cpu_graph, 100, 8, rect_window.right - 10, 110);
		SetRect(&rect_mem, 10, 140, 80, 240);
		SetRect(&rect_mem_update, 13, rect_mem.bottom - memory, 77,
			rect_mem.bottom);
		SetRect(&rect_mem_graph, 100, rect_mem.top - 2, rect_window.right - 10,
			rect_mem.bottom);

		FillRect(hdc, &rect, CreateSolidBrush(RGB(0, 0, 0)));
		FillRect(hdc, &rect_cpu, CreateSolidBrush(RGB(0, 255, 0)));
		FillRect(hdc, &rect_cpu_graph, CreateSolidBrush(RGB(0, 0, 0)));
		FillRect(hdc, &rect_mem, CreateSolidBrush(RGB(0, 0, 0)));
		FillRect(hdc, &rect_mem_update, CreateSolidBrush(RGB(0, 255, 0)));
		FillRect(hdc, &rect_mem_graph, CreateSolidBrush(RGB(0, 0, 0)));

		hpen = CreatePen(PS_SOLID, 0, RGB(0, 128, 64));
		SelectObject(hdc, hpen);

		//grid
		count += 3;

		for (int i = rect_window.right - 10; i > rect_cpu_graph.left; i -= 10)
		{
			if (i - count % 10 >= rect_cpu_graph.left)
			{
				MoveToEx(hdc, i - count % 10, 8, 0);
				LineTo(hdc, i - count % 10, 110);

				MoveToEx(hdc, i - count % 10, rect_mem.top - 2, 0);
				LineTo(hdc, i - count % 10, rect_mem.bottom);
			}

		}

		for (int i = 10; i < 110; i += 10)
		{
			MoveToEx(hdc, rect_cpu_graph.left, i, 0);
			LineTo(hdc, rect_window.right - 10, i);
			MoveToEx(hdc, rect_cpu_graph.left, i + 130, 0);
			LineTo(hdc, rect_window.right - 10, i + 130);
		}

		DeleteObject(hpen);

		//cpu_graph
		hpen_cpu_graph = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		SelectObject(hdc, hpen_cpu_graph);

		for (int i = 1, j = 10; i < SIZE; i++, j += 3)
		{
			if ((cpu_mas[i + 1] >= 0) &&
				((rect_window.right - j) > rect_cpu_graph.left))
			{
				MoveToEx(hdc, rect_window.right - j, 110 - cpu_mas[i], 0);
				LineTo(hdc, rect_window.right - j - 3,
					110 - cpu_mas[i + 1]);

			}
		}

		DeleteObject(hpen_cpu_graph);

		//memory_graph
		hpen_mem_graph = CreatePen(PS_SOLID, 2, RGB(50, 172, 225));
		SelectObject(hdc, hpen_mem_graph);

		for (int i = 0, j = 10; i < SIZE - 1; i++, j += 3)
		{
			if ((mem_mas[i + 1] >= 0) &&
				((rect_window.right - j) > rect_mem_graph.left))
			{
				MoveToEx(hdc, rect_window.right - j,
					rect_mem_graph.bottom - mem_mas[i], 0);
				LineTo(hdc, rect_window.right - j - 3,
					rect_mem_graph.bottom - mem_mas[i + 1]);
			}
		}

		DeleteObject(hpen_mem_graph);

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for process list box.
INT_PTR CALLBACK ProcessList(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG: {
		HWND hList = GetDlgItem(hDlg, 1000);  
		HANDLE processHandle;
		PROCESSENTRY32 processes;
		WCHAR buffer[255];

		// Создание снапшота процессов
		processHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		processes.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(processHandle, &processes))
		{
			do
			{
				// Формирование строки для отображения
				wsprintfW(buffer, L" ID [%05d] Threads [%03d] %30s",
					processes.th32ProcessID, processes.cntThreads,
					processes.szExeFile);

				// Добавление строки в список
				SendMessageW(hList, LB_ADDSTRING, 0, (WPARAM)buffer);
			} while (Process32Next(processHandle, &processes));
		}
		CloseHandle(processHandle);

		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
ULONGLONG SubtractTimes(const FILETIME& later, const FILETIME& earlier) {
	ULARGE_INTEGER a, b;
	a.LowPart = later.dwLowDateTime;
	a.HighPart = later.dwHighDateTime;

	b.LowPart = earlier.dwLowDateTime;
	b.HighPart = earlier.dwHighDateTime;

	return a.QuadPart - b.QuadPart;
}
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	THREADS* ti = (THREADS*)lpParam;
	GetSystemTimes(&last_idleTime, &last_kernelTime, &last_userTime);
	for (;;)
	{
		if (GetSystemTimes(&idleTime, &kernelTime, &userTime) != 0)
		{

			double usr = userTime.dwLowDateTime - last_userTime.dwLowDateTime;
			double ker = kernelTime.dwLowDateTime - last_kernelTime.dwLowDateTime;
			double idl = (idleTime.dwLowDateTime - last_idleTime.dwLowDateTime)*0.97;

			double sys = ker + usr;

			last_idleTime = idleTime;
			last_userTime = userTime;
			last_kernelTime = kernelTime;

			if (sys != 0) {
				cpu = (sys - idl) / sys * 100;

				for (int i = SIZE - 1; i > 0; i--) {
					cpu_mas[i] = cpu_mas[i - 1];
				}
				cpu_mas[0] = cpu;
			}


			if (cpu >= 0)
			{
				wsprintfW(cpu_char, L"%d %%", (int)cpu);
				SetDlgItemText(Thread.handleDialog, 2000, cpu_char); 
			}
		}

		// Получение состояния памяти
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);
		GlobalMemoryStatusEx(&statex);

		// Обновление памяти в UI
		wsprintfW(buffer, L"%ld %% - RAM uses\r\n", statex.dwMemoryLoad);
		memory = statex.dwMemoryLoad;
		for (int i = SIZE - 1; i > 0; i--) {
			mem_mas[i] = mem_mas[i - 1];
		}
		mem_mas[0] = memory;

		wsprintfW(mem_char, L"%d %%", (int)memory);
		SetDlgItemText(Thread.handleDialog, 2001, mem_char);  

		wsprintfW(temp, L"%ld - total physical memory (RAM)\r\n", statex.ullTotalPhys / DIV);
		StringCchCat(buffer, sizeof(temp), temp);

		wsprintfW(temp, L"%ld - free physical memory (RAM)\r\n", statex.ullAvailPhys / DIV);
		StringCchCat(buffer, sizeof(temp), temp);

		wsprintfW(mem_char, L"%d MB", (statex.ullTotalPhys - statex.ullAvailPhys) / DIV);
		SetDlgItemText(Thread.handleDialog, 2003, mem_char);  

		wsprintfW(temp, L"%ld - size of paging file\r\n", statex.ullTotalPageFile / DIV);
		StringCchCat(buffer, sizeof(temp), temp);
		wsprintfW(temp, L"%ld - free paging file\r\n", statex.ullAvailPageFile / DIV);
		StringCchCat(buffer, sizeof(temp), temp);
		wsprintfW(temp, L"%ld - total virtual memory\r\n", statex.ullTotalVirtual / DIV);
		StringCchCat(buffer, sizeof(temp), temp);
		wsprintfW(temp, L"%ld - free virtual memory.\r\n", statex.ullAvailVirtual / DIV);
		StringCchCat(buffer, sizeof(temp), temp);

		SetDlgItemText(Thread.handleDialog, 2002, buffer); 

		InvalidateRect(Thread.handleDialog, NULL, TRUE);
		Sleep(500);
	}
}

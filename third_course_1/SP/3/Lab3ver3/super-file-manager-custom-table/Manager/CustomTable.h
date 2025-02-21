#pragma once
#include <stdio.h>
#include <Windows.h>
#include "TableElement.h"
#include "TableHeader.h"

#define HEADER_CLICK_TEXT L"Header"
#define CELL_CLICK_TEXT L"Cell"
#define WM_CELL_LBUTTON (WM_USER + 12)
#define WM_CELL_DLBUTTON (WM_USER + 13)
#define WM_CELL_RBUTTON (WM_USER + 14)
#define WM_CELL_MBUTTON (WM_USER + 15)
#define WM_CELL_DRBUTTON (WM_USER + 16)
#define LEFT_SINGLE_CLICK_TEXT L"Target: () Custom event: WM_CELL_LBUTTON: (%d;%d)"
#define RIGHT_SINGLE_CLICK_TEXT L" Custom event: WM_CELL_RBUTTON: (%d;%d)"
#define LEFT_DOUBLE_CLICK_TEXT L"Custom event: WM_CELL_LDBUTTON: (%d;%d)"
#define MIDDLE_SINGLE_CLICK_TEXT L"Custom event: WM_CELL_MBUTTON: (%d;%d)"
#define RIGHT_DOUBLE_CLICK_TEXT L"Custom event: WM_CELL_RDBUTTON: (%d;%d)"

#define CUSTOM_TABLE_CLASSNAME L"CustomTable"

//#define TABLE_BASIC_ROW_HEIGHT (72)
//#define TABLE_BASIC_COLUMN_WIDTH (72)
#define TABLE_AMNT_OF_COLUMNS (10)
#define TABLE_AMNT_OF_ROWS (20)
#define TABLE_BASIC_HEADER_HEIGHT (60)
#define TABLE_BASIC_ROW_HEIGHT ((1540-TABLE_BASIC_HEADER_HEIGHT)/TABLE_AMNT_OF_ROWS)
#define TABLE_BASIC_COLUMN_WIDTH (1540/TABLE_AMNT_OF_COLUMNS)


#define TABLE_SEED_DATA_DIR_PATH (L"D:\\Git")


#define SCROLLBAR_WIDTH 17

#define TABLE_FONT_SIZE 24

#define TABLE_FONT_NAME L"Arial"

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

typedef enum SelectedObjectType {
	None,
	Header,
	Element,
	HorizontalLine,
	VerticalLine
} SelectedObjectType;

typedef struct {
	SelectedObjectType type;
	INT row;
	INT column;
	INT x;
	INT y;
} SelectedObject;

typedef struct {
	INT width;
	INT height;
	INT rowsAmount;
	INT columnsAmount;

	//Array of Object[][]
	TableElement** content;
	TableHeader* headers;
	HFONT font;

	INT* prefixColumnsWidth;
	INT* prefixRowsHeight;


	HWND tableHWND;
	HDC tableHDC;
	HBITMAP tableHBM;

	INT headerHeight;

	INT shiftX;
	INT shiftY;

	HPEN linesBrush;
	HBRUSH headerBrush;
	HBRUSH elementBrush;
	HBRUSH selectedElementBrush;

	UINT_PTR timerLButton;
	UINT_PTR timerRButton;

	SelectedObject selectedObj;

	BOOL isReady;
} TableData;


BOOL InsertData(HWND table, int row, int column, TableElement value);

BOOL InsertRow(HWND table, int pos, TableElement* elements);

BOOL AppendRow(HWND table, TableElement* elements);

BOOL InsertColumn(HWND table, int pos, TableHeader header);

BOOL AppendColumn(HWND table, TableHeader header);

HWND CreateCustomTable(HWND parent, TCHAR* name, int left, int top, int width, int height,
	int rowsAmount, int columnsAmount, TableHeader* headers, int headerAmount, LOGFONT* font);


BOOL CreateCustomTableClass(HINSTANCE hInstance);
BOOL InsertEmptyRow(HWND table, int pos);
BOOL AppendEmptyRow(HWND table);
TableElement GetData(HWND table, int row, int column);

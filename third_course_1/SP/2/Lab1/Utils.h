#pragma once
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <Windows.h>


#define MAX_TABLE_HEIGHT 32*26
#define BORDER_THICKNESS 1
#define DATA_FIELD_HEIGHT(headerFontSize) MAX_TABLE_HEIGHT-headerFontSize-7
#define CW1 384
#define CW2 384
#define FX_OFFS 0
#define SX_OFFS FX_OFFS+CW1+CW2
#define Y_OFFS 0

class Utils
{
private:
	static void DrawLines(const HDC& hdc, int hSize, int left, int& const top);
	static void FillTextData(const HDC& hdc, const HFONT& hFile, const HFONT& hCriterion, int fSize, int cSize, int start,
		std::vector<std::pair<std::wstring, std::wstring>>& const data, int xCol1, int xCol2, int startY);
public:
	static void DrawTable(const HDC& hdc, const HFONT& hHeader, const HFONT& hFile, const HFONT& hCriterion,
		int pageNum1, int pageNum2, int hSize, int fSize, int cSize,
		std::vector<std::pair<std::wstring, std::wstring>>& const data,
		std::vector<std::pair<std::wstring, std::wstring>>& const sortedData);
	static void GetFontTypeChoice(const HWND& combo, std::wstring& const fontType);
	static void FillComboBox(const HWND& combo, std::map<std::wstring, int>& const data);
	static void GetFontSizeChoice(const HWND& combo, int& const fontSize);
	static int GetMaxFontSize(int size1, int size2);
	static std::wstring GetFontName(wchar_t path[]);
};


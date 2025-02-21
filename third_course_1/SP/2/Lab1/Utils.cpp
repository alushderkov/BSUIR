#include "Utils.h"
void Utils::DrawLines(const HDC& hdc, int hSize, int left, int& const top)
{
	int startX = left;
	int startY = top;
	int endY = startY + MAX_TABLE_HEIGHT;

	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, startX, endY);
	MoveToEx(hdc, startX + CW1 - 1, startY, NULL);
	LineTo(hdc, startX + CW1 - 1, endY);
	MoveToEx(hdc, startX + CW1 + CW2 - 1, startY, NULL);
	LineTo(hdc, startX + CW1 + CW2 - 1, endY);
	MoveToEx(hdc, startX + 2 * CW1 + CW2 - 1, startY, NULL);
	LineTo(hdc, startX + 2 * CW1 + CW2 - 1, endY);
	MoveToEx(hdc, startX + 2 * CW1 + 2 * CW2+1, startY, NULL);
	LineTo(hdc, startX + 2 * CW1 + 2 * CW2+1, endY);

	MoveToEx(hdc, startX, endY, NULL);
	LineTo(hdc, startX + 2 * CW1 + 2 * CW2 + 1, endY);

	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, startX + 2 * CW1 + 2 * CW2 + 1, startY);

	std::wstring c1 = L"Полное имя файла";
	std::wstring c2 = L"Имя файла";

	int yOffs = (30 - hSize) / 2;
	TextOut(hdc, FX_OFFS + CW1 + 10, Y_OFFS + BORDER_THICKNESS + yOffs, c2.c_str(), c2.length());
	TextOut(hdc, SX_OFFS + 10, Y_OFFS + BORDER_THICKNESS + yOffs, c1.c_str(), c1.length());
	TextOut(hdc, SX_OFFS + CW1 + 10, Y_OFFS + BORDER_THICKNESS + yOffs, c2.c_str(), c2.length());

	top += 30;
	MoveToEx(hdc, startX, top, NULL);
	LineTo(hdc, startX + 2 * CW1 + 2 * CW2 + 1, top);
}

void TrimTextToFitWidth(HDC hdc, std::wstring& text, int columnWidth) {
	SIZE size;
	int textLength = text.length();
	bool isChange = false;
	// Проверяем ширину текста
	while (textLength > 0) {
		GetTextExtentPoint32(hdc, text.c_str(), textLength, &size);

		// Если ширина текста больше, чем ширина столбца, удаляем последний символ
		if (size.cx > columnWidth) {
			text[--textLength] = L'\0'; // Обрезаем строку на один символ
			isChange = true;
		}
		else {
			break; // Если текст помещается, выходим из цикла
		}
	}
	if(isChange){
		text[textLength - 1] = L'.';
		text[textLength - 2] = L'.';
		text[textLength - 3] = L'.';
		isChange = false;
	}
}

void Utils::FillTextData(const HDC& hdc, const HFONT& hFile, const HFONT& hCriterion, int start, int fSize, int cSize,
	std::vector<std::pair<std::wstring, std::wstring>>& const data, int xCol1, int xCol2, int startY)
{
	int counter = 0;
    
	int maxSize = GetMaxFontSize(fSize, cSize);
	int maxLength = 50;

	SIZE size1,size2;
		//maxLength = int(((xCol2-xCol1)/ fSize) *1.6+3);//24
		//maxLength = int(((xCol2 - xCol1) / cSize) *1.6+3);//24
	for (int i = start; i < (MAX_TABLE_HEIGHT / (maxSize+1)) + start; i++)
	{
		if (i < data.size())
		{
			std::wstring tempStrFirst = data[i].first;
			std::wstring tempStrSecond = data[i].second;

			SelectObject(hdc, hFile);
			SetBkMode(hdc, TRANSPARENT);
			TrimTextToFitWidth(hdc, tempStrFirst, CW1 - 15);

			SelectObject(hdc, hCriterion);
			SetBkMode(hdc, TRANSPARENT);
			TrimTextToFitWidth(hdc, tempStrSecond, CW1 - 15);

			if (i > 0 && data[i].first == data[i - 1].first) {
				counter++;
				std::wstring tempCounter = L"[" + std::to_wstring(counter) + L"]";

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

			size_t nullPos2 = tempStrSecond.find(L'\0'); 
			size_t nullPos1 = tempStrFirst.find(L'\0');

			if (nullPos2 != std::string::npos) {
				tempStrSecond = tempStrSecond.substr(0, nullPos2);
			}
			if (nullPos1 != std::string::npos) {
				tempStrFirst = tempStrFirst.substr(0, nullPos1);
			}

			///////////////
			SelectObject(hdc, hFile);
			SetBkMode(hdc, TRANSPARENT);

			int yCorrection = maxSize - fSize;
			TextOut(hdc, xCol1, startY + yCorrection, tempStrFirst.c_str(), tempStrFirst.length());

			SelectObject(hdc, hCriterion);
			SetBkMode(hdc, TRANSPARENT);
			yCorrection = maxSize - cSize;
			TextOut(hdc, xCol2, startY + yCorrection, tempStrSecond.c_str(), tempStrSecond.length());
		}
		//	MoveToEx(hdc, FX_OFFS, startY, NULL);
		//	LineTo(hdc, FX_OFFS + 2 * CW1 + 2 * CW2, startY);
		startY += maxSize;
	}
}
void Utils::DrawTable(const HDC& hdc, const HFONT& hHeader, const HFONT& hFile, const HFONT& hCriterion, int pageNum1,
	int pageNum2, int hSize, int fSize, int cSize, std::vector<std::pair<std::wstring, std::wstring>>& const data, std::vector<std::pair<std::wstring, std::wstring>>& const sortedData)
{
	int dataHeight = DATA_FIELD_HEIGHT(hSize);
	/*
	int startIndex1 = pageNum1* (MAX_TABLE_HEIGHT / GetMaxFontSize(fSize, cSize) - 1);
	int startIndex2 = pageNum2* (MAX_TABLE_HEIGHT / GetMaxFontSize(fSize, cSize) - 1);
	*/
	int startIndex1 = pageNum1;
	int startIndex2 = pageNum2;
	int yOff = 0;
	SelectObject(hdc, hHeader);
	SetBkMode(hdc, TRANSPARENT);
	DrawLines(hdc, hSize, FX_OFFS, yOff);
	FillTextData(hdc, hFile, hCriterion, startIndex1, fSize, cSize, data, FX_OFFS + 10, FX_OFFS + CW1 + 10, yOff + 5);
	FillTextData(hdc, hFile, hCriterion, startIndex2, fSize, cSize, sortedData, SX_OFFS + 10, SX_OFFS + CW1 + 10, yOff + 5);
}
void Utils::FillComboBox(const HWND& combo, std::map<std::wstring, int>& const data)
{
	std::vector<std::pair<std::wstring, int>> vec(data.begin(), data.end());
	std::sort(vec.begin(), vec.end(), [](const std::pair<std::wstring, int>& a, const std::pair<std::wstring, int>& b)
		{
			return a.second < b.second;
		});
	for (auto el : vec)
		SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)el.first.c_str());
}
void Utils::GetFontTypeChoice(const HWND& combo, std::wstring& const fontType)
{
	int i = SendMessage(combo, CB_GETCURSEL, 0, 0);
	if (i != CB_ERR)
	{
		wchar_t* buff = new wchar_t[SendMessage(combo, CB_GETLBTEXTLEN, i, 0) + 1];
		SendMessage(combo, CB_GETLBTEXT, i, (LPARAM)buff);
		fontType = std::wstring(buff);
		delete[] buff;
	}
}
void Utils::GetFontSizeChoice(const HWND& combo, int& const fontSize)
{
	int i = SendMessage(combo, CB_GETCURSEL, 0, 0);
	if (i != CB_ERR)
	{
		wchar_t* buff = new wchar_t[SendMessage(combo, CB_GETLBTEXTLEN, i, 0) + 1];
		SendMessage(combo, CB_GETLBTEXT, i, (LPARAM)buff);
		fontSize = std::stoi(std::wstring(buff));
		delete[] buff;
	}
}
int Utils::GetMaxFontSize(int size1, int size2)
{
	return (size1 > size2) ? size1 : size2;
}
std::wstring Utils::GetFontName(wchar_t path[])
{
	std::wstring res(path);
	int index1 = res.find_last_of(L'\\')+1;
	int index2 = res.find_first_of(L'.', index1);
	return res.substr(index1, index2-index1);
}
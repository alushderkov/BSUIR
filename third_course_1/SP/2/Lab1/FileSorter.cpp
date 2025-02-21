#include "FileSorter.h"
void FileSorter::ProcessDirectory(std::vector<std::pair<std::wstring, std::wstring>>& const container, const wchar_t path[])
{   
    std::wstring directory = std::wstring(path);
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directory + L"\\*").c_str(), &findFileData);
    
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        const std::wstring fileOrDir = findFileData.cFileName;
        if (fileOrDir != L"." && fileOrDir != L"..") {
            std::wstring fullPath = directory + L"\\" + fileOrDir;

            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

                ProcessDirectory(container, fullPath.c_str());
            }
            else {
                std::wstring fileName = fileOrDir;
                std::wstring fileNameWithoutExt = fileOrDir;
                PathRemoveExtension(&fileNameWithoutExt[0]);
                if (fileNameWithoutExt[0] == *L"") {
                    container.push_back(std::pair(fileName, fileName ));
                }
                else {
                    container.push_back(std::pair(fileName, fileNameWithoutExt ));
                }
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);
 
}

void FileSorter::Swap(std::pair<std::wstring, std::wstring>& const first, std::pair<std::wstring, std::wstring>& const scnd)
{
    std::pair<std::wstring, std::wstring> temp = first;
    first = scnd;
    scnd = temp;
}

int FileSorter::GetPartition(std::vector<std::pair<std::wstring, std::wstring>>& const data, int start, int end)
{
    std::pair<std::wstring, std::wstring> pivot = data[(start + end) / 2];  // Опорный элемент (середина массива)
    int i = start, j = end;
    while (i <= j)
    {
        // Сравниваем по названию файла (первый элемент пары)
        while (data[i].first < pivot.first)
            i++;
        while (data[j].first > pivot.first)
            j--;
        if (i >= j)
            break;
        Swap(data[i++], data[j--]);  // Меняем местами элементы, если они не на своих местах
    }
    return j;
}

void FileSorter::QSort(std::vector<std::pair<std::wstring, std::wstring>>& const data, int start, int end)
{
    if (start < end)
    {
        int q = GetPartition(data, start, end);  // Разбиение массива
        QSort(data, start, q);                  // Рекурсивная сортировка левой части
        QSort(data, q + 1, end);                // Рекурсивная сортировка правой части
    }
}

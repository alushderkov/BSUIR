#include "Sort.h"

std::pair<std::wstring, std::wstring> SplitFileNameAndExtension(const std::wstring& fileName) {
    size_t pos = fileName.find_last_of(L'.');
    if (pos != std::wstring::npos) {
        return { fileName.substr(0, pos), fileName.substr(pos) };
    }
    return { fileName, L"" };
}

std::wstring GenerateUniqueFileName(const std::wstring& baseName, std::unordered_map<std::wstring, int>& gNameCount)
{
    auto item = SplitFileNameAndExtension(baseName);
    std::wstring uniqueName = item.first + item.second;
    int count = gNameCount[baseName];

    if (count > 0) {
        uniqueName = item.first + L"[" + std::to_wstring(count) + L"]" + item.second;
    }

    gNameCount[baseName]++;

    return uniqueName;
}

std::wstring FormatFileTime(const FILETIME& fileTime) {
    SYSTEMTIME systemTime;
    FileTimeToSystemTime(&fileTime, &systemTime);

    std::wstringstream ss;
    ss << std::setfill(L'0') 
       << std::setw(3) << systemTime.wMilliseconds << L" "
       << std::setw(2) << systemTime.wSecond << L" "
       << std::setw(2) << systemTime.wMinute << L" "
       << std::setw(2) << systemTime.wHour << L" "
       << std::setw(2) << systemTime.wDay << L" "
       << std::setw(2) << systemTime.wMonth << L" "
       << systemTime.wYear;

    return ss.str();
}

void LoadFilesData(std::vector<FileData>& gFilesVector, const std::wstring directoryPath, std::unordered_map<std::wstring, int>& gNameCount)
{
    WIN32_FIND_DATA data;

    std::wstring searchPath = directoryPath + L"\\*";

    HANDLE hFind = FindFirstFile(searchPath.c_str(), &data);

    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (wcscmp(data.cFileName, L".") != 0 && wcscmp(data.cFileName, L"..") != 0) {
                const std::wstring newDirPath = directoryPath + L"\\" + data.cFileName;
                LoadFilesData(gFilesVector, newDirPath, gNameCount);
            }
        }
        else {
            FileData fileData;
            std::wstring baseName = data.cFileName;

            fileData.fileName = GenerateUniqueFileName(baseName, gNameCount);

            ULONGLONG fileSize = ((ULONGLONG)data.nFileSizeHigh << 32) + data.nFileSizeLow;
            fileData.fileSize = std::to_wstring(fileSize) + L" bytes";

            fileData.fileDate = FormatFileTime(data.ftCreationTime);

            gFilesVector.push_back(fileData);
        }
    } while (FindNextFile(hFind, &data) != 0);

    FindClose(hFind);
}

void Swap(std::vector<FileData>& gFilesVector, int i, int j)
{
    auto temp = gFilesVector[i];
    gFilesVector[i] = gFilesVector[j];
    gFilesVector[j] = temp;
}

int Partition(std::vector<FileData>& gFilesVector, int left, int right)
{
    auto pivot = gFilesVector[left].fileSize.c_str();
    int j = left;

    for (int i = left + 1; i <= right; i++)
    {
        if (wcscmp(gFilesVector[i].fileSize.c_str(), pivot) <= 0)
        {
            j++;
            Swap(gFilesVector, i, j);
        }
    }

    Swap(gFilesVector, left, j);
    return j;
}

void QSort(std::vector<FileData>& gFilesVector, int left, int right)
{
    while (left < right)
    {
        int pivotIndex = Partition(gFilesVector, left, right);
        if (pivotIndex - left <= right - pivotIndex)
        {
            QSort(gFilesVector, left, pivotIndex - 1);
            left = pivotIndex + 1;
        }
        else
        {
            QSort(gFilesVector, pivotIndex + 1, right);
            right = pivotIndex - 1;
        }
    }
}


bool Compare(const FileData& a, const FileData& b, int columnIndex) {
    switch (std::abs(columnIndex)) {
    case 1:  // Сортировка по имени файла
        if (columnIndex >= 0) {
            return a.fileName < b.fileName;
        }
        else {
            return a.fileName > b.fileName;
        }
    case 2:  // Сортировка по дате
        if (columnIndex >= 0) {
            return a.fileDate < b.fileDate;
        }
        else {
            return a.fileDate > b.fileDate;
        }
    case 3:  // Сортировка по размеру
        if (columnIndex >= 0) {
            return a.fileSize < b.fileSize;
        }
        else {
            return a.fileSize > b.fileSize;
        }
    default:
        return false;
    }
}



void Merge(std::vector<FileData>& data, int left, int mid, int right, int columnIndex) {
    std::vector<FileData> temp(right - left + 1); // Временный массив
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (Compare(data[i], data[j], columnIndex)) {
            temp[k++] = data[i++];
        }
        else {
            temp[k++] = data[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = data[i++];
    }

    while (j <= right) {
        temp[k++] = data[j++];
    }

    for (k = 0; k < temp.size(); ++k) {
        data[left + k] = temp[k];
    }
}

void MergeSort(std::vector<FileData>& data, int left, int right, int columnIndex) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        MergeSort(data, left, mid, columnIndex);
        MergeSort(data, mid + 1, right, columnIndex);

        Merge(data, left, mid, right, columnIndex);
    }
}
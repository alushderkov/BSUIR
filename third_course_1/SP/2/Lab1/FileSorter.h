#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <windows.h>
#include <algorithm>
#include <shlwapi.h> 

#pragma comment(lib, "Shlwapi.lib")
class FileSorter
{
private:
	static void Swap(std::pair<std::wstring, std::wstring>& const first, std::pair<std::wstring, std::wstring>& const scnd);
	static int GetPartition(std::vector<std::pair<std::wstring, std::wstring>>& const data, int start, int end);
public:
	static void ProcessDirectory(std::vector<std::pair<std::wstring, std::wstring>>& const container, const wchar_t path[]);
	static void QSort(std::vector<std::pair<std::wstring, std::wstring>>& const data, int start, int end);
};

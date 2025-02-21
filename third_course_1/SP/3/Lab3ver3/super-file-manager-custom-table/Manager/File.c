#include "File.h"
#include "ErrorHandling.h"

FileArray NullFileArray = {NULL, 0, 0};

BOOL IncreaseArrayCapacity(FileArray* array) {
	array->capacity = array->capacity * 2;
	FileData** oldptr = array->files;
	array->files = realloc(array->files, array->capacity * sizeof(FileData*));
	if (array->files != NULL) {
		return TRUE;
	}
	array->files = oldptr;
	return FALSE;
}

WCHAR* DeleteExtention(WCHAR* name) {
	WCHAR* pos = wcsrchr(name, L'.');
	if (pos != NULL)
		pos[0] = L'\0';
	return pos;
}

BOOL AddNewFile(FileArray* array, WIN32_FIND_DATA file) {
	FileData* newFileData = LoggedMalloc(sizeof(FileData));
	if (newFileData == NULL) return FALSE;
	
	int fileNameLength = wcslen(file.cFileName) + 1;
	WCHAR* fileName = LoggedMalloc(sizeof(WCHAR) * fileNameLength);
	if (fileName == NULL) {
		free(newFileData);
		return FALSE;
	}

	wcscpy_s(fileName, fileNameLength, file.cFileName);
	newFileData->fileName = fileName;
	newFileData->lengthOfFileName = fileNameLength;
	int Size = CharLength(file.nFileSizeLow) + 7;
	newFileData->lengthOfFileSize = Size;
	newFileData->fileSize = LoggedMalloc(Size * sizeof(WCHAR));

	if (newFileData->fileSize == NULL) {
		free(newFileData);
		return FALSE;
	}
	swprintf_s(newFileData->fileSize, Size + 1, L"%d bytes", file.nFileSizeLow);

	array->files[array->size++] = newFileData;
	if (array->size == array->capacity) {
		IncreaseArrayCapacity(array);
	}
	
	return TRUE;
}

BOOL ProceedDirectory(WCHAR* dirPath, INT dirSize, FileArray* fileArray) {

	BOOL memoryAvailable = TRUE;

	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	WCHAR* path = LoggedMalloc(sizeof(WCHAR) * (dirSize + 2));
	if (path == NULL) {
		return FALSE;
	}

	wcscpy_s(path, dirSize + 2, dirPath);
	wcscat_s(path, dirSize + 2, L"\\*");

	hFind = FindFirstFile(path, &ffd);

	if (hFind == INVALID_HANDLE_VALUE) { 
		int errorLen = wcslen(dirPath) + wcslen(ACCESS_DENIED_ERROR_STRING)+2;
		WCHAR* errorStr = LoggedMalloc(sizeof(WCHAR) * errorLen);
		wcscpy_s(errorStr, errorLen, ACCESS_DENIED_ERROR_STRING);
		wcscat_s(errorStr, errorLen, dirPath);
		wcscat_s(errorStr, errorLen, L"\n");

		LogError(errorStr);
		return TRUE;
	}
	else {
		do {

			if (!IS_DIR(ffd.dwFileAttributes)) {
				if (!AddNewFile(fileArray, ffd)) {
					memoryAvailable = FALSE;
					break;
				}
			}

			else if (wcscmp(ffd.cFileName, L".") && wcscmp(ffd.cFileName, L".."))
			{
				INT size = dirSize + wcslen(ffd.cFileName) + 4;
				WCHAR* newPath = LoggedMalloc(sizeof(WCHAR) * (size));
				if (newPath == NULL) {
					memoryAvailable = FALSE;
					break;
				} 

				wcscpy_s(newPath, size, dirPath);
				wcscat_s(newPath, size, L"\\");
				wcscat_s(newPath, size, ffd.cFileName);

				memoryAvailable = ProceedDirectory(newPath, size, fileArray);
				free(newPath);
				
				if (!memoryAvailable) break;

			}
		} while (FindNextFile(hFind, &ffd) != 0);
	}

	FindClose(hFind);
	free(path);
	return memoryAvailable;
}

INT CharLength(INT a) {
	INT l = 0;
	do {
		l++;
		a /= 10;
	} while (a);
	return l;
}
FileArray GetAllFiles(WCHAR* dirPath) {

	FileArray result;
	result.size = 0;
	result.capacity = ARRAY_STRATING_SIZE;

	result.files = LoggedMalloc(result.capacity * sizeof(FileData*));
	if (result.files == NULL)
	{
		return NullFileArray;
	}

	INT size = wcslen(dirPath) + 1;
	if (!ProceedDirectory(dirPath, size, &result)) {
		return NullFileArray;
	}

	return result;
}

void DestroyFileArray(FileArray arr) {
	if (arr.files != NULL) {
		for (int i = 0;i < arr.size;i++) {
			if (arr.files[i] != NULL) {
				if (arr.files[i]->fileName != NULL)
					free(arr.files[i]->fileName);
				free(arr.files[i]);
			}
		}
		free(arr.files);
	}
}
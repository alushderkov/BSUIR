#pragma once
#include <Windows.h>
#include <stdio.h>

#define ARRAY_STRATING_SIZE 128
#define IS_DIR(x) (x & FILE_ATTRIBUTE_DIRECTORY)
#define MyGetMax(a,b) ((a)>(b)?(a):(b))
#define MyGetMin(a,b) ((a)<(b)?(a):(b))

typedef struct {
	WCHAR* fileName;
	int lengthOfFileName;
	WCHAR* fileSize;
	int lengthOfFileSize;
} FileData;

typedef struct  {
	FileData** files;
	INT size;
	INT capacity;
} FileArray;

extern FileArray NullFileArray;

void DestroyFileArray(FileArray arr);

FileArray GetAllFiles(WCHAR* dirpath);
WCHAR* DeleteExtention(WCHAR* name);

INT CharLength(INT a);
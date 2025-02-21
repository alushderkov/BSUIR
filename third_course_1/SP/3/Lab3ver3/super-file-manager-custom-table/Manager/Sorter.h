#pragma once

#include <Windows.h>

int (*compare)(void*, void*);
void** CreateSortedArray(void** array, int length, int(*comporator)(void*, void*));
void Sort(void** array, int length, int(*comporator)(void*, void*));
void SortPart(void** array, int from, int to, int(*comporator)(void*, void*));



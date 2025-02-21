#pragma once

#include <stdio.h>
#include <Windows.h>


typedef struct {
	TCHAR* name;
} TableHeader;

TableHeader CreateTableHeader(TCHAR* string);
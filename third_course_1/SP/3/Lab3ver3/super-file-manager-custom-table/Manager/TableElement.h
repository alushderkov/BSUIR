#pragma once

#include <stdio.h>
#include <Windows.h>


typedef enum {
	String,
	Int,
	Float
} ElementType;

typedef struct {
	ElementType type;
	void* value;
	BOOL changeable;
} TableElement;

TCHAR* ToString(TableElement element);
TableElement CreateStringElement(TCHAR* string, BOOL changeable);

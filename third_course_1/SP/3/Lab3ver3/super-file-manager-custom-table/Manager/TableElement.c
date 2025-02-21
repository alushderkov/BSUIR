#include "TableElement.h"
#include "ErrorHandling.h"

TCHAR* ToString(TableElement element) {
	if (element.value == NULL) return NULL;
	switch (element.type) {
	case Int: {
		int val = *((int*)element.value);\
		TCHAR* str = malloc(12 * sizeof(TCHAR));
		_itow_s(val, str, 12, 10);
		return str;
		break;
	}
	case String: {
		TCHAR* val = (TCHAR*)element.value;
		int len = wcslen(val) + 1;
		TCHAR* str = malloc(len * sizeof(TCHAR));
		wcscpy_s(str, len, val);
		return str;
	}
	}
	return NULL;
}

TableElement CreateStringElement(TCHAR* string, BOOL changeable) {
	TableElement res;
	res.changeable = changeable;
	res.type = String;

	int stringLen = wcslen(string)+1;
	TCHAR* copiedString = LoggedMalloc(sizeof(TCHAR) * (stringLen));
	wcscpy_s(copiedString, stringLen, string);
	res.value = copiedString;
	

	return res;
}
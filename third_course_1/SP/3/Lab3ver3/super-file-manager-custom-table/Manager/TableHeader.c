#include "TableHeader.h"

TableHeader CreateTableHeader(TCHAR* string) {
	TableHeader res;
	res.name = string;
	return res;
}
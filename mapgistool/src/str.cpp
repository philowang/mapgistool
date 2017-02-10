#include "stdafx.h"
#include "str.h"

bool isstreq(const TCHAR* s, const TCHAR* t)
{
	return !strcmp(s, t);
}

bool havedesc(TCHAR* file)
{
	return (isstreq(file, "GPOINT") || \
		isstreq(file, "BOUNDARY") || \
		isstreq(file, "ROUTING") || \
		isstreq(file, "GROUTE"));
}
#include "stdafx.h"

#ifdef DLL_TIME
#define EXPORT_DLL __declspec(dllexport)
#else
#define EXPORT_DLL __declspec(dllimport)
#endif

extern EXPORT_DLL char * strptime (const char *buf, const char *format, struct tm *tm);


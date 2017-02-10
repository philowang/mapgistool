#include "stdafx.h"
#include "CMapgisArea.h"
#include "sortarea.h"

CMapgisArea::CMapgisArea(short area)
	:MapgisArea(area)
{

}

CMapgisArea::~CMapgisArea()
{

}

void CMapgisArea::sort(TCHAR* mainkey, ...)
{
	int n = 1;
	TCHAR* temp;
	va_list atts;
	va_start(atts, mainkey);
	for (temp = va_arg(atts, TCHAR*); temp != NULL; temp = va_arg(atts, TCHAR*))
		n++;
	va_end(atts);

	TCHAR** attptr = NULL;
	if ((attptr = (TCHAR**)malloc(sizeof(TCHAR*) * n)) == NULL)
	{
		fprintf(stderr, "malloc err\n");
		return;
	}

	TCHAR** ptraddr = NULL;
	ptraddr = attptr;

	*ptraddr++ = _strdup(mainkey);

	va_start(atts, mainkey);
	for (temp = va_arg(atts, TCHAR*); temp != NULL; temp = va_arg(atts, TCHAR*))
		*ptraddr++ = _strdup(temp);
	va_end(atts);

	sortarea(MapgisArea, n, attptr);

	freeArrayofPtr(attptr, n);
	free(attptr);
}

void CMapgisArea::updaterouting()
{
	CMapgisArea::sort("GEOPOINT", "R_CODE", NULL);
	CMapgisArea::updateid();
	writerouting(MapgisArea);
}

void CMapgisArea::updatecoord(TCHAR* x, TCHAR* y)
{
	writecord(MapgisArea, x, y);
}

void CMapgisArea::updateid()
{
	CATT_STRU* stru;
	char* att;
	long num;
	char areatype = _GetAreaType(MapgisArea);
	_GetRcdNum(MapgisArea, areatype, &num);
	for (long i = 1; i < num; i++)
	{
		_GetAtt(MapgisArea, areatype, i, &stru, &att);
		_SetField(att, stru, "ID", (char*) &i, sizeof(long));
		_WriteAtt(MapgisArea, areatype, i, stru, att);
	}
}
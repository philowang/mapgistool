#include "stdafx.h"
#include "sortarea.h"
#include "str.h"


void freeArrayofPtr(TCHAR** p, long num)
{
	for (long i = 0; i < num; i++)
		free(*p++);
}

void* memdup(void* src, long nbytes)
{
	void* buf = NULL;
	
	if ((buf = (void*)malloc(nbytes)) == NULL)
	{
		fprintf(stderr, "malloc err from memdup\n");
		return NULL;
	}

	memmove(buf, src, nbytes);
	return buf;
}

int attcmp(short area, int attc, TCHAR* attv[], long att1, long att2)
{
	CATT_STRU* stru;
	TCHAR* att;
	TCHAR sortkey1[40] = { 0 };
	TCHAR sortkey2[40] = { 0 };
	TCHAR fldbuf[10];

	for (int i = 0; i < attc; i++)
	{
		_GetAtt(area, _GetAreaType(area), att1, &stru, &att);
		_GetField(att, stru, attv[i], fldbuf, sizeof(fldbuf), NULL);
		strcat_s(sortkey1, fldbuf);

		_GetAtt(area, _GetAreaType(area), att2, &stru, &att);
		_GetField(att, stru, attv[i], fldbuf, sizeof(fldbuf), NULL);
		strcat_s(sortkey2, fldbuf);
	}

	return(strcmp(sortkey1, sortkey2));
}


long fldlen(CATT_STRU* stru)
{
	long sum = 0;
	for (long i = 0; i < stru->hd.numbfield; i++)
	{
		switch (stru->fldEntry[i].fieldtype)
		{
		case LONG_TYPE:
			sum += 5;
			break;
		case DOUBLE_TYPE:
			sum += 10;
			break;
		default: //STR_TYPE and other data type
			sum += stru->fldEntry[i].msk_leng;
			break;
		}
	}
	return sum;
}

void swapAtt(short area, long index1, long index2)
{
	CATT_STRU* stru1;
	CATT_STRU* stru2;
	TCHAR* att1;
	TCHAR* att2;

	_GetAtt(area, _GetAreaType(area), index1, &stru1, &att1);
	att1 = (TCHAR*)memdup(att1, fldlen(stru1) + ATTSIZE);
	_GetAtt(area, _GetAreaType(area), index2, &stru2, &att2);

	_WriteAtt(area, _GetAreaType(area), index1, stru2, att2);
	_WriteAtt(area, _GetAreaType(area), index2, stru1, att1);

	free(att1);
}

void swapPnt(short area, long pnt1, long pnt2)
{
	D_DOT pnt1xy, pnt2xy;
	TCHAR* pnt1dat;
	TCHAR* pnt2dat;
	PNT_INFO pnt1info, pnt2info;
	short pnt1len, pnt2len;

	_GetPnt(area, pnt1, &pnt1xy, &pnt1dat, &pnt1len, &pnt1info);
	pnt1dat = (TCHAR*)memdup(pnt1dat, sizeof(TCHAR) * pnt1len);
	_GetPnt(area, pnt2, &pnt2xy, &pnt2dat, &pnt2len, &pnt2info);

	_UpdatePnt(area, pnt1, &pnt2xy, pnt2dat, pnt2len, &pnt2info);
	_UpdatePnt(area, pnt2, &pnt1xy, pnt1dat, pnt1len, &pnt1info);

	free(pnt1dat);
	swapAtt(area, pnt1, pnt2);
}



void swapLin(short area, long lin1, long lin2)
{
	D_DOT* lxy1;
	D_DOT* lxy2;
	LIN_INFO linfo1, linfo2;
	long len1, len2;


	_GetLin(area, lin1, &lxy1, &len1, &linfo1);
	lxy1 = (D_DOT*)memdup(lxy1, len1 * sizeof(D_DOT));
	_GetLin(area, lin2, &lxy2, &len2, &linfo2);

	_UpdateLin(area, lin1, lxy2, len2, &linfo2);
	_UpdateLin(area, lin2, lxy1, len1, &linfo1);

	free(lxy1);

	swapAtt(area, lin1, lin2);

}

void swapReg(short area, long reg1, long reg2)
{
	long* regdata1;
	long* regdata2;
	long len1, len2;
	REG_INFO rinfo1, rinfo2;

	_GetReg(area, reg1, &regdata1, &len1, &rinfo1);
	regdata1 = (long*)memdup(regdata1, sizeof(long) * len1);
	_GetReg(area, reg2, &regdata2, &len2, &rinfo2);

	_UpdateReg(area, reg1, regdata2, len2, &rinfo2);
	_UpdateReg(area, reg2, regdata1, len1, &rinfo1);

	free(regdata1);
	swapAtt(area, reg1, reg2);
}

void swapentity(short area, long p, long q)
{
	switch (_GetAreaType(area))
	{
	case PNT:
		swapPnt(area, p, q);
		break;
	case LIN:
		swapLin(area, p, q);
		break;
	case REG:
		swapReg(area, p, q);
		break;
	default:
		fprintf(stderr, "currently not support this type of file\n");
		exit(1);
	}
}

void setidorderly(short area)
{
	CATT_STRU* stru;
	char* att;
	long num;
	char atype = _GetAreaType(area);

	_GetRcdNum(area, atype, &num);
	for (long i = 1; i < num; i++)
	{
		_GetAtt(area, atype, i, &stru, &att);
		_SetField(att, stru, "ID", (char*)&i, sizeof(long));
		_WriteAtt(area, atype, i, stru, att);
	}
}

void sortarea(short area, int attc, TCHAR* attv[])
{
	long num;

	_GetRcdNum(area, _GetAreaType(area), &num);
	for (long i = 0; i < num - 1; i++)
		for (long j = num - 1; j > i; j--)
			if (attcmp(area, attc, attv, j, j - 1) < 0)
			{
				swapentity(area, j, j - 1);
			}

	_SetPackFlag(area, 1);
}

void sortarea_varargs(short area, TCHAR* att1, ...)
{
	int n = 1;
	TCHAR* temp;
	va_list atts;
	va_start(atts, att1);
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

	*ptraddr++ = _strdup(att1);

	va_start(atts, att1);
	for (temp = va_arg(atts, TCHAR*); temp != NULL; temp = va_arg(atts, TCHAR*))
		*ptraddr++ = _strdup(temp);
	va_end(atts);

	sortarea(area, n, attptr);

	freeArrayofPtr(attptr, n);
	free(attptr);
}

double getrate(short area)
{
	MAP_PARA mpara;

	_GetMapParam(area, &mpara);
	return mpara.rate;
}

double callen(short area, long index)
{
	D_DOT* lxy;
	long len;
	LIN_INFO inf;
	static double rate = getrate(area);

	_GetLin(area, index, &lxy, &len, &inf);
	return _CalculateLength(lxy, len) * rate / 1000;
}

double calangle(short area, long index)
{
	D_DOT* lxy;
	long len;
	LIN_INFO inf;
	double angle;

	_GetLin(area, index, &lxy, &len, &inf);
	angle = _Cal_2PIAngle(&lxy[0], &lxy[len - 1]) * PITOANGLE;
	if (angle < 90)
	{
		return 90.0 - angle;
	}
	else
		return 450.0 - angle;
}

void writerouting(short area)
{
	double dist, sum, angle;
	CATT_STRU* stru;
	TCHAR* att;
	
	long num;
	TCHAR prevgpoint[10];
	TCHAR currgpoint[10];

	MAP_PARA para;
	para.rate;

	_GetRcdNum(area, _GetAreaType(area), &num);
	for (long i = 1; i < num; i++)
	{ 
		
		_GetAtt(area, _GetAreaType(area), i, &stru, &att);
		_GetField(att, stru, "GEOPOINT", currgpoint, sizeof(currgpoint), NULL);

		dist = callen(area, i);
		if (isstreq(prevgpoint, currgpoint))
		{
			sum += dist;
		}
		else
		{
			sum = dist;
		}
		strcpy_s(prevgpoint, currgpoint);

		angle = calangle(area, i);

		_SetField(att, stru, "DISTANCE", (TCHAR*)&dist, sizeof(double));
		_SetField(att, stru, "SUM_L", (TCHAR*)&sum, sizeof(double));
		_SetField(att, stru, "DIRECTION", (TCHAR*)&angle, sizeof(double));
		_WriteAtt(area, _GetAreaType(area), i, stru, att);
	}
}

void writecord(short area, TCHAR* x, TCHAR* y)
{
	CATT_STRU* stru;
	TCHAR* att;
	D_DOT pxy;
	PNT_INFO inf;
	long num;
	TCHAR* dat;
	short len;
	double xv, yv;
	static double rate = getrate(area);

	_GetRcdNum(area, _GetAreaType(area), &num);
	for (long i = 1; i < num; i++)
	{
		_GetPnt(area, i, &pxy, &dat, &len, &inf);
		xv = pxy.x * rate / 1000;
		yv = pxy.y * rate / 1000;
		_GetAtt(area, _GetAreaType(area), i, &stru, &att);
		_SetField(att, stru, x, (TCHAR*)&xv, sizeof(double));
		_SetField(att, stru, y, (TCHAR*)&yv, sizeof(double));
		_WriteAtt(area, _GetAreaType(area), i, stru, att);
	}
}
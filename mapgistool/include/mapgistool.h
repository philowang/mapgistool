#pragma once

enum mode {
	MODEERROR,
	CHPPARA,
	CHLPARA,
	ATT2PARA,
	ATT2NOTE,
	UPDATECOORD,
	UPDATEROUTING,
	DGSATTDB,
	COMPACT,
	GENEGPS,
	GENEELEV,
	SORT,
};

mode getmode(TCHAR * m);

void updaterouting(short area);

void updatecoord(short area, TCHAR * x, TCHAR * y);

void updatecoordLoop(TCHAR * x, TCHAR * y, int filec, TCHAR * filev[]);

void GetFieldCheck(TCHAR * att, CATT_STRU * stru, TCHAR * fldName, TCHAR * fldBuf, long bufLen, short * isBlank);

void chpparaLoop(short area, int argc, TCHAR * argv[]);

int chppara(short area, long index, TCHAR * param, TCHAR * num);

void chlparaLoop(short area, int argc, TCHAR * argv[]);

int chlpara(short area, long index, TCHAR * param, TCHAR * num);

int att2para(short area, int argc, TCHAR * argv[]);

void stringtoupper(TCHAR * s);

int att2note(AREA_HINST AHInst, short area, TCHAR* prjfile, int argc, TCHAR* argv[]);

void appendtoprj(TCHAR* prjfile, TCHAR* gisfile);

void TransPara(short area, double &x, double &y);

double arealength(short line);

int copystru(short from, short to);

D_3DOT getelev(short elev, short mzk, D_DOT* xy);

time_t strttime(char* t, const char* fmt);

double getz(short terl, char* fldName, D_DOT xy);

float getrand(float diff);

void touch(char*, char*);

int genegps(TCHAR * zfile, TCHAR * begin, TCHAR * end, TCHAR * fmt);

void sort(short area, int keyc, TCHAR * keyv[]);

void geneelev(TCHAR * zfile, short area, int fldc, TCHAR * fldv[]);

bool updateatt(short area, long i, TCHAR * fld, void * fldval);

char getfldtype(CATT_STRU * stru, TCHAR * fld);


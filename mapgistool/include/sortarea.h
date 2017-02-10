#pragma once
#define ATTSIZE 256
#define PITOANGLE 180/PI;

#define sortarea_va(...) sortarea_varargs(__VA_ARGS__, NULL);

void freeArrayofPtr(TCHAR ** p, long num);

void * memdup(void * src, long nbytes);

int attcmp(short area, int attc, TCHAR * attv[], long att1, long att2);

void swapAtt(short area, long index1, long index2);

void swapPnt(short area, long pnt1, long pnt2);

void swapLin(short area, long lin1, long lin2);

void swapReg(short area, long reg1, long reg2);

void swapentity(short area, long p, long q);

void setidorderly(short area);

void sortarea(short area, int attc, TCHAR * attv[]);

void sortarea_varargs(short area, TCHAR * att1, ...);

double getrate(short area);

double callen(short area, long index);

double calangle(short area, long index);

void writerouting(short area);

void writecord(short area, TCHAR* x, TCHAR* y);

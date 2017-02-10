
#define BUFSIZE 2024

#import <msxml6.dll>
#import <C:\\Program Files\\Common Files\\System\\ado\\msado15.dll>  \
    rename( "EOF", "AdoNSEOF" )


enum attdbway {
				MAPGIS2DB = 1,  //generate mode, from mapgis file to database
				DB2MAPGIS		//restore mode, from database to mapgis file
			  };

TCHAR* fldcat(CATT_STRU* stru);
TCHAR* fldncat(CATT_STRU* stru);
TCHAR* valcat(CATT_STRU* stru, TCHAR* att);
void getfldvalue(CATT_STRU* stru, TCHAR* att, TCHAR* fld, int len, TCHAR fldval[]);
int delchar(TCHAR s[], int ch);
int filename(TCHAR s[]);
TCHAR* file2buf(const TCHAR* file);
int str2file(const TCHAR* str, TCHAR* fname);
int strunix2dos(TCHAR* str);
TCHAR* strdos2unix(TCHAR* str);
TCHAR* replace_char(TCHAR* str, TCHAR find, TCHAR replace);
bool saverb(ADODB::_RecordsetPtr pRS, TCHAR* no, TCHAR* text, TCHAR* fname);
bool singleID(short area);
void setid(short area);

void getid(CATT_STRU * stru, TCHAR * att, TCHAR * file, TCHAR * id1, TCHAR * id2, int len, long * id);

bool putPNTpara2rcdset(short area, long index, ADODB::_RecordsetPtr pRS);
bool putLINpara2rcdset(short area, long index, ADODB::_RecordsetPtr pRS);
bool putpara2rcdset(short area, long index, ADODB::_RecordsetPtr pRS);
int putField2rcdset(short area, long index, CATT_STRU * stru, TCHAR * att, ADODB::_ConnectionPtr pConn, ADODB::_RecordsetPtr pRS, TCHAR * file);
int putDesc2rcdset(TCHAR* file, CATT_STRU* stru, TCHAR* att, ADODB::_RecordsetPtr pRS);
void printid(TCHAR* file, CATT_STRU* stru, TCHAR* att);
HRESULT msql(ADODB::_RecordsetPtr pRS, ADODB::_ConnectionPtr pConn, _bstr_t query);
bool isRcdExist(ADODB::_ConnectionPtr pConn, ADODB::_RecordsetPtr pRS, CATT_STRU * stru, TCHAR * att, TCHAR * tablename);

long AppendEntity(short area, void * psa, long lngsize);

long AppendPnt(short area, void * psa, long lngsize);

long AppendLin(short area, void * psa, long lngsize);

bool PopulateAtt(short area, long index, ADODB::_RecordsetPtr pRS);

void ChangeFieldType(TCHAR * dbfile, int mapgisc, TCHAR * mapgisv[]);
int dgsattdb(attdbway way, TCHAR * dbfile, int mapgisc, TCHAR * mapgisv[]);
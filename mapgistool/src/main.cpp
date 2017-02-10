// main.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "mapgistool.h"
#include "help.h"
#include "sortarea.h"
#include "str.h"
#include "dgsattdb.h"
#include "CMapgisArea.h"

TCHAR* cwd = NULL;		//current working directory, because of the add file to project mode
TCHAR* dbfile = NULL;
using namespace std;
int main(int argc, TCHAR* argv[])
{
	int c;
	int errflg = 0;
	TCHAR* gisfile = NULL;
	TCHAR* prjfile = NULL;
	TCHAR* x = NULL;
	TCHAR* y = NULL;
	TCHAR* zfile = NULL;	//terl file name
	TCHAR* begin = NULL;	//start time
	TCHAR* end = NULL;	//end time
	
	extern TCHAR *optarg;
	extern int optind, optopt;

	mode m;
	attdbway way;
	bool IsCwdGiven = false;

	while ((c = getopt(argc, argv, _T(":grd:m:b:f:p:x:y:z:s:e:"))) != -1) {
		switch (c) {

		case 'g':		//generate mode, from mapgis file to database
			if (way == DB2MAPGIS) errflg++;
			else way = MAPGIS2DB;
			break;
		case 'r':		//restore mode, from database to mapgis file
			if (way == MAPGIS2DB) errflg++;
			else way = DB2MAPGIS;
			break;
		case 'd':		//working directory(mapgis project directory)
			cwd = optarg;
			IsCwdGiven = true;
			break;
		case 'm':		//mode
			m = getmode(optarg);
			break;
		case 'b':		//database file name
			dbfile = optarg;
			break;
		case 'f':		//mapgis file name
			gisfile = optarg;
			break;
		case 'p':		//mapgis project file name
			prjfile = optarg;
			break;
		case 'x':
			x = optarg;
			break;
		case 'y':
			y = optarg;
			break;
		case 'z':
			zfile = optarg;
			break;
		case 's':
			begin = optarg;
			break;
		case 'e':
			end = optarg;
			break;
		case ':':       /* -d or -t without operand */
			fprintf(stderr, _T("Option -%c requires an operand\n"), optopt);
			errflg++;
			break;
		case '?':
			fprintf(stderr, _T("Unrecognized option: -%c\n"), optopt);
			errflg++;
		}
	}
	
	if (errflg) {
		fprintf(stderr, "must specify either -g or -r option\n");
		exit(1);
	}


	if (!IsCwdGiven)
	{
		fprintf(stderr, _T("Must set CWD, before proceding, exit...\n"));
		exit(1);
	}

	if (!(SetCurrentDirectory(cwd)))
	{
		fprintf(stderr, _T("%s:the directory is invalid, exit...\n"), cwd);
		exit(1);
	}


	AREA_HINST AHInst;
	short area;
	AHInst = _InitWorkArea(NULL);


	if (gisfile)
	{
		if ((area = _OpenFileArea(AHInst, gisfile)) < 1)
		{
			printf(_T("WARNING: 文件%s不存在或不能打开\n"), gisfile);
			return -1;
		}
	}

	switch (m)
	{
	//change pnt paras
	case CHPPARA:
		chpparaLoop(area, argc - optind, &argv[optind]);
		break;

	//change line paras
	case CHLPARA:
		chlparaLoop(area, argc - optind, &argv[optind]);
		break;

	//change paras based on attributes
	case ATT2PARA:
		att2para(area, argc - optind, &argv[optind]);
		break;

	//generate note files bassed on attributes
	case ATT2NOTE:
		att2note(AHInst, area, prjfile, argc - optind, &argv[optind]);
		break;
	//generate gps.wt file
	case GENEGPS:
		genegps(zfile, begin, end, _T("%Y-%m-%d/%H:%M:%S"));
		break;
	case GENEELEV:
		geneelev(zfile, area, argc - optind, &argv[optind]);
		break;
	//write Pnt xx, yy, zz value
	//write Lin distance, direction, etc. 
	case UPDATECOORD:
		updatecoordLoop(x, y, argc - optind, &argv[optind]);
		break;
	case UPDATEROUTING:
		updaterouting(area);
		break;
	case COMPACT:
		ChangeFieldType(dbfile, argc - optind, &argv[optind]);
		break;
	case DGSATTDB:
		dgsattdb(way, dbfile, argc - optind, &argv[optind]);
		break;
	case SORT:
		sort(area, argc - optind, &argv[optind]);
		break;
	default:
		fprintf(stderr, "not support this mode\n");
		exit(1);
	}

	_CloseAllArea(AHInst);
	_FreeWorkArea(AHInst);

	

	return 0;
}


//-m chppara -f GPTNOTE.WT he 2.5 wi 2.5
//-m att2note -d c:\l4821 -p L4821.mpj -f gpoint.wt GPTNOTE.WT GEOPOINT
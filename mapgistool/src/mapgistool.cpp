// mapgistool.cpp : Defines the misc functions for the console application.

#include "stdafx.h"
#include "mapgistool.h"
#include "help.h"
#include "CMapgisArea.h"
#include "str.h"
#include "gps.h"
#include "sortarea.h"

mode getmode(TCHAR* m)
{
	if (isstreq(m, "chppara"))
		return CHPPARA;
	else if (isstreq(m, "chlpara"))
		return CHLPARA;
	else if (isstreq(m, "att2para"))
		return ATT2PARA;
	else if (isstreq(m, "att2note"))
		return ATT2NOTE;
	else if (isstreq(m, "compact"))
		return COMPACT;
	else if (isstreq(m, "dgsattdb"))
		return DGSATTDB;
	else if (isstreq(m, "updatecoord"))
		return UPDATECOORD;
	else if (isstreq(m, "updaterouting"))
		return UPDATEROUTING;
	else if (isstreq(m, "genegps"))
		return GENEGPS;
	else if (isstreq(m, "geneelev"))
		return GENEELEV;
	else if (isstreq(m, "sort"))
		return SORT;
	else
		return MODEERROR;
}

void updaterouting(short area)
{
	
	if (_IsAreaEmpty(area) || _GetAreaType(area) != LIN)
		return;

	CMapgisArea CArea = CMapgisArea(area);
	CArea.updaterouting();
	
	if (_GetChangeFlag(area))
	{
		TCHAR* FileName = strrchr(_GetAreaFileName(area), '\\');
		if (_SaveFile(area))
			printf("成功更新路线文件距离属性: %s\n", &FileName[1]);
	}
}

void updatecoord(short area, TCHAR* x = _T("XX"), TCHAR* y = _T("YY"))
{
	CMapgisArea CArea = CMapgisArea(area);
	CArea.updatecoord(x, y);
}

void updatecoordLoop(TCHAR* x, TCHAR* y, int filec, TCHAR* filev[])
{
	if (filec < 1)
	{
		updatecoordLoopHelp();
		exit(1);
	}

	AREA_HINST ah;
	short area;

	ah = _InitWorkArea(NULL);
	TCHAR* xx = (x ? x : _T("XX"));
	TCHAR* yy = (y ? y : _T("YY"));

	for (int i = 0; i < filec; i++)
	{
		if ((area = _OpenFileArea(ah, filev[i])) < 1 || _GetAreaType(area) != PNT)
		{
			fprintf(stderr, "failed to update %s coord\n", filev[i]);
			continue;
		}
		if (!_IsAreaEmpty(area))
		{
			updatecoord(area, xx, yy);
		}
		
		if (_SaveFile(area))
			printf("成功更新文件XX,YY坐标: %s\n", filev[i]);

		_CloseArea(area);
	}
	
	_FreeWorkArea(ah);
}


void GetFieldCheck(TCHAR* att, CATT_STRU* stru, TCHAR* fldName, TCHAR* fldBuf, long bufLen, short* isBlank)
{
	if (!_GetField(att, stru, fldName, fldBuf, bufLen, isBlank))
	{
		fprintf(stderr, "get field %s failed!\n", fldName);
		exit(1);
	}
}

void chpparaLoop(short area, int argc, TCHAR* argv[])
{
	if (argc < 2 || argc % 2)	//'argc % 2' checks for pairs of arguments 
	{
		chpparaHelp();
		exit(1);
	}
	if (_GetAreaType(area) != PNT)
	{
		fprintf(stderr, "file:%s isn't point type\n", _GetAreaFileName(area));
		exit(1);
	}
	if (_IsAreaEmpty(area))
		return;

	long j, pntN;
	_GetPntNum(area, &j, &pntN);
	for (j = 1; j < pntN; j++)
		for (int i = 0; i < argc; i += 2)
			if (chppara(area, j, argv[i], argv[i + 1]) != 1)
				printf("WARNING: 未能修改参数：%s,ID:%ld\n", argv[i], j);

	if (_GetChangeFlag(area))
	{
		TCHAR* FileName = strrchr(_GetAreaFileName(area), '\\');
		if (_SaveFile(area))
			printf("成功修改文件参数: %s\n", &FileName[1]);
	}
}

int chppara(short area, long index, TCHAR *param, TCHAR *num)
{
	D_DOT xy;
	TCHAR *dat;
	short len;
	PNT_INFO info;

	if (_GetPnt(area, index, &xy, &dat, &len, &info) <= 0)
		return -1;

	if (strncmp("type", param, 2) == 0)	//点类型
	{
		info.type = '0' + atoi(num);
	}
	else if (strncmp("ovprnt", param, 2) == 0)	//覆盖方式
	{
	}
	else if (strncmp("infoDx", param, 6) == 0)	//	[new, 点参数偏移dx]
	{
	}
	else if (strncmp("infoDy", param, 6) == 0)	//	[new, 点参数偏移dy]
	{
	}
	else if (strncmp("iclr", param, 2) == 0)		//颜色[short-->long]
	{
		info.iclr = atol(num);
	}
	else if (strncmp("linNo", param, 2) == 0)	//点所属的线号
	{
		info.linNo = atol(num);
	}
	else if (strncmp("layer", param, 2) == 0)	//图层
	{
		info.layer = (short)atoi(num);
	}
	else {
		switch (info.type)
		{
		case PNT_NOTE:	//处理字符串

			if (strncmp("height", param, 2) == 0) {		//高度
				info.info.ch.height = atof(num);
				break;
			}
			else if (strncmp("width", param, 2) == 0) {	//宽度
				info.info.ch.width = atof(num);
				break;
			}
			else if (strncmp("space", param, 2) == 0) {	//间隔
				info.info.ch.space = atof(num);
				break;
			}
			else if (strncmp("angle", param, 2) == 0) {	//字符串角度，以360度制为单位表示
				info.info.ch.angle = atof(num);
				break;
			}
			else if (strncmp("ifnt", param, 2) == 0) {		//中文字体
				info.info.ch.ifnt = (short)atoi(num);
				break;
			}
			else if (strncmp("chnt", param, 2) == 0) {		//西文字体
				info.info.ch.chnt = (short)atoi(num);
				break;
			}
			else if (strncmp("ifnx", param, 2) == 0) {		//字形
				info.info.ch.ifnx = (TCHAR)atoi(num);
				break;
			}
			else if (strncmp("hvpl", param, 2) == 0) {		//水平或垂直排列0=水平排列 1=垂直排列
				info.info.ch.hvpl = (TCHAR)atoi(num);
				break;
			}
			else {
				printf("WARNING: 不支持%s类型参数的修改\n", param);
				break;
			}

		case PNT_SUB:	//子图
			if (strncmp("subno", param, 2) == 0) {		//子图号
				info.info.sub.subno = atol(num);
				break;
			}
			else if (strncmp("height", param, 2) == 0) {		//高度
				info.info.sub.height = atof(num);
				break;
			}
			else if (strncmp("width", param, 2) == 0) {	//宽度
				info.info.sub.width = atof(num);
				break;
			}
			else if (strncmp("angle", param, 2) == 0) {	//角度
				info.info.sub.angle = atof(num);
				break;
			}
			else if (strncmp("penw", param, 2) == 0) {		//线宽
				info.info.sub.penw = atof(num);
				break;
			}
			else {
				printf("WARNING: 不支持%s类型参数的修改\n", param);
				break;
			}

		case PNT_CIR:	//圆
			break;
		case PNT_ARC:	//圆弧
			break;
		case PNT_IMAGE:	//图象
			break;
		case PNT_TEXT:	//文本
			break;
		default:
			printf("WARNING: 不支持该类型参数的修改\n");
			return -1;
		}
	}

	_UpdatePntInfo(area, index, &info);
	return 1;
}

void chlparaLoop(short area, int argc, TCHAR* argv[])
{
	if (argc < 2 || argc % 2)	//'argc % 2' checks for pairs of arguments 
	{
		chlparaHelp();
		exit(1);
	}
	if (_GetAreaType(area) != LIN)
	{
		fprintf(stderr, "file:%s isn't line type\n", _GetAreaFileName(area));
		exit(1);
	}
	if (_IsAreaEmpty(area))
		return;

	long j, linN;
	_GetLinNum(area, &j, &linN);
	for (j = 1; j < linN; j++)
		for (int i = 0; i < argc; i += 2)
			if (chlpara(area, j, argv[i], argv[i + 1]) != 1)
				printf("WARNING: 未能修改参数：%s,ID:%ld\n", argv[i], j);

	if (_GetChangeFlag(area))
	{	
		TCHAR* FileName = strrchr(_GetAreaFileName(area), '\\');
		if (_SaveFile(area))
			printf("成功修改文件参数: %s\n", &FileName[1]);
	}
}

int chlpara(short area, long index, TCHAR *param, TCHAR *num)
{
	D_DOT *lxy;
	long len;
	LIN_INFO info;

	if (_GetLin(area, index, &lxy, &len, &info) <= 0)
		return -1;

	if (strncmp("ltp", param, 2) == 0)	//线型号
	{
		info.ltp = (short)atoi(num);
	}
	else if (strncmp("fltp", param, 2) == 0)	//辅助线型号
	{
		info.fltp = '0' + atoi(num);
	}
	else if (strncmp("ovprnt", param, 6) == 0)	//覆盖方式	
	{
		info.ovprnt = '0' + atoi(num);
	}
	else if (strncmp("lclr", param, 2) == 0)		//线颜色号[short-->long]
	{
		info.lclr = atol(num);
	}
	else if (strncmp("lw", param, 2) == 0)	//线宽
	{
		info.lw = atof(num);
	}
	else if (strncmp("lclass", param, 2) == 0)	//线种类
	{
		info.lclass = '0' + atoi(num);
	}
	else if (strncmp("xscale", param, 2) == 0)	//x系数
	{
		info.xscale = atof(num);
	}
	else if (strncmp("yscale", param, 2) == 0)	//y系数
	{
		info.yscale = atof(num);
	}
	else if (strncmp("fclr", param, 2) == 0)		//辅助色
	{
		info.fclr = atol(num);
	}
	else if (strncmp("layer", param, 2) == 0)	//图层
	{
		info.layer = (short)atoi(num);
	}
	else {
		printf("不支持该类型参数的修改\n");
		return -1;
	}

	_UpdateLinInfo(area, index, &info);
	return 1;
}

int att2para(short area, int argc, TCHAR* argv[])
{

	if (argc < 4 || argc % 2)
	{
		att2paraHelp();
		exit(1);
	}

	if (_IsAreaEmpty(area))
		return 0;

	CATT_STRU *stru = NULL;
	TCHAR *att = NULL;
	long j, linN;
	TCHAR fldBuf[20];

	//printf("即将修改的文件是： %s\n", argv[1]);
	if (_GetAreaType(area) == PNT)
		_GetPntNum(area, &j, &linN);
	else if (_GetAreaType(area) == LIN)
		_GetLinNum(area, &j, &linN);
	else
	{
		fprintf(stderr, "currently not support this type of file:%s", _GetAreaFileName(area));
		exit(1);
	}

	for (j = 1; j < linN; j++)
	{
		static TCHAR areatype = _GetAreaType(area);

		_GetAtt(area, areatype, j, &stru, &att);
		_GetField(att, stru, argv[0], fldBuf, sizeof(fldBuf), NULL);

		if (strcmp(fldBuf, argv[1]) == 0)
		{
			for (int i = 2; i < argc; i += 2)
				if (_GetAreaType(area) == PNT)
				{
					if (chppara(area, j, argv[i], argv[i + 1]) != 1)
						printf("WARNING: 未能修改参数：%s,ID:%ld\n", _GetAreaFileName(area), j);
				}
				else if (_GetAreaType(area) == LIN) {
					if (chlpara(area, j, argv[i], argv[i + 1]) != 1)
						printf("WARNING: 未能修改参数：%s,ID:%ld\n", _GetAreaFileName(area), j);
				}
				else
					printf("WARNING: 尚不支持对%s文件参数修改！\n", _GetAreaFileName(area));
		}
		else
			continue;
	}


	if (_GetChangeFlag(area))
	{
		TCHAR* FileName = strrchr(_GetAreaFileName(area), '\\');
		if (_SaveFile(area))
			printf("根据属性 [%s=%s], 成功修改文件参数：%s\n", argv[0], argv[1], &FileName[1]);
	}

	return 1;
}

void stringtoupper(TCHAR* s)
{
	TCHAR c;
	int p = 0;
	TCHAR* addr = _strdup(s);

	while ((c = *addr++) != '\0')
	{
		printf("%c\n", c);
		s[p++] = toupper(c);
	}
	s[p] = '\0';
}

//area indicates the mapgis file's area from which the note file generates
//argv[0] indicates the note file name
//argv[1], ... indicates the att fields
int att2note(AREA_HINST AHInst, short area, TCHAR* prjfile, int argc, TCHAR* argv[])
{
	if (argc < 2)
	{
		att2noteHelp();
		exit(1);
	}

	if (_IsAreaEmpty(area))	//无图元,不需要生成标注.
		return 0;

	long pn;
	short aiNote, t = 1;
	D_DOT xy;
	int i, k = 1;
	TCHAR *att = NULL;
	TCHAR *attNote = NULL;
	CATT_STRU *stru = NULL;			//原有属性结构
	CATT_STRU *struNote = NULL;
	CFIELD_HEAD new_fld[2];
	TCHAR fldBuf[12];
	TCHAR note[20];
	PNT_INFO info;
	TCHAR s[12], temp[12];
	double dip;
	
	aiNote = _OpenArea(AHInst, PNT);	//用于存放标注的工作区.
	_CopyMapParam(area, aiNote);
	strcpy(s, argv[0]);
	//stringtoupper(argv[0]);

	TCHAR areatype = _GetAreaType(area);

	memset(&info, 0, sizeof(PNT_INFO));
	info.type = PNT_NOTE;
	info.info.ch.ifnt = 1;
	info.iclr = 1;
	info.info.ch.height = 2;
	info.info.ch.width = 2;

	//新增MAPCODE字段,因原有文件中的字段长度与要求的不同.
	//是否存在直接编辑字段的方法?
	lstrcpy(new_fld[0].fieldname, "ROUTECODE");
	new_fld[0].fieldtype = STR_TYPE;
	new_fld[0].msk_leng = 5;
	new_fld[0].edit_enable = 1;
	lstrcpy(new_fld[1].fieldname, "MAPCODE");
	new_fld[1].fieldtype = STR_TYPE;
	new_fld[1].msk_leng = 30;
	new_fld[1].edit_enable = 1;
	_AppendStruFld(aiNote, areatype, 2, new_fld);

	long j, linN;
	_GetPntNum(area, &j, &linN);


	for (j = 1; j < linN; j++)
	{
		//determine the positions of notes
		if (_GetPntPos(area, j, &xy) <= 0)
		{
			printf("文件%s的第%ld个图元未生成标注！\n", argv[1], j);
			continue;
		}

		if (strcmp(s, "ATTNOTE.WT") == 0)
		{
			_GetAtt(area, areatype, j, &stru, &att);
			dip = _GetFld(att, stru, "DIP");
			xy.x += 1.5 * sin(dip / 360 * 3.14);
			xy.y -= 1.5 * cos(dip / 360 * 3.14);
		}
		else {
			xy.x += 1.5;
			xy.y -= 1.0;
		}


		//determine the first part of the notes
		_GetAtt(area, areatype, j, &stru, &att);
		//标本标注的头顶1或2个字母确定(根据标本类型)
		if (strcmp(s, "SAMNOTE.WT") == 0)
		{
			_GetField(att, stru, "TYPE", fldBuf, sizeof(fldBuf), NULL);

			if (strcmp(fldBuf, "B标本") == 0 || strcmp(fldBuf, "b薄片") == 0 || \
				strcmp(fldBuf, "g光片") == 0 || strcmp(fldBuf, "Z自然重砂") == 0 || \
				strcmp(fldBuf, "H基本化学分析") == 0)
			{
				strncpy(note, fldBuf, 1);
				note[1] = '\0';
			}
			else {
				strncpy(note, fldBuf, 2);
				note[2] = '\0';
			}

			GetFieldCheck(att, stru, argv[1], fldBuf, sizeof(fldBuf), NULL);
			strcat(note, &fldBuf[1]);

			//产状倾角为long型,直接复制到note中去会产生乱码,
			//故使用sprintf将long型转换成string.
		}
		else if (strcmp(s, "ATTNOTE.WT") == 0) {
			GetFieldCheck(att, stru, argv[1], fldBuf, sizeof(fldBuf), NULL);
			sprintf(temp, "%ld", fldBuf[0]);
			strcpy(note, temp);

		}
		else {
			GetFieldCheck(att, stru, argv[1], fldBuf, sizeof(fldBuf), NULL);
			strcpy(note, fldBuf);
		}


		//determine the remaining parts of the notes
		for (i = 2; i < argc; i++)
		{
			strcat(note, "-");
			GetFieldCheck(att, stru, argv[i], fldBuf, sizeof(fldBuf), NULL);
			strcat(note, fldBuf);
		}

		//append the note to area
		pn = _AppendPnt(aiNote, &xy, note, lstrlen(note), &info);

		//update attributes
		_GetAtt(aiNote, areatype, j, &struNote, &attNote);
		GetFieldCheck(att, stru, "ROUTECODE", fldBuf, sizeof(fldBuf), NULL);
		_SetFld(attNote, struNote, "ROUTECODE", fldBuf);
		_WriteAtt(aiNote, areatype, pn, stru, att);
	}

	//保存文件,释放工作区.
	if (_SaveAFile(aiNote, s))
		printf("成功生成标注文件：%s", s);

	if (prjfile)    appendtoprj(prjfile, s);
	else
	{
		TCHAR tempp[MAX_PATH];
		GetCurrentDirectory(sizeof(tempp), tempp);
		TCHAR* FileName = strrchr(tempp, '\\');
		strcpy(tempp, &FileName[1]);
		strcat(tempp, ".MPJ");
		appendtoprj(tempp, s);
	}

	return 1;
}


//prjfile为需要载入的工程文件名
void appendtoprj(TCHAR* prjfile, TCHAR* gisfile)
{

	MAPPROJECT prj;

	if ((prj = _PRJLoadAProject(prjfile)) == 0)
	{
		fprintf(stderr, "工程文件路径使用绝对路径:%s\n", prjfile);
		return;
	}
	if (_PRJFindItemIndex(prj, gisfile) != -1) //若添加的文件存在工程中，则直接返回
	{
		printf("\n");
		return;
	}

	TCHAR dir[MAX_PATH];
	GetCurrentDirectory(sizeof(dir), dir);
	strcat(dir, "\\");
	strcat(dir, gisfile);

	if (!_PRJAppend(prj, dir))
		fprintf(stderr, "被添加文件路径使用绝对路径:%s\n", gisfile);

	if (_PRJGetModifyFlg(prj))
	{
		_PRJSaveProject(prj);
		printf("添加到工程文件%s\n", prjfile);
	}	

}

void TransPara(short area, double &x, double &y)
{

	using std::cout;
	using std::endl;

	MAP_PARA* para1 = new MAP_PARA; //图件的投影参数
	MAP_PARA* para2 = new MAP_PARA; //图件的投影参数

	_GetMapParam(area, para1);
	_GetMapParam(area, para2);

	para1->type = TYPE_PRJ;
	para1->earthParam = EP_Xian80;
	para1->projType = PRJ_Gauss_Kruger;
	para1->rate = 1;
	para1->unit = UNIT_Meter;
	para1->lon = 1200000;


	para2->type = TYPE_JWD;
	para2->earthParam = EP_Xian80;
	para2->rate = 1;
	para2->unit = UNIT_Degree;


	CProjTrans pTrans = _GetCProjTrans();
	_MapToInPara(pTrans, para1);
	_MapToOutPara(pTrans, para2);
	if (!_PntCProjTrans(pTrans, &x, &y))
		cout << "Trans failed!" << endl;//转换后结果存储到x,y变量中
										//	cout << "xd=" << x << ", yd=" << y << endl;
	_DeleteCProjTrans(pTrans);
}

double arealength(short line)
{
	long j, linN, len;
	LIN_INFO LineInfo;
	double length = 0;
	D_DOT* lxy;


	_GetLinNum(line, &j, &linN);
	for (long i = 1; i < linN; i++)
	{
		_GetLin(line, i, &lxy, &len, &LineInfo);
		length += _CalculateLength(lxy, len);
	}
	return length;
}

int copystru(short from, short to)
{
	CATT_STRU * stru;

	if (_GetAreaType(from) == _GetAreaType(to))
	{
		return (_GetAttStru(from, _GetAreaType(from), &stru) &&
			_SetAttStru(to, _GetAreaType(to), stru));
	}
	else {
		return 0;
	}

}


D_3DOT getelev(short elev, short mzk, D_DOT* xy)
{
	//	using std::cout;
	//	using std::endl;

	D_RECT rect;
	double z[10000];
	D_3DOT result;
	L_DOT p;
	DemInfoStrcT* mHdInf = new DemInfoStrcT;

	_GetRange(elev, &rect);
	mHdInf->xmin = rect.xmin;
	mHdInf->xmax = rect.xmax;
	mHdInf->ymin = rect.ymin;
	mHdInf->ymax = rect.ymax;
	mHdInf->zmin = 5; //detla x
	mHdInf->zmax = 5; //detla y
	mHdInf->nx = (mHdInf->xmax - mHdInf->xmin) / mHdInf->zmin;
	mHdInf->ny = (mHdInf->ymax - mHdInf->ymin) / mHdInf->zmax;

	_tnLinDatDiscretingToBuf(elev, mzk, mHdInf, z);

	p.x = (xy->x - rect.xmin) / mHdInf->zmin;
	p.y = (xy->y - rect.ymin) / mHdInf->zmax;
	if (!_tnGetPntDtmParamInGrd(mHdInf, z, &p, &result))
		std::cout << "GetPntDtm failed!" << std::endl;
	return result;
}

time_t strttime(char* t, const char* fmt)
{
	tm tm;
	//std::cout << "time:" << t << std::endl;
	strptime(t, fmt, &tm);
	tm.tm_isdst = -1;
	time_t tt = mktime(&tm);
	//	std::cout << "seconds:" << tt << std::endl;
	return tt;
}

double getz(short terl, char* fldName, D_DOT xy)
{
	double dmin, temp, elev;
	long i, j, dMinIdx, linN, len;
	LIN_INFO linf;
	D_DOT *lxy;
	char *attLin = NULL;
	CATT_STRU *struLin = NULL;


	//	if(_IsEmptyArea(terl)) 

	_GetLinNum(terl, &j, &linN);
	dmin = 5000000;
	for (i = 1; i < linN; i++)
	{
		if (_GetLin(terl, i, &lxy, &len, &linf) <= 0)
			continue;
		temp = _DistofPntToLin(&xy, lxy, len);
		if (dmin > temp)
		{
			dmin = temp;
			dMinIdx = i;
		}
	}
	_GetAtt(terl, _GetAreaType(terl), dMinIdx, &struLin, &attLin);
	elev = _GetFld(attLin, struLin, fldName);
	return elev;
}

float getrand(float diff)
{
	float t = 0;
	static int test;

	if (diff < 0 || diff > 1)
	{
		std::cout << "diff must between 0...1!" << std::endl;
		return 0;
	}

	if (!test)
	{
		srand(time(0));
		test = 1;
	}

	while (!(t < 1 + diff && t  > 1 - diff))
		t = 1.5 * rand() / RAND_MAX;
	return t;

}
//void touch(char* src, char* dst)
void touch(char* src, char* dst)
{
	HANDLE hSrc, hDst;
	FILETIME ftCreate, ftAccess, ftWrite;

	hSrc = CreateFile(src, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	hDst = CreateFile(dst, FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hSrc == INVALID_HANDLE_VALUE)
		std::cout << src << " can not open" << std::endl;

	if (hDst == INVALID_HANDLE_VALUE)
		std::cout << dst << " can not open" << std::endl;
	GetFileTime(hSrc, &ftCreate, &ftAccess, &ftWrite);
	if (!SetFileTime(hDst, &ftCreate, &ftAccess, &ftWrite))
	{
		std::cout << "failed to setFileTime:" << dst << std::endl;
	}
	else {
		std::cout << "Successfully SetFileTime: " << dst << std::endl;
	}

	CloseHandle(hSrc);
	CloseHandle(hDst);
}

int genegps(TCHAR* zfile, TCHAR* begin, TCHAR* end, TCHAR* fmt)
{
	if (zfile == NULL)
	{
		genegpsHelp();
		exit(1);
	}

	using dgs::Gps;
	using std::cout;
	using std::endl;

	AREA_HINST inst;
	short oldgps, newgps, PntSrcArea, elev;
	long j, linN;
	long len;
	double high, length = 0, temp = 0;
	double MapRate;
	D_DOT *lxy, xy;
	LIN_INFO LineInfo;
	PNT_INFO GpsInfo;

	

	time_t start, finish, range, time, last;
	start = strttime(begin, fmt);
	finish = strttime(end, fmt);
	range = finish - start;
	last = start;


	inst = _InitWorkArea(NULL);

	///////////////////////////////////////////////////////////////////////////
	////////////////////////PREDFINED FILE AND INFO///////////////////////////
	///////////////////////////////////////////////////////////////////////////
	//origin gps.wt file where new gps paras and attstru to copy from
	TCHAR* gps = _T("GPS.WT");		
	if ((oldgps = _OpenFileArea(inst, gps)) < 1)
	{
		fprintf(stderr, "gps.wt file doesn't not exist in floder, exit...\n");
		genegpsHelp();
		exit(1);
	}

	//gps point coord generate from line
	TCHAR* PntSrcLine = _T("LINE.WL");
	//the field of terl file where attlitude values from
	TCHAR* zentry = _T("ELEV");		
	
	//timestamp info copy to new gps file
	TCHAR* timefile = _T("FREE.WL");	
	
	//route code attribute, which get from the directory name
	TCHAR dir[MAX_PATH];
	GetCurrentDirectory(sizeof(dir), dir);
	TCHAR* routecode = strrchr(dir, '\\');	
	routecode = &routecode[1];
	///////////////////////////////////////////////////////////////////////////
	/////////////////////////////END///////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	if (_IsAreaEmpty(oldgps))
	{
		cout << "ERROR: " << gps << "gps.wt is empty!" << endl;
		genegpsHelp();
		exit(1);
	}

	MapRate = getrate(oldgps);
	newgps = _OpenArea(inst, PNT);

	if ((elev = _OpenFileArea(inst, zfile)) < 1)
	{
		fprintf(stderr, "terl file %s doesn't exist, exit...\n", zfile);
		genegpsHelp();
		exit(1);
	}

	if((PntSrcArea = _OpenFileArea(inst, PntSrcLine)) < 1)
	{
		fprintf(stderr, "file line.wl doesn't exist, exit...\n");
		genegpsHelp();
		exit(1);
	}



	_GetLinNum(PntSrcArea, &j, &linN);
	_GetPntInfo(oldgps, 1, &GpsInfo);
	if (!(copystru(oldgps, newgps) && _CopyMapParam(oldgps, newgps)))
	{
		cout << "copystru failed because of mismatch of type!" << endl;
		return 0;
	}
	length = arealength(PntSrcArea);

	xy.x = 0;
	xy.y = 0;
	long n = 0;


	for (long i = 1; i < linN; i++)
	{
		_GetLin(PntSrcArea, i, &lxy, &len, &LineInfo);
		for (long ii = 1; ii < len; ii++)
		{
			if (xy.x != 0 && xy.y != 0)
			{
				temp = _DistofPntToPnt(&xy, &lxy[ii]);
				//	cout << "temp from if:" << temp << endl;
			}


			xy.x = lxy[ii].x;
			xy.y = lxy[ii].y;
			
			double x = xy.x * MapRate / 1000;
			double y = xy.y * MapRate / 1000;
			_AppendPnt(newgps, &xy, NULL, 0, &GpsInfo);
			TransPara(oldgps, x, y);
			high = getz(elev, zentry, xy);
			//cout << "high" << high << endl;

			time = last + temp / length * range * getrand(0.5);
			if (time < last)
			{
				time = last + temp / length * range;
			}
			last = time;

			Gps g(xy.x * MapRate / 1000, xy.y * MapRate / 1000, high * getrand(0.05), x, y, time);
			g.writegps(newgps, ++n, routecode);
			//	g.printall();


		}
		xy.x = 0;
		xy.y = 0;

	}


	_SetPackFlag(PntSrcArea, 1);
	_ClearArea(oldgps);
	_CloseArea(oldgps);
	_SetAreaFileName(newgps, gps);
	_SaveAFile(PntSrcArea, PntSrcLine);

	if (_SaveAFile(newgps, gps))
		cout << "Successfully generate GPS!" << endl;

	touch(timefile, gps);

	_CloseAllArea(inst);
	_FreeWorkArea(inst);
	return 1;
}

void sort(short area, int keyc, TCHAR* keyv[])
{
	sortarea(area, keyc, keyv);
	CMapgisArea CArea(area);
	CArea.updateid();
	if (_GetChangeFlag(area))
	{
		TCHAR* FileName = strrchr(_GetAreaFileName(area), '\\');
		if (_SaveFile(area))
			printf("成功重排文件：%s\n", &FileName[1]);
	}
}

void geneelev(TCHAR* zfile, short area, int fldc, TCHAR* fldv[])
{
	AREA_HINST ah;
	short zfilearea;
	TCHAR* zfilefld = fldv[0];
	TCHAR* targetfilefld = fldv[1];

	ah = _InitWorkArea(NULL);

	if ((zfilearea = _OpenFileArea(ah, zfile)) < 1)
	{
		fprintf(stderr, "terl file %s doesn't exist, exit...\n", zfile);
		exit(1);
	}

	long num;
	D_DOT pxy;
	char* dat;
	short len;
	PNT_INFO inf;
	double z;

	_GetRcdNum(area, _GetAreaType(area), &num);
	for (long i = 1; i < num; i++)
	{
		_GetPnt(area, i, &pxy, &dat, &len, &inf);
		z = getz(zfilearea, zfilefld, pxy) * getrand(0.05);
		if(!updateatt(area, i, targetfilefld, (void*)&z))
			fprintf(stderr, "update field %s value failed!\n", targetfilefld);
	}

	if (_GetChangeFlag(area))
	{
		TCHAR* FileName = strrchr(_GetAreaFileName(area), '\\');
		if (_SaveFile(area))
			printf("成功更新文件ZZ数据：%s\n", &FileName[1]);
	}

	_CloseAllArea(ah);
	_FreeWorkArea(ah);
}

bool updateatt(short area, long i, TCHAR* fld, void* fldval)
{
	CATT_STRU* stru;
	char* att;
	char areatype = _GetAreaType(area);

	_GetAtt(area, areatype, i, &stru, &att);
	static char fieldtype = getfldtype(stru, fld);

	if (!fieldtype)
	{
		fprintf(stderr, "doesn't exist field:%s\n", fld);
		exit(1);
	}

	switch (fieldtype)
	{
		case STR_TYPE:
			_SetField(att, stru, fld, (char*)fldval, strlen((char*)fldval));
			break;
		case LONG_TYPE:
			_SetField(att, stru, fld, (char*)fldval, sizeof(long));
			break;
		case DOUBLE_TYPE:
			_SetField(att, stru, fld, (char*)fldval, sizeof(double));
			break;
		default:
			return false;
	}

	if (_WriteAtt(area, areatype, i, stru, att))
		return true;
	else
		return false;
}

char getfldtype(CATT_STRU* stru, TCHAR* fld)
{
	short numberfield = stru->hd.numbfield;
	for (short i = 0; i < numberfield; i++)
	{
		if (isstreq(stru->fldEntry[i].fieldname, fld))
			return stru->fldEntry[i].fieldtype;
	}
	return false;
}

#include "stdafx.h"
#include "dgsattdb.h"
#include "domapi.h"
#include "str.h"
#include "help.h"
#include "mapgistool.h"
#include <OleAuto.h>

/*
 * concantate fields, types and its corresponding limit(optional) in 
 * form of "fld1 type1(limit1), fld2 type2(limit2), ..., fldn typen(limitn)"
 */
TCHAR* fldcat(CATT_STRU* stru)
{
	long i;
	TCHAR* fields = NULL;
	TCHAR tmp[BUFSIZE];

	if((fields = (TCHAR*) malloc(5 * BUFSIZE)) == NULL)
	{
		fputs("malloc err from fldcat\n", stderr);
		return NULL;
	}
	fields[0] = '\0';

	for(i = 0; i < stru->hd.numbfield; i++)
	{
			switch(stru->fldEntry[i].fieldtype)
			{
				case LONG_TYPE:
					sprintf(tmp, "`%s` long, ", stru->fldEntry[i].fieldname);
					break;
				case DOUBLE_TYPE:
					sprintf(tmp, "	`%s` double, ", stru->fldEntry[i].fieldname);
					break;
				default: //STR_TYPE and other data type
					sprintf(tmp, "`%s` varchar(%d), ", stru->fldEntry[i].fieldname,stru->fldEntry[i].msk_leng);
					break;
			}

			//fields = mstrcat(fields, tmp, &allocSize, &takenSize);

			strcat(fields, tmp);

	}

	strcat(fields, "`position` memo, `para` memo");
//	std::cout <<  fields << std::endl;
	return fields;
}

//concantate field names in form of "fld1, fld2, fld3....fldn"
TCHAR* fldncat(CATT_STRU* stru)
{
	long i;
	TCHAR* fldname = NULL;

	if((fldname = (TCHAR*) malloc(5 * BUFSIZE)) == NULL)
	{
		fputs("malloc err from fldncat\n", stderr);
		return NULL;
	}
	fldname[0] = 0;

	for(i = 0; i < stru->hd.numbfield; i++)
	{
		strcat(fldname, "`");
		strcat(fldname, stru->fldEntry[i].fieldname);
		strcat(fldname, "`");
		strcat(fldname, ",");
	}
	fldname[strlen(fldname) - 1] = '\0';
	//std::cout <<"Field Name:" <<  fldname << std::endl;
	return fldname;
}


TCHAR* valcat(CATT_STRU* stru, TCHAR* att)
{
	TCHAR tmp[BUFSIZE];
	TCHAR strBuf[BUFSIZE];
	long lFld = 0;
	double dFld = 0;
	TCHAR* values = NULL;
	
	if((values = (TCHAR*) malloc(5 * BUFSIZE)) == NULL)
	{
		fputs("malloc err from vlacat\n", stderr);
		return NULL;
	}
	values[0] = 0;

   for(long j = 0; j < stru->hd.numbfield; j++)
   {
	   switch(stru->fldEntry[j].fieldtype)
		{
			case STR_TYPE:
				_GetField(att, stru, stru->fldEntry[j].fieldname, strBuf, sizeof(strBuf), NULL);
				sprintf(tmp, "'%s',", strBuf);
				break;
			case LONG_TYPE:
				
				_GetField(att, stru, stru->fldEntry[j].fieldname, (TCHAR*) &lFld, sizeof(long), NULL);
				sprintf(tmp, "%ld,", lFld);
				//std::cout << "long type" << std::endl;
				break;
			case DOUBLE_TYPE:
				_GetField(att, stru, stru->fldEntry[j].fieldname, (TCHAR*) &dFld, sizeof(double), NULL);
				sprintf(tmp, "%0.2f,", dFld);
				break;
			default:
				//strcat(values, "'', ");
				break;
	}

	strcat(values, tmp);

  }
 	//delchar(values, '\n');
    values[strlen(values) - 1] = '\0';
//	std::cout << values << std::endl;
	return values;
}

//field value put in 'TCHAR fld[]'
void getfldvalue(CATT_STRU* stru, TCHAR* att, TCHAR* fld, int len, TCHAR fldval[])
{
	fldval[0] = 0;
	_GetField(att, stru, fld, fldval, len, NULL);
	//std::cout << "fldval:" << fldval <<std::endl;
}

int delchar(TCHAR s[], int ch)
{
	int i, j = 0;
	int len = strlen(s);

	for(i = 0; i < len; i++)
	{
		if(s[i] != ch)
			s[j++] = s [i];
	}
	s[j] = '\0';

	return 0;
}

int filename(TCHAR s[])
{
	int i, j = 0;
	int len = strlen(s);
	for(i = 0; i < len; i++)
	{
		if(s[i] != '.')
			s[j++] = s[i];
		else {
		    s[j] = '\0';
			return 0;
		}
	}
	return 0;

}

TCHAR* file2buf(const TCHAR* file)
{
	FILE* fp;
	long fsize;
	size_t result;
	TCHAR* buffer;

	if((fp = fopen(file, "rb")) == NULL)
	{
		printf("File %s not exists or cannot open\n", file);
		return NULL;
	}

	//obtain file size
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	rewind(fp);

	//allocate memory to cotain the whole file
	if((buffer = (TCHAR*) malloc(fsize + 1)) == NULL)
	{
		fputs("malloc memory err!", stderr);
		return NULL;
	}

	result = fread(buffer, 1, fsize, fp);
	if(result != fsize)
	{
		fputs("read file err!", stderr);
		free(buffer);
		return NULL;
	}
	buffer[fsize] = '\0';
	fclose(fp);
	return buffer;
}

int str2file(const TCHAR* str, TCHAR* fname)
{
	if(str[0] != '\0')
	{
		FILE *fp;
		fp = fopen(fname, "wb");
		fputs(str, fp);
		fclose(fp);
		printf("successfully save DESC content to %s\n", fname);
		return 0;
	}
	else 
	{
		return -1;
	}
}

int strunix2dos(TCHAR* str)
{
	TCHAR* dupstr = NULL;
	int c;
	if(dupstr = _strdup(str))
	{
		while((c = *dupstr++) != '\0')
		{
			if(c == '\n')
			{
				*str++ = '\r';
				*str++ = '\n';
			}
			else
				*str++ = c;
			}

			//*str++ = '\r';
			//*str++ = '\n';
			*str = '\0';
			//free(dupstr);
			return 0;
	}
	else
	{
		printf("in strunix2dos, cannot finish strdup\n");
		return -1;
	}
}

TCHAR* strdos2unix(TCHAR* str)
{
	TCHAR* dupstr = _strdup(str);
	int c;
	if(dupstr != NULL)
	{
		while((c = *dupstr++) != '\0')
			if(c == '\r')
			{
				*str++ = '\n';
			}
			else if(c == '\n')
			{
				continue;
			}
			else
			{
				*str++ = c;
			}

			*str = '\0';
			return &str[0];
	}
	else
	{
		printf("in strunix2dos, cannot finish strdup\n");
		return NULL;
	}
}



TCHAR* replace_char(TCHAR* str, TCHAR find, TCHAR replace){
    TCHAR *current_pos = strchr(str,find);
    while (current_pos){
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
}


bool saverb(ADODB::_RecordsetPtr pRS, TCHAR* no, TCHAR* text, TCHAR* fname)
{
	MSXML2::IXMLDOMDocumentPtr pXMLDom;

	HRESULT hr = pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
	if (FAILED(hr))
	{
		printf("Failed to instantiate an XML DOM. \n");
	}

	try 
	{
		//pXMLDom->preserveWhiteSpace = VARIANT_FALSE;
		pXMLDom->async = VARIANT_FALSE;
		pXMLDom->validateOnParse = VARIANT_FALSE;
		pXMLDom->resolveExternals = VARIANT_TRUE;

		// Create a processing instruction targeted for xml.
		MSXML2::IXMLDOMProcessingInstructionPtr pi = pXMLDom->createProcessingInstruction(L"xml", L"version='1.0' encoding='gb2312'");
		pXMLDom->appendChild(pi);

		// Create the root element (i.e., the documentElement).
		MSXML2::IXMLDOMElementPtr pRoot = pXMLDom->createElement(L"Rgmap");
		
		pRS->MoveFirst();
		while(!pRS->AdoNSEOF)
		{
			_variant_t var = pRS->GetCollect(text);
			if(var.vt != VT_NULL)
			{
				appendNoteElem(pXMLDom, pRoot, (TCHAR*) (_bstr_t) pRS->GetCollect(no), (TCHAR*)(_bstr_t) var);
			}

			pRS->MoveNext();
		}
		//std::cout << "pRoot->text" << (TCHAR*) pRoot->text << std::endl;
		//std::cout << (TCHAR*)pXMLDom->xml << std::endl << std::endl<< std::endl;
		if(!isstreq((TCHAR*) pRoot->text, ""))
			saveXML(pXMLDom, pRoot, fname);
	}
	catch (_com_error errorObject)
	{
		fprintf(stderr, "Exception thrown, HRESULT: 0x%08x\n", errorObject.Error());
	}

	return true;
}


bool singleID(short area)
{
	CATT_STRU* stru = NULL;
	TCHAR* att = NULL;
	long num;
	long idfld;
	long alreadyInSingleID = 0;
	long doubleIDIndex = 0;

	long* id = NULL;
	long* index = NULL;
	_GetRcdNum(area, _GetAreaType(area), &num);

	if (((id = (long*)malloc(num * sizeof(long))) == NULL) || \
		((index = (long*)malloc(num * sizeof(long))) == NULL))
	{
		fprintf(stderr, "malloc err from singleID\n");
		return false;
	};


	for(long i = 1; i < num; i++)
	{
		_GetAtt(area, _GetAreaType(area), i, &stru, &att);
		_GetField(att, stru, "ID", (TCHAR*)&idfld, sizeof(long), NULL);
		if (alreadyInSingleID == 0)
		{
			id[0] = idfld;
			++alreadyInSingleID;
		}
		else
		{
			for (long j = 0; j < alreadyInSingleID; j++)
			{
				if (idfld > id[j])
				{
					if (j == alreadyInSingleID - 1)
					{
						id[alreadyInSingleID++] = idfld;
						break;
					}
					continue;
				}
				else if (idfld < id[j])
				{
					for (long k = alreadyInSingleID; k > j; k--)
					{
						id[k] = id[k - 1];
					}

					id[j] = idfld;
					++alreadyInSingleID;
					break;
				}
				else
				{
					index[doubleIDIndex++] = i;
					break;
				}
			}
		}
	}
	

	if (doubleIDIndex == 0)
	{
		free(id);
		free(index);
		return true;
	}
	
	long front = 1;
	long back = 0;
	long temp = 0;
	long i = 0;

	do
	{
		if (front == alreadyInSingleID)
		{
			temp = id[back];
			for (; i < doubleIDIndex; i++)
			{
				++temp;
				_GetAtt(area, _GetAreaType(area), index[i], &stru, &att);
				_SetField(att, stru, "ID", (TCHAR*)&temp, sizeof(long));
				_WriteAtt(area, _GetAreaType(area), index[i], stru, att);
			}
				
			free(id);
			free(index);
			return true;
		}

		else if (id[front] - id[back] > 1)
		{
			temp = id[back];
			for (long j = 0; j < id[front] - id[back] - 1; j++)
			{
				if (i == doubleIDIndex)
				{
					free(id);
					free(index);
					return true;
				}
				else
				{
					++temp;
					_GetAtt(area, _GetAreaType(area), index[i], &stru, &att);
					_SetField(att, stru, "ID", (TCHAR*)&temp, sizeof(long));
					_WriteAtt(area, _GetAreaType(area), index[i], stru, att);
					++i;	
				}
			}

			++front;
			++back;
		}
		
		else
		{
			++back;
			++front;
		}
			
	} while (i < doubleIDIndex);
}


void setid(short area)
{
	CATT_STRU* stru = NULL;
	TCHAR* att = NULL;
	long num;

	_GetRcdNum(area, _GetAreaType(area), &num);

	for (long i = 1; i < num; i++)
	{
		_GetAtt(area, _GetAreaType(area), i, &stru, &att);
		_SetField(att, stru, "ID", (TCHAR*)&i, sizeof(long));
		_WriteAtt(area, _GetAreaType(area), i, stru, att);
	}
}


void getid(CATT_STRU* stru, TCHAR* att, TCHAR* file, TCHAR* id1, TCHAR* id2, int len, long* id)
{
	if (isstreq(file, "GROUTE"))
	{
		_GetField(att, stru, "MAPCODE", id1, len, NULL);
		_GetField(att, stru, "ROUTECODE", id2, len, NULL);
	}
	else if (isstreq(file, "GPOINT"))
	{
		_GetField(att, stru, "ROUTECODE", id1, len, NULL);
		_GetField(att, stru, "GEOPOINT", id2, len, NULL);
	}
	else if (isstreq(file, "ROUTING"))
	{
		_GetField(att, stru, "GEOPOINT", id1, len, NULL);
		_GetField(att, stru, "R_CODE", id2, len, NULL);
	}
	else if (isstreq(file, "BOUNDARY"))
	{
		_GetField(att, stru, "GEOPOINT", id1, len, NULL);
		_GetField(att, stru, "SUBPOINT", id2, len, NULL);
	}
	else
	{
		_GetField(att, stru, "GEOPOINT", id1, len, NULL);
		_GetField(att, stru, "CODE", id2, len, NULL);
	}
	_GetField(att, stru, "ID", (char*)id, sizeof(long), NULL);
}

bool putPNTpara2rcdset(short area, long index, ADODB::_RecordsetPtr pRS)
{
	HRESULT hr = S_OK;
	D_DOT pxy;
	TCHAR* pdat = NULL;
	short plen;
	PNT_INFO pinfo;
	long flen;

	_GetPnt(area, index, &pxy, &pdat, &plen, &pinfo);
	flen = sizeof(D_DOT) + sizeof(PNT_INFO) + plen;

	SAFEARRAY *psa;
	SAFEARRAYBOUND rgsabound[1];
	rgsabound[0].lLbound = 0;
	rgsabound[0].cElements = flen;
	psa = SafeArrayCreate(VT_UI1, 1, rgsabound);

	char* p = NULL;
	if (SUCCEEDED(hr = SafeArrayAccessData(psa, (void**)&p)))
	{
		memmove(p, &pxy, sizeof(D_DOT));
		memmove(p + sizeof(D_DOT), &pinfo, sizeof(PNT_INFO));
		memmove(p + sizeof(D_DOT) + sizeof(PNT_INFO), pdat, plen);
	
		if (SUCCEEDED(hr = SafeArrayUnaccessData(psa)))
		{
			VARIANT var;
			var.vt = VT_ARRAY | VT_UI1;
			var.parray = psa;

			pRS->Fields->GetItem("position")->AppendChunk(var);
			pRS->Update();
		}					
	}
	return true;
}

bool putLINpara2rcdset(short area, long index, ADODB::_RecordsetPtr pRS)
{
	HRESULT hr = S_OK;
	D_DOT* lxy = NULL;
	LIN_INFO linfo;
	long len;
	long flen;

	if(_GetLin(area, index, &lxy, &len, &linfo) <=0 )
		return false;

	flen = len * sizeof(D_DOT) + sizeof(LIN_INFO);

	SAFEARRAY *psa;
	SAFEARRAYBOUND rgsabound[1];
	rgsabound[0].lLbound = 0;
	rgsabound[0].cElements = flen;
	psa = SafeArrayCreate(VT_UI1, 1, rgsabound);

	char* p = NULL;
	if (SUCCEEDED(hr = SafeArrayAccessData(psa, (void**)&p)))
	{
		memmove(p, lxy, len * sizeof(D_DOT));
		memmove((p + len * sizeof(D_DOT)), &linfo, sizeof(LIN_INFO));

		if (SUCCEEDED(hr = SafeArrayUnaccessData(psa)))
		{
			_variant_t var;
			var.vt = VT_ARRAY | VT_UI1;
			var.parray = psa;

			pRS->Fields->GetItem("position")->AppendChunk(var);
			pRS->Update();
		}
	}
	return true;
}

bool putREGpara2rcdset(short area, long index, ADODB::_RecordsetPtr pRS)
{
	HRESULT hr = S_OK;
	REG_INFO rinfo;
	long* regdata;
	long len;
	long flen;

	_GetReg(area, index, &regdata, &len, &rinfo);

	flen = len * sizeof(long) + sizeof(REG_INFO);

	SAFEARRAY *psa;
	SAFEARRAYBOUND rgsabound[1];
	rgsabound[0].lLbound = 0;
	rgsabound[0].cElements = flen;
	psa = SafeArrayCreate(VT_UI1, 1, rgsabound);

	char* p = NULL;
	if (SUCCEEDED(hr = SafeArrayAccessData(psa, (void**)&p)))
	{
		memmove(p, regdata, len * sizeof(long));
		memmove(p + len * sizeof(long), &rinfo, sizeof(REG_INFO));

		if (SUCCEEDED(hr = SafeArrayUnaccessData(psa)))
		{
			VARIANT var;
			var.vt = VT_ARRAY | VT_UI1;
			var.parray = psa;

			pRS->Fields->GetItem("position")->AppendChunk(var);
			pRS->Update();
		}
	}
	return true;
}

bool putpara2rcdset(short area, long index, ADODB::_RecordsetPtr pRS)
{
	char areatype;
	areatype = _GetAreaType(area);
	switch (areatype)
	{
	case PNT:
		putPNTpara2rcdset(area, index, pRS);
		break;
	case LIN:
		putLINpara2rcdset(area, index, pRS);
		break;
	case REG:
		putREGpara2rcdset(area, index, pRS);
		break;
	case NET:case TBL:
		break;
	default:
		break;
	}
	return true;
}

int putField2rcdset(short area, long index, CATT_STRU* stru, TCHAR* att, ADODB::_ConnectionPtr pConn, ADODB::_RecordsetPtr pRS, TCHAR* file)
{
	int flderr = 0;
	long lFld;
	double dFld;
	TCHAR strFld[150];
	TCHAR sql[BUFSIZE];
	TCHAR* fld = NULL;
	TCHAR* val = NULL;

	TCHAR key[20];
	TCHAR key1[20];
	long id;
	getid(stru, att, file, key, key1, sizeof(key), &id);

	//std::cout << '\n' << file << std::endl;
	try {

		//isRcdExist(pConn, pRS, stru, att, file);

		if (isRcdExist(pConn, pRS, stru, att, file))
		{
			//record already exists
			std::cout << "Exist,记录已经存在数据库中：ID=" << id << "," << key << "-" << key1 << std::endl;
			pRS->Close();
			return flderr;
		}

		else

		{
			/*
			fld = fldncat(stru);
			val = valcat(stru, att);
			sprintf(sql, "INSERT INTO `%s` (%s) VALUES (%s)", file, fld, val);
			msql(pRS, pConn, sql);
			free(fld);
			free(val);
			*/

			pRS->AddNew();

			if (isstreq(file, "GROUTE"))
			{
				_GetField(att, stru, "ROUTECODE", strFld, sizeof(strFld), NULL);
				pRS->PutCollect("ROUTECODE", strFld);
			}
			else if (isstreq(file, "GPOINT"))
			{
				_GetField(att, stru, "GEOPOINT", strFld, sizeof(strFld), NULL);
				pRS->PutCollect("GEOPOINT", strFld);
			}
			pRS->Update();

			for (long j = 0; j < stru->hd.numbfield; j++)
			{
				switch (stru->fldEntry[j].fieldtype)
				{

				case STR_TYPE:
					_GetField(att, stru, stru->fldEntry[j].fieldname, strFld, sizeof(strFld), NULL);
					pRS->PutCollect(stru->fldEntry[j].fieldname, strFld);
					pRS->Update();
					break;

				case LONG_TYPE:
					_GetField(att, stru, stru->fldEntry[j].fieldname, (TCHAR*)&lFld, sizeof(long), NULL);
					pRS->PutCollect(stru->fldEntry[j].fieldname, lFld);
					pRS->Update();
					break;

				case DOUBLE_TYPE:
					_GetField(att, stru, stru->fldEntry[j].fieldname, (TCHAR*)&dFld, sizeof(double), NULL);
					pRS->PutCollect(stru->fldEntry[j].fieldname, dFld);
					pRS->Update();
					break;
				default:
					flderr++;
					std::cerr << "不支持string, long, double以外的数据类型" << std::endl;
					break;
				}
			}

			putpara2rcdset(area, index, pRS);

			if (havedesc(file))
			{
				putDesc2rcdset(file, stru, att, pRS);
			}

			pRS->Close();
			std::cout << "Imported,成功将记录导入数据库中, ID=" << id << "," << key << "-" << key1 << std::endl;
		}

		
	}
	catch (_com_error& e)
	{
		pRS->CancelUpdate();
		pRS->Close();
		std::cerr << "_COM_ERROR: ID=" << id << "," << key << "-" << key1 << "\t" << e.Description() << std::endl;
	}

	return flderr;
}

int putDesc2rcdset(TCHAR* file, CATT_STRU* stru, TCHAR* att, ADODB::_RecordsetPtr pRS)
{
	TCHAR dir[MAX_PATH];
	TCHAR pdir[MAX_PATH];
	TCHAR gpno[20];   //gpoint number
	TCHAR subno[10];  //R or B subNO.

	TCHAR* desc = NULL;
	TCHAR* pdesc = NULL;

	TCHAR rdesc[BUFSIZE];
	TCHAR prdesc[BUFSIZE];
	TCHAR xpath[20];


	if (isstreq(file, "GROUTE"))
	{
		getfldvalue(stru, att, "ROUTECODE", sizeof(gpno), gpno);
		sprintf(dir, "note\\%s.txt", gpno);
		sprintf(pdir, "note\\%sPZ.txt", gpno);

		desc = file2buf(dir);
		pdesc = file2buf(pdir);
	}
	else if (isstreq(file, "GPOINT"))
	{
		getfldvalue(stru, att, "GEOPOINT", sizeof(gpno), gpno);
		sprintf(dir, "note\\%s_P.TXT", gpno);
		sprintf(pdir, "note\\%s_PP.TXT", gpno);

		desc = file2buf(dir);
		pdesc = file2buf(pdir);
	}
	else if (isstreq(file, "ROUTING"))
	{
		getfldvalue(stru, att, "GEOPOINT", sizeof(gpno), gpno);
		getfldvalue(stru, att, "R_CODE", sizeof(subno), subno);
		sprintf(dir, "note\\%s_R.xml", gpno);
		sprintf(pdir, "note\\%s_PR.xml", gpno);
		sprintf(xpath, "//Note[@No='%s']", subno);


		if (!getSubNode(dir, xpath, rdesc))
			rdesc[0] = 0;
		if (!getSubNode(pdir, xpath, prdesc))
			prdesc[0] = 0;

		strunix2dos(rdesc);
		strunix2dos(prdesc);
	}
	else if (isstreq(file, "BOUNDARY"))
	{
		getfldvalue(stru, att, "GEOPOINT", sizeof(gpno), gpno);
		getfldvalue(stru, att, "SUBPOINT", sizeof(subno), subno);
		sprintf(dir, "note\\%s_B.xml", gpno);
		sprintf(pdir, "note\\%s_PB.xml", gpno);
		sprintf(xpath, "//Note[@No='%s']", subno);

		if (!getSubNode(dir, xpath, rdesc))
			rdesc[0] = 0;
		if (!getSubNode(pdir, xpath, prdesc))
			prdesc[0] = 0;

		strunix2dos(rdesc);
		strunix2dos(prdesc);
	}

	if (isstreq(file, "GROUTE") || \
		isstreq(file, "GPOINT"))
	{
		if (desc == NULL)
		{
			pRS->PutCollect("DESC", "");
		}
		else
		{
			pRS->PutCollect("DESC", desc);
		}

		
		if(pdesc == NULL)
		{
			pRS->PutCollect("PDESC", "");
		}
		else
		{
			pRS->PutCollect("PDESC", pdesc);
		}
	}
	else if (isstreq(file, "BOUNDARY") || \
			 isstreq(file, "ROUTING"))
	{
		pRS->PutCollect("DESC", rdesc);
		pRS->PutCollect("PDESC", prdesc);
	}

	pRS->Update();

	if(desc == NULL)
		free(desc);
	if (pdesc == NULL)
		free(pdesc);

	printf("successfully put DESC and PDESC content into datebase\n");
	return 0;
}




void printid(TCHAR* file, CATT_STRU* stru, TCHAR* att)
{
	TCHAR id[15];
	if (isstreq(file, "GROUTE"))
	{
		_GetField(att, stru, "ROUTECODE", id, sizeof(id), NULL);
	}
	else
	{
		if (!_GetField(att, stru, "GEOPOINT", id, sizeof(id), NULL))
			fprintf(stderr, "%s: does not have GEOPOINT field\n", file);
	}
	printf("\n%s\n", id);
}

HRESULT msql(ADODB::_RecordsetPtr pRS, ADODB::_ConnectionPtr pConn, _bstr_t query)
{
	return pRS->Open(query,
		_variant_t((IDispatch *)pConn, true),
		ADODB::adOpenUnspecified,
		ADODB::adLockOptimistic,
		ADODB::adCmdText);
}


bool isTableExist(TCHAR* tableName, ADODB::_ConnectionPtr pConn)
{

	ADODB::_RecordsetPtr pSet;
	pSet = pConn->OpenSchema(ADODB::adSchemaTables);
	while (!(pSet->AdoNSEOF))
	{

		//获取表格
		_variant_t table_name = pSet->Fields->GetItem("TABLE_NAME")->Value;

		//获取表格类型
		_variant_t table_type = pSet->Fields->GetItem("TABLE_TYPE")->Value;

		if (strcmp(((LPCSTR)(_bstr_t)table_type), "TABLE") == 0)
			if (isstreq((LPCSTR)(_bstr_t)table_name, tableName))
				return true;

		pSet->MoveNext();
	}

	pSet->Close();
	return false;

}

bool isRcdExist(ADODB::_ConnectionPtr pConn, ADODB::_RecordsetPtr pRS, CATT_STRU* stru, TCHAR* att, TCHAR* tablename)
{
	TCHAR query[100];
	TCHAR routecode[10];
	long id;

	_GetField(att, stru, "ID", (TCHAR*)&id, sizeof(long), NULL);
	_GetField(att, stru, "ROUTECODE", routecode, sizeof(routecode), NULL);
	sprintf(query, "SELECT * FROM %s WHERE ID=%d AND ROUTECODE='%s';", tablename, id, routecode);
	msql(pRS, pConn, query);
	if (!pRS->AdoNSEOF)
	{
		//pRS->Close();
		return true;
	}
	else
	{
		//pRS->Close();
		return false;
	}
}

int dgs2db(short area, ADODB::_ConnectionPtr pConn, ADODB::_RecordsetPtr pRS)
{
	if (FAILED(::CoInitialize(NULL)))
		return -1;

	long num;
	CATT_STRU* stru = NULL;
	TCHAR* att = NULL;
	TCHAR file[20];

	_GetAreaFileName1(area, file, sizeof(file));
	printf("-----------------------------------------\nMapgis --->>> datebase:%s\n", file);
	filename(file);

	//if table not exists, then create the cooresponding table
	if (!isTableExist(file, pConn))
	{
		TCHAR* tmp = NULL;
		if ((tmp = (TCHAR*)malloc(2 * BUFSIZE)) == NULL)
		{
			fputs("malloc for tmp in main err\n", stderr);
			return -1;
		}

		TCHAR* Field = NULL;
		_GetAttStru(area, _GetAreaType(area), &stru);

		if ((Field = fldcat(stru)) == NULL)
		{
			fputs("create fields failure\n", stderr);
			return -1;
		}

		if (havedesc(file))
		{
			sprintf(tmp, "CREATE TABLE %s (%s, `DESC` memo, `PDESC` memo);", file, Field);
		}
		else {
			sprintf(tmp, "CREATE TABLE %s (%s);", file, Field);
		}

		free(Field);
		_bstr_t query = tmp;
		msql(pRS, pConn, query);
		printf("create table %s\n", file);
		free(tmp);
	}

	_GetRcdNum(area, _GetAreaType(area), &num);

	for (long i = 1; i < num; i++)
	{
		if (_GetAtt(area, _GetAreaType(area), i, &stru, &att) <= 0)
			continue;
		//printid(file, stru, att);

		putField2rcdset(area, i, stru, att, pConn, pRS, file);
	}

	::CoUninitialize();
}


long AppendEntity(short area, void* psa, long lngsize)
{
	char areatype;
	areatype = _GetAreaType(area);

	switch (areatype)
	{
	case PNT:
		return AppendPnt(area, psa, lngsize);
	case LIN:
		return AppendLin(area, psa, lngsize);
		break;
	case REG:
		break;
	default:
		std::cerr << "not support this type of file" << std::endl;

	}
}

long AppendPnt(short area, void* psa, long lngsize)
{
	HRESULT hr = S_OK;
	D_DOT* pxy = NULL;
	PNT_INFO* pinfo = NULL;
	char* pdat = NULL;
	short len;
	char* p = NULL;
	long pntNum;

	//if (SUCCEEDED(hr = SafeArrayAccessData(psa, (void**)&p)))
	{
		pxy = (D_DOT*)psa;
		pinfo = (PNT_INFO*)((char*)psa + sizeof(D_DOT));
		pdat = (char*) psa + sizeof(D_DOT) + sizeof(PNT_INFO);
		len = lngsize - sizeof(D_DOT) - sizeof(PNT_INFO);

		pntNum = _AppendPnt(area, pxy, pdat, len, pinfo);

		//SafeArrayUnaccessData(psa);
	}

	return pntNum;
}

long AppendLin(short area, void* psa, long lngsize)
{
	HRESULT hr = S_OK;
	D_DOT* lxy = NULL;
	LIN_INFO* linfo;
	long len;
	long linNum;

	len = lngsize - sizeof(LIN_INFO);

	//if (SUCCEEDED(hr = SafeArrayAccessData(psa, (void**)&p)))
	{
		lxy = (D_DOT*)psa;
		linfo = (LIN_INFO*)((char*)psa + len - 1);
		linNum = _AppendLin(area, lxy, len / sizeof(D_DOT), linfo);

		//SafeArrayUnaccessData(psa);
	}

	return linNum;
}


bool PopulateAtt(short area, long index, ADODB::_RecordsetPtr pRS)
{
	CATT_STRU* stru = NULL;
	TCHAR* att = NULL;

	if (_GetAtt(area, _GetAreaType(area), index, &stru, &att) <= 0)
		;
	for (long j = 0; j < stru->hd.numbfield; j++)
	{
		long lfld = 1;
		double dfld = 0;
		_variant_t fldv;

		fldv = pRS->GetCollect(stru->fldEntry[j].fieldname);
		if (fldv.vt != VT_NULL)
		{
			switch (stru->fldEntry[j].fieldtype)
			{

			case STR_TYPE:
				_SetFld(att, stru, stru->fldEntry[j].fieldname, (LPSTR)(_bstr_t)fldv);
				break;
			case LONG_TYPE:
				lfld = atol((LPSTR)(_bstr_t)fldv);

				_SetFld(att, stru, stru->fldEntry[j].fieldname, (TCHAR*)&lfld);
				break;
			case DOUBLE_TYPE:
				dfld = atof((LPSTR)(_bstr_t)fldv);
				_SetFld(att, stru, stru->fldEntry[j].fieldname, (TCHAR*)&dfld);
				break;
			default:
				break;
			}
		}
	}
	if (!_WriteAtt(area, _GetAreaType(area), index, stru, att))
		;
		//fprintf(stderr, "file %s of %s writeatt error", file, gpno);
	//printf("successfully write attributes to mapgis file:%s\n", file);
}

bool PopulateDesc(TCHAR* file, ADODB::_ConnectionPtr pConn, ADODB::_RecordsetPtr pRS)
{
	ADODB::_RecordsetPtr pRStmp("ADODB.Recordset");
	TCHAR fname[30];
	TCHAR query[100];

	if (isstreq(file, "GROUTE"))
	{
		sprintf(fname, "note\\%s.txt", (LPCSTR)(_bstr_t)pRS->GetCollect("ROUTECODE"));
		str2file((LPCSTR)(_bstr_t)pRS->GetCollect("DESC"), fname);
		sprintf(fname, "note\\%sPZ.txt", (LPCSTR)(_bstr_t)pRS->GetCollect("ROUTECODE"));
		str2file((LPCSTR)(_bstr_t)pRS->GetCollect("PDESC"), fname);
	}
	else
	{
		if (isstreq(file, "GPOINT"))
		{
			sprintf(fname, "note\\%s_P.TXT", (LPCSTR)(_bstr_t)pRS->GetCollect("GEOPOINT"));
			str2file((LPCSTR)(_bstr_t)pRS->GetCollect("DESC"), fname);
			sprintf(fname, "note\\%s_PP.TXT", (LPCSTR)(_bstr_t)pRS->GetCollect("GEOPOINT"));
			str2file((LPCSTR)(_bstr_t)pRS->GetCollect("PDESC"), fname);
		}
		else if (isstreq(file, "ROUTING"))
		{
			sprintf(query, "SELECT * FROM %s WHERE GEOPOINT='%s' ORDER BY R_CODE", file, (LPCSTR)(_bstr_t)pRS->GetCollect("GEOPOINT"));
			msql(pRStmp, pConn, query);

			sprintf(fname, "note\\%s_R.xml", (LPCSTR)(_bstr_t)pRS->GetCollect("GEOPOINT"));
			saverb(pRStmp, "R_CODE", "DESC", fname);

			sprintf(fname, "note\\%s_PR.xml", (LPCSTR)(_bstr_t)pRS->GetCollect("GEOPOINT"));
			saverb(pRStmp, "R_CODE", "PDESC", fname);

			pRStmp->Close();
		}
		else if (isstreq(file, "BOUNDARY"))
		{
			sprintf(query, "SELECT * FROM %s WHERE GEOPOINT='%s' ORDER BY SUBPOINT", file, (LPCSTR)(_bstr_t)pRS->GetCollect("GEOPOINT"));
			msql(pRStmp, pConn, query);

			sprintf(fname, "note\\%s_B.xml", (LPCSTR)(_bstr_t)pRS->GetCollect("GEOPOINT"));
			saverb(pRStmp, "SUBPOINT", "DESC", fname);

			sprintf(fname, "note\\%s_PB.xml", (LPCSTR)(_bstr_t)pRS->GetCollect("GEOPOINT"));
			saverb(pRStmp, "SUBPOINT", "PDESC", fname);

			pRStmp->Close();
		}
	}
}

int db2dgs(short area, ADODB::_ConnectionPtr pConn, ADODB::_RecordsetPtr pRS)
{
	TCHAR query[100];
	TCHAR file[20];
	_variant_t var;
	long lngsize;

	TCHAR dir[MAX_PATH];
	GetCurrentDirectory(sizeof(dir), dir);
	TCHAR* routecode = strrchr(dir, '\\');
	routecode = &routecode[1];

	_GetAreaFileName1(area, file, sizeof(file));
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "更新路线属性数据" << routecode << ": " << file << std::endl;
	filename(file);

	short newarea = _OpenArea(_GetAreaInstance(area), _GetAreaType(area));
	copystru(area, newarea);
	if (!_CopyMapParam(area, newarea))
	{
		std::cout << "copymappara error!" << std::endl;
		return -1;
	}

	//ChangFieldType(pConn, file, "`position`", "memo");

	sprintf(query, "SELECT * FROM %s WHERE ROUTECODE='%s'", file, routecode);
	msql(pRS, pConn, query);
	try {
		while (!pRS->AdoNSEOF)
		{
			lngsize = pRS->Fields->GetItem("position")->ActualSize;
			var = pRS->Fields->GetItem("position")->GetChunk(lngsize);

			if (var.vt != VT_NULL)
			{
				PopulateAtt(newarea, AppendEntity(newarea, var.parray, lngsize), pRS);
				if (havedesc(file))
					PopulateDesc(file, pConn, pRS);
			}
			else
			{
				std::cout << "position 数据为空: ID = " << (_bstr_t)pRS->GetCollect("ID") << std::endl;
				continue;
			}

			pRS->MoveNext();
		}
	}
	catch (_com_error& e)
	{
		std::cerr << "error db2dgs: " << e.Description() << std::endl;
	}

	pRS->Close();
	//ChangFieldType(pConn, file, "`position`", "OLEObject");

	_GetAreaFileName1(area, file, sizeof(file));
	_CloseArea(area);
	if (_GetChangeFlag(newarea))
	{
		if (_SaveAFile(newarea, file))
			printf("成功更新属性数据：%s\n", file);
	}

	_CloseArea(newarea);
	return 0;
}

void ChangeFieldType(TCHAR* dbfile, int mapgisc, TCHAR* mapgisv[])
{
	CoInitialize(NULL);
	HRESULT hr;

	TCHAR query[100];
	TCHAR cn[BUFSIZE / 2];
	sprintf(cn, "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=%s; Jet OLEDB:Database Password=;", dbfile);
	_bstr_t bstrConnect = cn;

	try
	{
		ADODB::_ConnectionPtr pConn("ADODB.Connection");
		hr = pConn->Open(bstrConnect, "admin", "", ADODB::adConnectUnspecified);
		if (SUCCEEDED(hr))
		{

			ADODB::_RecordsetPtr pRS("ADODB.Recordset");
			for (int i = 2; i < mapgisc; i++)
			{
				TCHAR* file = mapgisv[i];
				filename(file);
				sprintf(query, "ALTER TABLE %s ALTER COLUMN `%s` %s", file, mapgisv[0], mapgisv[1]);
				msql(pRS, pConn, query);
				//pRS->Close();
			}
		}
	}
	catch (_com_error& e)
	{
		std::cerr << "ChangeFieldType:" << e.Description() << std::endl;
	}
}


int dgsattdb(attdbway way, TCHAR* dbfile, int mapgisc, TCHAR *mapgisv[])
{
	extern TCHAR* cwd;

	if (mapgisc < 1 || !way) {
		dgsattdbHelp();
	}
	else 
	{
		TCHAR cn[BUFSIZE / 2];
		sprintf(cn, "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=%s; Jet OLEDB:Database Password=;", dbfile);
		_bstr_t bstrConnect = cn;

		AREA_HINST ah;
		short MapgisArea;
		ah = _InitWorkArea(NULL);
		CoInitialize(NULL);
		HRESULT hr;

		try
		{
			ADODB::_ConnectionPtr pConn("ADODB.Connection");
			hr = pConn->Open(bstrConnect, "admin", "", ADODB::adConnectUnspecified);
			if (SUCCEEDED(hr))
			{
				//cout << "Successfully connected to database. Data source name:\n  "
				//	<< pConn->GetConnectionString() << endl;

				ADODB::_RecordsetPtr pRS("ADODB.Recordset");

				std::cout << "**************************************************************" << std::endl;
				std::cout << "Mapgis工程文件路径：" << cwd << std::endl;
				std::cout << "数据库文件路径：" << dbfile << std::endl;

				for (int i = 0; i < mapgisc; i++)
				{
					TCHAR* file = mapgisv[i];
					if ((MapgisArea = _OpenFileArea(ah, file)) < 1) {
						std::cout << file << " not exists or cannot open!" << std::endl;
						continue;
					}

					//in generate mode, data flow from routefile to database
					if (way == MAPGIS2DB) {
						//printf("mapgisv[%d]: %s\n", optind, mapgisv[optind]);

						//singleID(MapgisArea);
						dgs2db(MapgisArea, pConn, pRS);
						//if (_GetChangeFlag(MapgisArea))
						//	_SaveAFile(MapgisArea, file);
					}
					//in restore mode data flow from database to routefile
					else if(way == DB2MAPGIS){
						db2dgs(MapgisArea, pConn, pRS);
						//printf("mapgisv[%d]: %s\n", optind, mapgisv[optind]);
					}
					else {
						fprintf(stderr, "dgsattdb mode must be '-g' or '-r'\n");
						exit(1);
					}
					_CloseArea(MapgisArea);
				}


				_FreeWorkArea(ah);
				pConn->Close();
				std::cout << "**************************************************************" << std::endl;
			}
			else {
				std::cerr << "Unable to connect to data source: " << bstrConnect << std::endl;
			}

		}
		
		catch (_com_error& e)
		{
			std::cerr << "_COM_ERROR:" << e.Description() << std::endl;
			
		}
		
	}
}



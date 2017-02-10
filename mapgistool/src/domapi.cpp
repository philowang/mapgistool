#include "stdafx.h"
#include "domapi.h"


// Macro that calls a COM method returning HRESULT value.
#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUp; } while(0)

void createDOM(MSXML2::IXMLDOMDocumentPtr pXMLDom)
{
	HRESULT hr = pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
	if (FAILED(hr))
	{
		printf("Failed to instantiate an XML DOM. \n");
	}
	try {
		pXMLDom->async = VARIANT_FALSE;
		pXMLDom->validateOnParse = VARIANT_FALSE;
		pXMLDom->resolveExternals = VARIANT_FALSE;
		pXMLDom->preserveWhiteSpace = VARIANT_TRUE;
	}
	catch (_com_error errorObject)
	{
		printf("in createDOM, Exception thrown, HRESULT: 0x%08x\n", errorObject.Error());
	}
}

void loadxml(MSXML2::IXMLDOMDocumentPtr pXMLDom, TCHAR* xmlfile)
{
	
	if (pXMLDom->load(xmlfile) != VARIANT_TRUE)
	{
		pXMLDom->parseError->errorCode;
		printf("Failed to load DOM from %s.\n%s\n",
			xmlfile, (LPCSTR)pXMLDom->parseError->Getreason());
	}
}



//xpath = "//Note[@No='0']"
bool getSubNote(MSXML2::IXMLDOMDocumentPtr pXMLDom, TCHAR* xpath, TCHAR s[])
{
    try
    {
        // Query a single node.
        MSXML2::IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode(xpath);
        if (pNode)
        {
            //printf("Result from selectSingleNode:\nNode, <%s>:\n\t%s\n\n",
              //    (LPCSTR)pNode->nodeName, (LPCSTR)pNode->text);
			s[0] = 0;
			strcpy(s, (TCHAR*)pNode->text);
			std::cout << "from getSubNote:" << s << std::endl;
			return true;
        }
        else
        {
            printf("No node is fetched.\n");
			return false;
        }
    }
    catch (_com_error errorObject)
    {
        printf("Exception thrown, HRESULT: 0x%08x", errorObject.Error());
    }
}


void createRgmap(MSXML2::IXMLDOMDocumentPtr pXMLDom, MSXML2::IXMLDOMElementPtr pRoot)
{
    try
    {
		//createDOM(pXMLDom);
        // Create a processing instruction targeted for xml.
        MSXML2::IXMLDOMProcessingInstructionPtr pi = pXMLDom->createProcessingInstruction(L"xml", L"version='1.0' encoding='gb2312'");
        pXMLDom->appendChild(pi);

        // Create the root element (i.e., the documentElement).
        pRoot = pXMLDom->createElement(L"Rgmap");
    }
    catch (_com_error errorObject)
    {
        printf("exception thrown, HRESULT: 0x%08x\n\n", errorObject.Error());
    }
}


void appendNoteElem(MSXML2::IXMLDOMDocumentPtr pXMLDom, MSXML2::IXMLDOMElementPtr pParent, TCHAR* attval, TCHAR* text)
{
	try {
		// Create an element to hold text content.
		MSXML2::IXMLDOMElementPtr pe = pXMLDom->createElement(L"Note");
		pe->text = text;
		
		// Create a "No" attribute for the parent element and
		// assign the "attval" as the attribute value.
		MSXML2::IXMLDOMAttributePtr pa = pXMLDom->createAttribute(L"No");
		pa->value = attval;
		pe->setAttributeNode(pa);

		// Add newline + tab for indentation.
		pParent->appendChild(pXMLDom->createTextNode(L"\n"));
		//append the newly created node to parent
		pParent->appendChild(pe);
	}
	catch (_com_error errorObject)
	{
		printf("exception thrown, HRESULT:0x%08x", errorObject.Error());
	}
}


void saveXML(MSXML2::IXMLDOMDocumentPtr pXMLDom, MSXML2::IXMLDOMElementPtr pRoot, TCHAR* filename)
{
	//add new line
	pRoot->appendChild(pXMLDom->createTextNode(L"\n"));

	// Add the root element to the DOM instance.
	pXMLDom->appendChild(pRoot);

	//printf("Dynamically created xmlfile(%s):\n%s\n", filename, static_cast<PCSTR>(pXMLDom->xml));

	HRESULT hr = pXMLDom->save(filename);
	if (SUCCEEDED(hr))
	{
		printf("successfully save xmlfile:%s\n", filename);
	}
	else
	{
		printf("Failed to save %s\n", filename);
	}
}


bool getSubNode(TCHAR* xmlfile, TCHAR* xpath, TCHAR* desc)
{
	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	desc[0] = 0;

	HRESULT hr = pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
	if (FAILED(hr))
	{
		printf("Failed to instantiate an XML DOM. \n");
	}
	try {
		pXMLDom->preserveWhiteSpace = VARIANT_FALSE;
		pXMLDom->async = VARIANT_FALSE;
		pXMLDom->validateOnParse = VARIANT_FALSE;
		pXMLDom->resolveExternals = VARIANT_FALSE;

	if (pXMLDom->load(xmlfile) != VARIANT_TRUE)
	{
		pXMLDom->parseError->errorCode;
		printf("Failed to load DOM from %s.\n%s\n",
			xmlfile, (LPCSTR)pXMLDom->parseError->Getreason());
	}

	// Query a single node.
    MSXML2::IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode(xpath);
    if (pNode)
        {
           // printf("Result from selectSingleNode:\nNode, <%s>:\n\t%s\n\n",
            //      (LPCSTR)pNode->nodeName, (LPCSTR)pNode->text);
			desc[0] = 0;
			strcpy(desc, (TCHAR*)pNode->text);
			return true;
        }
        else
        {
            printf("No node is fetched.\n");
			return false;
        }
	}
	catch (_com_error errorObject)
	{
		printf("Exception thrown, HRESULT: 0x%08x", errorObject.Error());
	}
	//std::cout << "xml,xpath, desc:" << xmlfile << xpath << desc << std::endl;
}





/*****************************************************************

bool getRB(TCHAR* xmlfile, TCHAR* xpath, TCHAR* desc)
{
	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	//desc[0] = 0;
	createDOM(pXMLDom);
	loadxml(pXMLDom, xmlfile);
	return getSubNote(pXMLDom, xpath, desc);
}

*******************************************************************/


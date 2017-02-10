#pragma once

#import <msxml6.dll>

bool getSubNode(TCHAR* xmlfile, TCHAR* xpath, TCHAR* desc);

void saveXML(MSXML2::IXMLDOMDocumentPtr pXMLDom, MSXML2::IXMLDOMElementPtr pRoot, TCHAR* filename);

void appendNoteElem(MSXML2::IXMLDOMDocumentPtr pXMLDom, MSXML2::IXMLDOMElementPtr pParent, TCHAR* attval, TCHAR* text);

#pragma once

class CMapgisArea {
	short MapgisArea;

public:
	CMapgisArea(short area);
	~CMapgisArea();
	void sort(TCHAR* mainkey, ...);
	void updaterouting();
	void updatecoord(TCHAR* x, TCHAR* y);
	void updateid();
};

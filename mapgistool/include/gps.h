//#include "stdafx.h"
#include <windows.h>
#include <string.h>
#include <ctime>
#include <iostream>
#include <win_area.h>

namespace dgs
{

	class Gps
	{
		double xx, yy, high;
		char t[20];
		char longitude[7];
		char latitude[6];

	public:
		Gps(double, double, double, double, double, time_t tm);
		~Gps(){};
		void writegps(short pntarea, long i, char* route);
		void printall();
		void printdate();
	};
}
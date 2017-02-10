#include "stdafx.h"
#include "gps.h"
#include <ctime>
#include <time.h>
#include <iostream>


//definitions of class Date

namespace dgs{
	//definitons of class Gps
	Gps::Gps(double x, double y, double z, double lt, double la, time_t tm)
		:xx(x), yy(y), high(z)
	{
			sprintf(longitude, "%.3f", lt);
			sprintf(latitude, "%.3f", la);
		//	std::cout << "longitude:" << lt << "latitude:" << la << std::endl;
			//longitude[sizeof(longitude) - 1] = '\0';
			//latitude[sizeof(latitude) - 1] = '\0';
			struct tm* ltm = localtime(&tm);
			strftime(t, sizeof(t), "%Y-%m-%d %H:%M:%S", ltm);
	}

	void Gps::writegps(short pnt, long i, char* route)
	{
		CATT_STRU* stru;
		char* att;

		_GetPntAtt(pnt, i, &stru, &att);
		_SetFld(att, stru, "XX", (char*) &xx);
		_SetFld(att, stru, "YY", (char*) &yy);
		_SetFld(att, stru, "HIGH", (char*) &high);
		_SetFld(att, stru, "LONGITUDE", longitude);
		_SetFld(att, stru, "LATITUDE", latitude);
		_SetFld(att, stru, "DATE", t);
		_SetFld(att, stru, "ROUTECODE", route);
		_WritePntAtt(pnt, i, stru, att);
	}

	void Gps::printall ()
	{
		using std::cout;
		using std::endl;

		cout << "xx=" << xx << ", yy=" << yy << ", high=" << high << endl;
		cout << "longitude=" << longitude << ", latitude=" << latitude << endl;
		cout << "date=" << t << endl << endl;

	}
	void Gps::printdate()
	{
		std::cout << "from printdate:" << t << std::endl;
	};
}

#pragma once
#include <stdint.h>
#include <time.h>
#include <string>

struct Time
{
	int Y, M, D;
	double h, m, s;
};


class JD
{
public :
	//公历转儒略日
	static double DD2JD(int y, uint8_t m, double d);
	static Time JD2DD(double jd);

	static  double toJD(Time& time);
	static std::string timeStr( double jd);
};

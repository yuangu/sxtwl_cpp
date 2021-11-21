#pragma once
#include <stdint.h>
#include <time.h>
#include <string>

struct Time
{
	// int Y, M, D;
	// double h, m, s;
	Time(){};
	Time(int year, int month, int day, double hour, double min, double sec){
		this->year = year; this->month = month; this->day = day;
		this->hour = hour; this-> min = min; this->sec = sec;
	}
	int year, month, day;
	double hour, min, sec;
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

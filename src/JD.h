#pragma once
#include <stdint.h>
#include <time.h>
#include <string>
#include <sstream>

struct Time
{
	 int Y, M, D;
	 double h, m, s;
	Time(){};
	Time(int year, int month, int day, double hour, double min, double sec){
		this->Y = year; this->M = month; this->D = day;
		this->h = hour; this->m = min; this->s = sec;
	}

	int getYear() {
		return Y;
	}

	void setYear(int year) {
		Y = year;
	}

	void setMonth(int month) {
		M = month;
	}

	int getMonth() {
		return M;
	}

	int getDay() {
		return D;
	}

	void setDay(int day) {
		D = day;
	}

	double getHour() {
		return h;
	}

	void setHour(double hour) {
		h = hour;
	}

	double getMin() {
		return m;
	}

	void setMour(double  min) {
		m = min;
	}


	double getSec() {
		return s;
	}

	void setSec(double  sec) {
		s = sec;
	}

	std::string toStr() {
		std::stringstream ss;
		ss << getYear() << "-" << getMonth() << "-" << getDay() << ' '
			<< getHour() << ":" << getMin() << ":" << (int)getSec();
		return ss.str();
	}
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

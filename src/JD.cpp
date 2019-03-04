#include "JD.h"
#include "const.h"
#include <cstring>
#include <cstdio>


//https://github.com/guolisen/YiEngine/blob/2ce67dc91fd5fea8e394a5af60dc1e56c5044452/src/DateTime/JulianDay.cpp
//公历转儒略日
double JD::DD2JD(int y, uint8_t m, double d)
{
 	int n = 0, G = 0;
	//判断是否为格里高利历日1582*372+10*31+15
	if (y * 372 + m * 31 + (int)(d) >= 588829)
	{
		G = 1;
	}
	if (m <= 2)
	{
		m += 12, y--;
	}
	//加百年闰
	if (G)
	{
		n = int2(y / 100), n = 2 - n + int(n / 4);
	}

	return int2(365.25*(y + 4716)) + int2(30.6001*(m + 1)) + d + n - 1524.5;
	
}

//儒略日数转公历
Time JD::JD2DD(double jd)
{
	Time r;
	int D = int2(jd + 0.5);
	float F = jd + 0.5 - D, c;  //取得日数的整数部份A及小数部分F
	if (D >= 2299161)
	{
		c = int((D - 1867216.25) / 36524.25), D += 1 + c - int2(c / 4);
	}
	D += 1524;               r.Y = int2((D - 122.1) / 365.25);//年数
	D -= int2(365.25*r.Y);   r.M = int2(D / 30.601); //月数
	D -= int2(30.601*r.M);   r.D = D; //日数
	if (r.M > 13)
	{
		r.M -= 13, r.Y -= 4715;
	}
	else
	{
		r.M -= 1, r.Y -= 4716;
	}
	//日的小数转为时分秒
	F *= 24; r.h = int2(F); F -= r.h;
	F *= 60; r.m = int2(F); F -= r.m;
	F *= 60; r.s = F;
	return r;
}

double JD::toJD(Time& time)
{
	return JD::DD2JD(time.Y, time.M, time.D + ((time.s / 60 + time.m) / 60 + time.h) / 24);
}

//提取jd中的时间(去除日期);
std::string JD::timeStr(double jd)
{
	int h, m, s;
	jd += 0.5; jd = (jd - int2(jd));
	s = int2(jd * 86400 + 0.5);
	h = int2(s / 3600); s -= h * 3600;
	m = int2(s / 60);   s -= m * 60;
	std::string ret = "";
	char buff[11];
	memset(buff, 0, 11);
	sprintf(buff, "0%d", h);
	ret.append(buff + strlen(buff) - 2);
	ret += ":";

	memset(buff, 0, 11);
	sprintf(buff, "0%d", m);
	ret.append(buff + strlen(buff) - 2);
	ret += ":";

	memset(buff, 0, 11);
	sprintf(buff, "0%d", s);
	ret.append(buff + strlen(buff) - 2);

	return ret;
}

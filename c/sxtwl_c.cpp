#include "sxtwl_c.h"

#include "sxtwl.h"


extern "C"
{
	sxtwl_Day* sxtwl_after(sxtwl_Day* day, sxtwl_Day* ret, int day_num)
	{
		ret->ptr = (Day*)((Day*)day->ptr)->after(day_num);
		return ret;
	}
	
	sxtwl_Day* sxtwl_before(sxtwl_Day* day, sxtwl_Day* ret, int day_num)
	{
		ret->ptr = (Day*)((Day*)day->ptr)->before(day_num);
		return ret;
	}

	int sxtwl_getLunarDay(sxtwl_Day* day)
	{
		return ((Day*)day->ptr)->getLunarDay();
	}

	int  sxtwl_getLunarYear(sxtwl_Day* day, bool chineseNewYearBoundary)
	{
		return ((Day*)day->ptr)->getLunarYear(chineseNewYearBoundary);
	}

	sxtwl_GZ  sxtwl_getYearGZ(sxtwl_Day* day, bool chineseNewYearBoundary)
	{
		GZ gz = ((Day*)day->ptr)->getYearGZ(chineseNewYearBoundary);
		sxtwl_GZ ret{gz.tg, gz.dz};
		return ret;
	}

	sxtwl_GZ  sxtwl_getMonthGZ(sxtwl_Day* day)
	{
		GZ gz = ((Day*)day->ptr)->getMonthGZ();
		sxtwl_GZ ret{ gz.tg, gz.dz };
		return ret;
	}

	sxtwl_GZ  sxtwl_getDayGZ(sxtwl_Day* day)
	{
		GZ gz = ((Day*)day->ptr)->getDayGZ();
		sxtwl_GZ ret{ gz.tg, gz.dz };
		return ret;
	}

	sxtwl_GZ  sxtwl_getHourGZ(sxtwl_Day* day, uint8_t hour, bool isZaoWanZiShi)
	{
		GZ gz = ((Day*)day->ptr)->getHourGZ(hour, isZaoWanZiShi);
		sxtwl_GZ ret{ gz.tg, gz.dz };
		return ret;
	}

	bool   sxtwl_isLunarLeap(sxtwl_Day* day)
	{
		return ((Day*)day->ptr)->isLunarLeap();
	}

	int  sxtwl_getSolarYear(sxtwl_Day* day)
	{
		return ((Day*)day->ptr)->getSolarYear();
	}

	uint8_t   sxtwl_getSolarMonth(sxtwl_Day* day)
	{
		return ((Day*)day->ptr)->getSolarMonth();
	}

	int  sxtwl_getSolarDay(sxtwl_Day* day)
	{
		return ((Day*)day->ptr)->getSolarDay();
	}

	uint8_t  sxtwl_getWeek(sxtwl_Day* day)
	{
		return ((Day*)day->ptr)->getWeek();
	}

	// 处于该月的第几周
	uint8_t  sxtwl_getWeekIndex(sxtwl_Day* day)
	{
		return ((Day*)day->ptr)->getWeekIndex();
	}

	//是否有节气
	bool  sxtwl_hasJieQi(sxtwl_Day* day)
	{
		return ((Day*)day->ptr)->hasJieQi();
	}

	// 获取节气
	uint8_t  sxtwl_getJieQi(sxtwl_Day* day)
	{
		return ((Day*)day->ptr)->getJieQi();
	}

	double  sxtwl_getJieQiJD(sxtwl_Day* day)
	{
		return ((Day*)day->ptr)->getJieQiJD();
	}

	// 获取星座
	uint8_t   sxtwl_getConstellation(sxtwl_Day* day)
	{
		return ((Day*)day->ptr)->getConstellation();
	}

	void sxtwl_freeDay(sxtwl_Day* day)
	{
		delete ((Day*)day->ptr);
	}

	sxtwl_GZ  sxtwl_getShiGz(uint8_t dayTg, uint8_t hour, bool isZaoWanZiShi)
	{
		GZ gz = sxtwl::getShiGz(dayTg, hour, isZaoWanZiShi);
		sxtwl_GZ ret{ gz.tg, gz.dz };
		return ret;
	}

	uint8_t  sxtwl_getRunMonth(int By)
	{
		return sxtwl::getRunMonth(By);
	}

	uint8_t sxtwl_getLunarMonthNum(int By, uint8_t month, bool isRun)
	{
		return sxtwl::getLunarMonthNum(By, month, isRun);
	}

	sxtwl_Time sxtwl_JD2DD(double jd)
	{
		Time t = sxtwl::JD2DD(jd);
		sxtwl_Time time;
		time.year = t.Y;
		time.month = t.M;
		time.day = t.D;

		time.hour = t.h;
		time.min = t.m;
		time.sec = t.s;
		return time;
	}

	double sxtwl_toJD(sxtwl_Time& time)
	{	
		Time t(time.year, time.month, time.day, time.hour, time.min, time.sec);
		return sxtwl::toJD(t);
	}

	sxtwl_Day* sxtwl_fromSolar(sxtwl_Day* ret, int year, uint8_t month, int day)
	{
		ret->ptr = (void*) sxtwl::fromSolar(year, month, day);
		return ret;
	}

	sxtwl_Day* sxtwl_fromLunar(sxtwl_Day* ret, int year, uint8_t month, int day, bool isRun)
	{
		ret->ptr = (void*)sxtwl::fromLunar(year, month, day, isRun);
		return ret;
	}

	double* sxtwl_siZhu2Year(double** list, int& ret_num, sxtwl_GZ year, sxtwl_GZ  yue, sxtwl_GZ  ri, sxtwl_GZ  shi, int fromYear, int  toYear)
	{
		auto  ret = sxtwl::siZhu2Year(GZ(year.tg, year.dz), GZ(yue.tg, yue.dz), GZ(ri.tg, ri.dz), GZ(shi.tg, shi.dz), fromYear, toYear);
		*list = (double*)malloc(ret.size() * sizeof(double));
		memcpy(*list, &(ret[0]), ret.size() * sizeof(double));
		return *list;
	}
}
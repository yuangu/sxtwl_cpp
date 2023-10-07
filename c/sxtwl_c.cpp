#include "sxtwl_c.h"
#include "sxtwl.h"

extern "C"
{
	sxtwl_Day *sxtwl_after(sxtwl_Day *day, sxtwl_Day *ret, int day_num)
	{
		ret->ptr = (Day *)((Day *)day->ptr)->after(day_num);
		return ret;
	}

	sxtwl_Day *sxtwl_before(sxtwl_Day *day, sxtwl_Day *ret, int day_num)
	{
		ret->ptr = (Day *)((Day *)day->ptr)->before(day_num);
		return ret;
	}

	int sxtwl_getLunarDay(sxtwl_Day *day)
	{
		return ((Day *)day->ptr)->getLunarDay();
	}

	int sxtwl_getLunarYear(sxtwl_Day *day, bool chineseNewYearBoundary)
	{
		return ((Day *)day->ptr)->getLunarYear(chineseNewYearBoundary);
	}

	sxtwl_GZ *sxtwl_getYearGZ(sxtwl_Day *day, sxtwl_GZ *ret, bool chineseNewYearBoundary)
	{
		GZ gz = ((Day *)day->ptr)->getYearGZ(chineseNewYearBoundary);
		ret->dz = gz.dz;
		ret->tg = gz.tg;
		return ret;
	}

	sxtwl_GZ *sxtwl_getMonthGZ(sxtwl_Day *day, sxtwl_GZ *ret)
	{
		GZ gz = ((Day *)day->ptr)->getMonthGZ();
		ret->dz = gz.dz;
		ret->tg = gz.tg;
		return ret;
	}

	sxtwl_GZ *sxtwl_getDayGZ(sxtwl_Day *day, sxtwl_GZ *ret)
	{
		GZ gz = ((Day *)day->ptr)->getDayGZ();
		ret->dz = gz.dz;
		ret->tg = gz.tg;
		return ret;
	}

	sxtwl_GZ *sxtwl_getHourGZ(sxtwl_Day *day, sxtwl_GZ *ret, uint8_t hour, bool isZaoWanZiShi)
	{
		GZ gz = ((Day *)day->ptr)->getHourGZ(hour, isZaoWanZiShi);
		ret->dz = gz.dz;
		ret->tg = gz.tg;
		return ret;
	}

	bool sxtwl_isLunarLeap(sxtwl_Day *day)
	{
		return ((Day *)day->ptr)->isLunarLeap();
	}

	int sxtwl_getSolarYear(sxtwl_Day *day)
	{
		return ((Day *)day->ptr)->getSolarYear();
	}

	uint8_t sxtwl_getSolarMonth(sxtwl_Day *day)
	{
		return ((Day *)day->ptr)->getSolarMonth();
	}

	int sxtwl_getSolarDay(sxtwl_Day *day)
	{
		return ((Day *)day->ptr)->getSolarDay();
	}

	uint8_t sxtwl_getWeek(sxtwl_Day *day)
	{
		return ((Day *)day->ptr)->getWeek();
	}

	// 处于该月的第几周
	uint8_t sxtwl_getWeekIndex(sxtwl_Day *day)
	{
		return ((Day *)day->ptr)->getWeekIndex();
	}

	//是否有节气
	bool sxtwl_hasJieQi(sxtwl_Day *day)
	{
		return ((Day *)day->ptr)->hasJieQi();
	}

	// 获取节气
	uint8_t sxtwl_getJieQi(sxtwl_Day *day)
	{
		return ((Day *)day->ptr)->getJieQi();
	}

	double sxtwl_getJieQiJD(sxtwl_Day *day)
	{
		return ((Day *)day->ptr)->getJieQiJD();
	}

	// 获取星座
	uint8_t sxtwl_getConstellation(sxtwl_Day *day)
	{
		return ((Day *)day->ptr)->getConstellation();
	}

	void sxtwl_freeDay(sxtwl_Day *day)
	{
		delete ((Day *)day->ptr);
	}

	sxtwl_GZ *sxtwl_getShiGz(uint8_t dayTg, sxtwl_GZ *ret, uint8_t hour, bool isZaoWanZiShi)
	{
		GZ gz = sxtwl::getShiGz(dayTg, hour, isZaoWanZiShi);
		ret->dz = gz.dz;
		ret->tg = gz.tg;
		return ret;
	}

	uint8_t sxtwl_getRunMonth(int By)
	{
		return sxtwl::getRunMonth(By);
	}

	uint8_t sxtwl_getLunarMonthNum(int By, uint8_t month, bool isRun)
	{
		return sxtwl::getLunarMonthNum(By, month, isRun);
	}

	sxtwl_Time *sxtwl_JD2DD(double jd, sxtwl_Time *ret)
	{
		Time t = sxtwl::JD2DD(jd);

		ret->year = t.Y;
		ret->month = t.M;
		ret->day = t.D;

		ret->hour = t.h;
		ret->min = t.m;
		ret->sec = t.s;
		return ret;
	}

	double sxtwl_toJD(sxtwl_Time *time)
	{
		Time t(time->year, time->month, time->day, time->hour, time->min, time->sec);
		return sxtwl::toJD(t);
	}

	sxtwl_Day *sxtwl_fromSolar(sxtwl_Day *ret, int year, uint8_t month, int day)
	{
		ret->ptr = (void *)sxtwl::fromSolar(year, month, day);
		return ret;
	}

	sxtwl_Day *sxtwl_fromLunar(sxtwl_Day *ret, int year, uint8_t month, int day, bool isRun)
	{
		ret->ptr = (void *)sxtwl::fromLunar(year, month, day, isRun);
		return ret;
	}

	double *sxtwl_siZhu2Year(double **list, int &ret_num, sxtwl_GZ *year, sxtwl_GZ *yue, sxtwl_GZ *ri, sxtwl_GZ *shi, int fromYear, int toYear)
	{
		auto ret = sxtwl::siZhu2Year(GZ(year->tg, year->dz), GZ(yue->tg, yue->dz), GZ(ri->tg, ri->dz), GZ(shi->tg, shi->dz), fromYear, toYear);
		*list = (double *)malloc(ret.size() * sizeof(double));
		memcpy(*list, &(ret[0]), ret.size() * sizeof(double));
		return *list;
	}

	sxtwl_GZ *sxtwl_newGZ()
	{
		return (sxtwl_GZ *)struct_malloc(sizeof(sxtwl_GZ));
	}

	void sxtwl_freeGZ(sxtwl_GZ *gz)
	{
		struct_free(gz);
	}

	uint8_t sxtwl_GZ_getTg(sxtwl_GZ *gz)
	{
		return gz->tg;
	}

	uint8_t sxtwl_GZ_getDz(sxtwl_GZ *gz)
	{
		return gz->dz;
	}

	void sxtwl_GZ_setTg(sxtwl_GZ *gz, uint8_t tg)
	{
		gz->tg = tg;
	}

	void sxtwl_GZ_setDz(sxtwl_GZ *gz, uint8_t dz)
	{
		gz->dz = dz;
	}

	sxtwl_Time *sxtwl_newTime()
	{
		return (sxtwl_Time *)struct_malloc(sizeof(sxtwl_Time));
	}

	void sxtwl_freeTime(sxtwl_Time *time)
	{
		struct_free(time);
	}

	int sxtwl_Time_getYear(sxtwl_Time *time)
	{
		return time->year;
	}

	int sxtwl_Time_getMonth(sxtwl_Time *time)
	{
		return time->month;
	}

	int sxtwl_Time_getDay(sxtwl_Time *time)
	{
		return time->day;
	}

	void sxtwl_Time_setYear(sxtwl_Time *time, int value)
	{
		time->year = value;
	}
	void sxtwl_Time_setMonth(sxtwl_Time *time, int value)
	{
		time->month = value;
	}
	void sxtwl_Time_setDay(sxtwl_Time *time, int value)
	{
		time->day = value;
	}

	double sxtwl_Time_getHour(sxtwl_Time *time)
	{
		return time->hour;
	}

	double sxtwl_Time_getMin(sxtwl_Time *time)
	{
		return time->min;
	}

	double sxtwl_Time_getSec(sxtwl_Time *time)
	{
		return time->sec;
	}

	void sxtwl_Time_setHour(sxtwl_Time *time, double value)
	{
		time->hour = value;
	}

	CC_EX_DLL void sxtwl_Time_setMin(sxtwl_Time *time, double value)
	{
		time->min = value;
	}

	CC_EX_DLL void sxtwl_Time_setSec(sxtwl_Time *time, double value)
	{
		time->sec = value;
	}

	//=====================================================
	void *struct_malloc(int size)
	{
		return malloc((size_t)size);
	}

	void struct_free(void *ptr)
	{
		free(ptr);
	}
	//=====================================================
}

#ifdef __EMSCRIPTEN__
int main()
{
	return 0;
}
#endif
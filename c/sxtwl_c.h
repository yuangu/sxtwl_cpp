#ifndef _SXTWL_C_H_
#define _SXTWL_C_H_

#define _USREXDLL 1

#if (defined(WIN32) && defined(_WINDOWS)) || defined(WINRT) || defined(WP8)
#ifdef __MINGW32__
#include <string.h>
#endif

#if defined(CC_STATIC)
#define CC_EX_DLL
#else
#if defined(_USREXDLL)
#define CC_EX_DLL     __declspec(dllexport)
#else         /* use a DLL library */
#define CC_EX_DLL     __declspec(dllimport)
#endif
#endif

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif
#elif defined(_SHARED_)
#define CC_EX_DLL     __attribute__((visibility("default")))
#else
#define CC_EX_DLL
#endif


#include <stdint.h>

extern "C"
{
	typedef struct
	{
		int year, month, day;
		double hour, min, sec;
	} sxtwl_Time;

	typedef struct 
	{
		uint8_t tg;
		uint8_t dz;
	}  sxtwl_GZ;

	typedef struct
	{
		void* ptr;
	} sxtwl_Day;


	// 注意返回的sxtwl_Day指针，需要使用sxtwl_freeDay释放
	CC_EX_DLL sxtwl_Day* sxtwl_after(sxtwl_Day* day, sxtwl_Day* ret, int day_num);
	CC_EX_DLL sxtwl_Day* sxtwl_before(sxtwl_Day* day, sxtwl_Day* ret, int day_num);

	CC_EX_DLL int sxtwl_getLunarDay(sxtwl_Day* day);
	CC_EX_DLL int  sxtwl_getLunarYear(sxtwl_Day* day, bool chineseNewYearBoundary);
	CC_EX_DLL sxtwl_GZ  sxtwl_getYearGZ(sxtwl_Day* day, bool chineseNewYearBoundary);
	CC_EX_DLL sxtwl_GZ  sxtwl_getMonthGZ(sxtwl_Day* day);
	CC_EX_DLL sxtwl_GZ  sxtwl_getDayGZ(sxtwl_Day* day);
	// 注意非早晚子时的时候，day要算第二天
	CC_EX_DLL sxtwl_GZ  sxtwl_getHourGZ(sxtwl_Day* day, uint8_t hour, bool isZaoWanZiShi = true);
	CC_EX_DLL bool   sxtwl_isLunarLeap(sxtwl_Day* day);

	CC_EX_DLL int  sxtwl_getSolarYear(sxtwl_Day* day);
	CC_EX_DLL uint8_t   sxtwl_getSolarMonth(sxtwl_Day* day);
	CC_EX_DLL int  sxtwl_getSolarDay(sxtwl_Day* day);
	CC_EX_DLL uint8_t  sxtwl_getWeek(sxtwl_Day* day);
	// 处于该月的第几周
	CC_EX_DLL uint8_t  sxtwl_getWeekIndex(sxtwl_Day* day);
	//是否有节气
	CC_EX_DLL bool  sxtwl_hasJieQi(sxtwl_Day* day);
	// 获取节气
	CC_EX_DLL uint8_t  sxtwl_getJieQi(sxtwl_Day* day);
	CC_EX_DLL double  sxtwl_getJieQiJD(sxtwl_Day* day);
	// 获取星座
	CC_EX_DLL uint8_t   sxtwl_getConstellation(sxtwl_Day* day);
	CC_EX_DLL void sxtwl_freeDay(sxtwl_Day* day);


	CC_EX_DLL sxtwl_Day* sxtwl_fromSolar(sxtwl_Day* ret, int year, uint8_t month, int day);

	CC_EX_DLL sxtwl_Day* sxtwl_fromLunar(sxtwl_Day* ret, int year, uint8_t month, int day, bool isRun);
	
	//通过四柱获取年月日, 返回的是儒略日列表
	CC_EX_DLL double* sxtwl_siZhu2Year(double** list, int& ret_num,  sxtwl_GZ year, sxtwl_GZ  yue, sxtwl_GZ  ri, sxtwl_GZ  shi, int fromYear, int  toYear);
	////获取时辰上的那个天干
	CC_EX_DLL sxtwl_GZ sxtwl_getShiGz(uint8_t dayTg, uint8_t hour, bool isZaoWanZiShi);
	//获取一年中的润月(不存，则返回0)
	CC_EX_DLL uint8_t sxtwl_getRunMonth(int By);
	//获取一月中的阴日数量
	CC_EX_DLL uint8_t sxtwl_getLunarMonthNum(int By, uint8_t month, bool isRun);
	//儒略日数转公历
	CC_EX_DLL sxtwl_Time sxtwl_JD2DD(double jd);
	//公历转儒略日
	CC_EX_DLL double sxtwl_toJD(sxtwl_Time &time);
}
#endif
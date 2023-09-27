%module sxtwl

%{
#include "const.h"
#include "sxtwl.h"
%}

%include "stdint.i"
%include "std_vector.i"
%include "std_string.i"

namespace std{
    %template(JDList) vector<double>;
    %template(JQList) vector<sxtwl::JieQiInfo>;
}

%constant int J2000=2451545;

// %catches(LunarException) Lunar::getLunarMonthNum(int By, uint8_t month, bool isRun = false);
// %catches(LunarException) Lunar::getDayBySolar(int year, uint8_t month, uint8_t day);
// %catches(LunarException) Lunar::getDayByLunar(int year, uint8_t month, uint8_t day, bool isRun = false);


struct Time
{
    Time();
	Time(int year, int month, int day, double hour, double min, double sec);
	int Y, M, D;
	double h, m, s;

//=========================================================
//time的操作方法
    int getYear();
	void setYear(int year);
	void setMonth(int month) ;
	int getMonth() ;
	int getDay() ;
	void setDay(int day);
	double getHour();
	void setHour(double hour);
	double getMin() ;
	void setMour(double  min);
	double getSec();
	void setSec(double  sec);
//=========================================================
};


struct GZ
{
    GZ(){};
    GZ(uint8_t tg, uint8_t dz) : tg(tg), dz(dz)
    {
    }
    uint8_t tg;
    uint8_t dz;
};

class Day
{
public:
   static Day *fromSolar(int _year, uint8_t _month, int _day);
   static Day *fromLunar(int year, uint8_t month, int day, bool isRun = false);
private:
    Day(int d0);
public:
    Day *after(int day);
    Day *before(int day);
    // 获取阴历日期
    int getLunarDay();
    //获取阴历月
    uint8_t getLunarMonth();
    //获取阴历年 chineseNewYearBoundary是否以春节为界
    int getLunarYear(bool chineseNewYearBoundary = true);
    //获取阴历年干支 chineseNewYearBoundary是否以春节为界
    GZ getYearGZ(bool chineseNewYearBoundary = false);
    GZ getMonthGZ();
    GZ getDayGZ();
    GZ getHourGZ(uint8_t hour, bool isZaoWanZiShi = true);
    bool  isLunarLeap();

    int getSolarYear();
    uint8_t  getSolarMonth();
    int getSolarDay() ;
    uint8_t getWeek();
    // 处于该月的第几周
    uint8_t getWeekIndex();
    //是否有节气
    bool hasJieQi();
    // 获取节气
    uint8_t getJieQi() ;
    double getJieQiJD();
    // 获取星座
    uint8_t  getConstellation();
};

namespace sxtwl
{
    struct JieQiInfo
    {
        double jd;  //节气的儒略日
        uint8_t jqIndex; //节气索引
    };

	Day *fromSolar(int year, uint8_t month, int day);
	Day *fromLunar(int year, uint8_t month, int day, bool isRun = false);
	//通过四柱获取年月日
	std::vector<double> siZhu2Year(GZ year, GZ  yue, GZ  ri, GZ  shi, int fromYear, int  toYear);
	//获取时辰上的那个天干
    GZ  getShiGz(uint8_t dayTg,  uint8_t hour, bool isZaoWanZiShi = true);
	//获取一年中的润月(不存，则返回0)
	uint8_t getRunMonth(int By);
	//获取一月中的阴日数量 
	uint8_t getLunarMonthNum(int By, uint8_t month, bool isRun = false);
    //儒略日数转公历
	Time JD2DD(double jd);
	//公历转儒略日
	double toJD(Time& time);

    std::vector<JieQiInfo> getJieQiByYear(int year);
};
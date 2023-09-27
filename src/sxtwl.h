#pragma once
#include <stdint.h>

#include "SSQ.h"
#include <functional>
#include "day.h"

namespace sxtwl
{
	struct JieQiInfo
	{
		double jd;  //节气的儒略日
		uint8_t jqIndex; //节气索引
	};


	Day *fromSolar(int year, uint8_t month, int day);
	Day *fromLunar(int year, uint8_t month, int day, bool isRun = false);
	//通过四柱获取年月日, 返回的是儒略日列表
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
	//获取某年的节气（立春到立春）
	std::vector<JieQiInfo> getJieQiByYear(int year);
};

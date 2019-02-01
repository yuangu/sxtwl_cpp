#pragma once
#include <stdint.h>
#include "SSQ.h"
#include "JD.h"
#include <ctime>

struct  GZ
{
	uint8_t tg;
	uint8_t dz;
};

struct Day
{
	int d0;   //儒略日
	int di; //公历月内日序数
	int y;   //公历年
	uint8_t m; //公历月
	int d; 
	int dn;   //所在公历月的天数
	uint8_t week0; //月首的星期
	uint8_t week;  //当前日的星期数
	uint8_t weeki; //本日所在的周序号
	uint8_t weekN; //本月的总周数

	uint8_t Ldi;  //距农历月首的编移量,0对应初一
	int cur_dz;  //距冬至的天数
	int cur_xz; //距夏至的天数
	int cur_lq;//距立秋的天数
	int cur_mz ;  //距芒种的天数
	int cur_xs;  //距小暑的天数
    
    std::vector<long double> cur_jq;
    std::vector<time_t> cur_jq_cutoff_datetime;

    
    
	uint8_t Lmc;  //阴历月的月份
	uint8_t Ldn;  //阴历月的天数
	bool Lleap; //是不是阴历的润月
	int8_t Lmc2; //下个月的月份，不存在则为-1

	int8_t qk;  //节气，不存在则为-1

	int Lyear;  //农历纪年(10进制,1984年起算)
	int Lyear0;  //农历纪年(10进制,1984年起算)
	GZ Lyear2; //干支纪年(立春)
	GZ Lyear3; //干支纪年(正月)
	int Lyear4;

	uint8_t Lmonth;
	GZ Lmonth2;   //月天干地支

	GZ Lday2;

	uint8_t XiZ; //星座
	uint8_t jqmc; //节气类型
	uint8_t jqjd;
	std::string jqsj;  //节气时间

	uint8_t yxmc; //月像类型
	uint8_t yxjd;
	std::string yxsj; //月像时间
};

struct Month
{
	int y; //公历年份
	uint8_t m; //公历月分
	//年生肖
	uint8_t ShX;
	uint8_t yearGan;
	uint8_t yearZhi;
	std::vector<Day> days;
};

struct Year
{
    int y; //公历年份
    //年生肖
    uint8_t ShX;
    uint8_t yearGan;
    uint8_t yearZhi;
};

class Lunar
{
public:
	//获取一个月的信息
	Month yueLiCalc(int By, uint8_t Bm);
    //通过阳历获取Day对像
    Day getDayBySolar(int year, uint8_t month, uint8_t day);
    //通过阴历获取Day对像
    Day getDayByLunar(int year, uint8_t month, uint8_t day, bool isRun = false);
	//获取时辰上的那个天干
    GZ  getShiGz(uint8_t dayTg,  uint8_t hour);
    //获取一年的信息
    Year getYearCal(int By);
private:
	SSQ mSSQ;
};

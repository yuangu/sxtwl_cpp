#pragma once
#include <stdint.h>
#include "lunar.h"
#include "SSQ.h"

struct DayInfo
{
    int d0;   //那日所在的月的首日儒略日
	int di;   //公历月内日序数

    int y;   //公历年
	int m;   //公历月
	int d;   //公历日

    int ly; //阴历年(以立春为界)
    int lm; //阴历月
    int ld; //阴历日
    bool isRun; //是不是润月

    
	bool hasJq; //此日是否是节气
	int jqIndex; //节气的索引
	double jqjd;  //节气的儒略日
};

class SXDay
{
    public:
        void  initBySolar(int year, uint8_t month, uint8_t day);
        void  initByLunar(int year, uint8_t month, uint8_t day, bool isRun = false);

        //阳历信息
        int getYear();
        int getMonth();
        int getDay();

        //阴历信息
        int getLYear();
        int getLMonth();
        int getLDay();
        bool isRun();
        
        //获取今天是星期几
        int getWeek();
		//是否有节气信息
		bool hasQJ();

        //节气类型
        int getJQIndex();
        //节气儒略日
		double getJQJD();
       

    private:
        void lunar(int di);
        void lunarYear();
        void reset();

    private:
	    SSQ mSSQ;
	
	private:
		bool hasLunar;  //是否初使化阴历
		bool hasLunarYear;//是否初使化了阴历年
		bool hasCheckJQ; //是否判断过了，有节气？
		bool hasGetJQJd; //是否计算过节气的时间
		bool hasGetJQIndex; 
    private:
        DayInfo mDayInfo;
};



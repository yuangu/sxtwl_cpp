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
    bool isRun;

    int8_t qk;  //节气，不存在则为-1

    void reset();
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

        //节气类型
        int getJQIndex();
        //节气儒略日（精确到日）
        int getJQJD();
        //精确节气的儒略日
        int getJQExactJD();

    private:
        void lunar(int di);
        void lunarYear();
        void reset();

    private:
	    SSQ mSSQ;

    private:
        DayInfo mDayInfo;
};



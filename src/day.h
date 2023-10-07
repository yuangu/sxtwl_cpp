#pragma once

#include <stdint.h>
#include <memory>
#include "JD.h"
#include "const.h"
#include "SSQ.h"

static std::unique_ptr<SSQ> SSQPtr(new SSQ());

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
private:
    int d0; //儒略日

    int y;     //公历年
    uint8_t m; //公历月
    int d;

    int Lmc;     //阴历月的月
    uint8_t Ldi; //阴历月的日
    int Ldn;     //该阴历月的总天数
    bool Lleap;  //是不是阴历的润月

    int Lyear;  //以立春为界， 定农历纪年(10进制,1984年起算)
    int Lyear0; //以正月初一为界，农历纪年(10进制,1984年起算)

    uint8_t week; //星期几
    int8_t qk;    //节令值
    uint8_t XiZ;  //星座
    double jqjd; //节气最体的时间

    GZ *Lyear2;  //干支纪年(立春)
    GZ *Lyear3;  //干支纪年(正月 春节)
    GZ *Lmonth2; //月天干地支
    GZ *Lday2;   // 日天干地支

private:
    Day(int d0)
    {
        this->d0 = d0;
        this->Ldn = 0;
        this->m = 0;
        this->qk = -2;
        this->Lyear = 0;
        this->Lyear0 = 0;

        this->Lyear2 = NULL;
        this->Lyear3 = NULL;
        this->Lmonth2 = NULL;
        this->Lday2 = NULL;

        this->week = 0xFF;
        this->XiZ = 0xFF;
        this->jqjd = 0;
    };

    void checkSSQ();
    void checkLunarData();
    void checkSolarData();
    void checkJQData();

public:
    virtual ~Day()
    {
        if (this->Lyear2)
        {
            delete this->Lyear2;
            this->Lyear2 = NULL;
        }

        if (this->Lyear3)
        {
            delete this->Lyear3;
            this->Lyear3 = NULL;
        }

        if (this->Lmonth2)
        {
            delete this->Lmonth2;
            this->Lmonth2 = NULL;
        }

        if (this->Lday2)
        {
            delete this->Lday2;
            this->Lday2 = NULL;
        }
    }

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
    // 注意非早晚子时的时候，day要算第二天
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
public:
    static Day *fromSolar(int _year, uint8_t _month, int _day)
    {
        Time *t = new Time();
        t->h = 12, t->m = 0, t->s = 0.1;
        t->Y = _year;
        t->M = _month;
        t->D = _day;
        int d0 = int2(JD::toJD(*t)) - J2000;
        return new Day(d0);
    }

    static Day *fromLunar(int year, uint8_t month, int day, bool isRun = false)
    {
        Time *t = new Time();
        t->h = 12, t->m = 0, t->s = 0.1;
        t->Y = year;
        t->M = 1;
        t->D = 1;
        if (month > 10)
        {
            t->Y = year + 1;
        }

        int Bd0 = int2(JD::toJD(*t)) - J2000;
        if (!SSQPtr->ZQ.size() || Bd0 < SSQPtr->ZQ[0] || Bd0 >= SSQPtr->ZQ[24])
        {
            SSQPtr->calcY(Bd0);
        }

        static const int yueIndex[12] =  {11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        int yue = 0;

        for (int i = 0; i < sizeof(yueIndex); ++i)
        {
            if (yueIndex[i] == month)
            {
                yue = i;
                break;
            }
        }

        int mk = 0;
        int leap = SSQPtr->leap - 1;

        for (int i = 0; i < SSQPtr->ym.size(); ++i)
        {
            int it = SSQPtr->ym[i];
            if (leap < 0)
            {
                if (it == yue)
                {
                    break;
                }
            }
            else
            {
                if (yue < leap && it == yue)
                {
                    break;
                }

                if (yue == leap && it == yue && isRun)
                {
                    ++mk;
                    break;
                }

                if (yue == leap && it == yue && !isRun)
                {
                    break;
                }

                if (yue > leap && it == yue)
                {
                    break;
                }
            }
            ++mk;
        }

        int bdi = SSQPtr->HS[mk];
        int jd = bdi + day - 1;

        return new Day(jd);
    }
};

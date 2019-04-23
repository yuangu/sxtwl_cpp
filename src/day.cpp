#include "day.h"
#include "JD.h"
#include "const.h"

void DayInfo::reset()
{
    y = -1;
    m = -1;
    d = -1;
}

void SXDay::initBySolar(int _year, uint8_t _month, uint8_t _day)
{
    //公元记年法没有0年，公元从1年开始，公元前从-1开始
    if(_year == 0)
    {
        throw LunarException(ErrorCode_DateError);
    }

    if ((_month > 12 || _month <= 0))
    {
        throw LunarException(ErrorCode_DateError);
    }

    static int num[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (_day <= 0 || _day > num[_month])
    {
        int year = _year;

        //公元前润年
        if (year < 0)
        {
             year++;     
        }
                                                       
        if (!(year % 4 == 0 && _month == 2 && _day == 29) ) 
        {
            throw LunarException(ErrorCode_DateError);
        }
    }


    //公历月首的儒略日,中午;
    Time t;
	t.h = 12, t.m = 0, t.s = 0.1;
	t.Y = _year; t.M = _month; t.D = 1;
    
    mDayInfo.d0 = int2(JD::toJD(t)) - J2000;
    mDayInfo.di =  mDayInfo.d0 + _day - 1;
    // //计算公历月的时长
    // ++t.M;
    // if (t.M > 12)
    // {
    //     ++t.Y;
    //     t.M = 1;
    // }

	// //本月天数(公历);
	// int Bdn = int2(JD::toJD(t)) - J2000 - mDayInfo.d0;
    int Bdn = num[_month];

    if (mSSQ.ZQ.size() <= 0 || mDayInfo.di < mSSQ.ZQ[0] || mDayInfo.di >= mSSQ.ZQ[24])
	{
		mSSQ.calcY(mDayInfo.di);
	}
}

void SXDay::initByLunar(int year, uint8_t month, uint8_t day, bool isRun)
{
    if (month > 10)
    {
        year = year + 1;
    }

    if (month > 12 || month < 0)
    {
        throw LunarException(ErrorCode_DateError);
    }

    //计算1月1号的信息
    Time t;
    t.h = 12, t.m = 0, t.s = 0.1;
    t.Y = year;
    t.M = 1;
    t.D = 1;

    //公历月首的儒略日,中午;
    int Bd0 = int2(JD::toJD(t)) - J2000;

    if (!mSSQ.ZQ.size() || Bd0 < mSSQ.ZQ[0] || Bd0 >= mSSQ.ZQ[24])
    {
        mSSQ.calcY(Bd0);
    }

    //{ "十一", "十二", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十" }
    // static int mkIndex[] = { 11, 12, 1,2,3,4,5,6,7, 8,9,10 };
    static int yueIndex[] = {11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int yue = 0;

    for (int i = 0; i < sizeof(yueIndex); ++i)
    {
        if (*(yueIndex + i) == month)
        {
            yue = i;
            break;
        }
    }

    int mk = 0;
    int leap = mSSQ.leap - 1;

    if (isRun && ((leap < 0) || (leap >= 0 && month != yueIndex[leap])))
    {
        throw LunarException(ErrorCode_NotRun);
    }

    for (auto it = mSSQ.ym.begin(); it != mSSQ.ym.end(); ++it)
    {

        if (leap < 0)
        {
            if (*it == yue)
            {
                break;
            }
        }
        else
        {
            if (yue < leap && *it == yue)
            {
                break;
            }

            if (yue == leap && *it == yue && isRun)
            {
                ++mk;
                break;
            }

            if (yue == leap && *it == yue && !isRun)
            {
                break;
            }

            if (yue > leap && *it == yue)
            {
                break;
            }
        }
        ++mk;
    }

    //阴历首月的儒略日
    int bdi = mSSQ.HS[mk];

    //mk会不会超出超出预期？
    if (mSSQ.HS[mk + 1] - mSSQ.HS[mk] < day || day <= 0)
    {
        throw LunarException(ErrorCode_DateError);
    }

    int jd = bdi + day - 1;

    t = JD::JD2DD(J2000 + jd);

    initBySolar(t.Y, t.M, t.D);
}

void SXDay::reset()
{
}

int SXDay::getYear()
{
    return mDayInfo.y;
}

int SXDay::getMonth()
{
    return mDayInfo.m;
}

int SXDay::getDay()
{
    return mDayInfo.d;
}

int SXDay::getLYear()
{
}

int SXDay::getLMonth()
{
}

int SXDay::getLDay()
{
    auto di = mDayInfo.di;
    int mk = int2((di - mSSQ.HS[0]) / 30);
    
    //农历所在月的序数;
    if (mk < 13 && mSSQ.HS[mk + 1] <= di)
    {
        mk++;
    }

    return di - mSSQ.HS[mk];   //距农历月首的编移量,0对应初一
}

bool SXDay::isRun()
{
}

int SXDay::getWeek()
{
    int w0 = (mDayInfo.d0 + J2000 + 1 + 7000000) % 7;
    return (w0 + mDayInfo.d) % 7; 
}

int SXDay::getJQIndex()
{
}

int SXDay::getJQJD()
{
}

int SXDay::getJQExactJD()
{
}

void SXDay::lunar(int di)
{
    //auto di = mDayInfo.di;
    int mk = int2((di - mSSQ.HS[0]) / 30);
    //农历所在月的序数;
    if (mk < 13 && mSSQ.HS[mk + 1] <= di)
    {
        mk++;
    }

	mDayInfo.ld = di - mSSQ.HS[mk];   //距农历月首的编移量,0对应初一
    if (di == mSSQ.HS[mk] || di == mDayInfo.d0) { //月的信息
		mDayInfo.lm = mSSQ.ym[mk]; //月名称
		mDayInfo.isRun   = (mSSQ.leap&&mSSQ.leap == mk); 
        //day.Lmc2 = mk < 13 ? mk + 1 : -1; //下个月名称,判断除夕时要用到
	}
	else 
    {
		lunar(di - 1);
	}
		//int qk = int2((day.d0 - mSSQ.ZQ[0] - 7) / 15.2184);
}

void SXDay::lunarYear()
{
    auto di = mDayInfo.di;
	int qk = int2((di - mSSQ.ZQ[0] - 7) / 15.2184);

    //节气的取值范围是0-23
    if (qk < 23 && di >= mSSQ.ZQ[qk + 1])
    {
        qk++;
    }

    mDayInfo.qk = -1;
    if (di == mSSQ.ZQ[qk])
    {
        di = qk;
    }

    auto D = mSSQ.ZQ[3] + (di < mSSQ.ZQ[3] ? -365 : 0) + 365.25 * 16 - 35; //以立春为界定纪年
	mDayInfo.ly = floor(D / 365.2422 + 0.5); //农历纪年(10进制,1984年起算)

}
#include "day.h"
#include "eph.h"

void Day::checkSSQ()
{
    if (!SSQPtr->ZQ.size() || this->d0 < SSQPtr->ZQ[0] || this->d0 >= SSQPtr->ZQ[24])
    {
        SSQPtr->calcY(this->d0);
    }
}

/**
     * 确定已经计算过阴历信息
     */
void Day::checkLunarData()
{
    // 已经计算过了
    if (this->Ldn != 0)
    {
        return;
    }
    this->checkSSQ();

    int mk = int2((this->d0 - SSQPtr->HS[0]) / 30);
    if (mk < 13 && SSQPtr->HS[mk + 1] <= this->d0)
    {
        mk++; //农历所在月的序数
    }

    //if (this.d0 == SSQPtr->HS[mk]) { //月的信息
    this->Lmc = SSQPtr->ym[mk];                              //月名称
    this->Ldn = SSQPtr->dx[mk];                              //月大小
    this->Lleap = (SSQPtr->leap != 0 && SSQPtr->leap == mk); //闰状况
    //}

    // 阴历所处的日
    this->Ldi = this->d0 - SSQPtr->HS[mk];
}

void Day::checkSolarData()
{
    if (this->m != 0)
    {
        return;
    }

    Time t = JD::JD2DD(this->d0 + J2000);
    this->y = t.Y;
    this->d = t.D;
    this->m = t.M;
}

/**
 * 计算节气数据
 */
void Day::checkJQData()
{
    if (this->qk != -2)
    {
        return;
    }
    this->checkSSQ();

    int qk = int2((this->d0 - SSQPtr->ZQ[0] - 7) / 15.2184);
    ////节气的取值范围是0-23
    if (qk < 23 && this->d0 >= SSQPtr->ZQ[qk + 1])
    {
        qk++;
    }

    this->qk = -1;
    if (this->d0 == SSQPtr->ZQ[qk])
    {
        this->qk = qk;
    }
}

Day *Day::after(int day)
{
    return new Day(this->d0 + day);
}

Day *Day::before(int day)
{
    return new Day(this->d0 - day);
}

/**
 * 获取阴历日期
 */
int Day::getLunarDay()
{
    this->checkLunarData();
    return this->Ldi + 1;
}

/**
     * 获取阴历月
     */
uint8_t Day::getLunarMonth()
{
    this->checkLunarData();
    static const int yueIndex[12] = {11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    return yueIndex[this->Lmc];
}

int Day::getLunarYear(bool chineseNewYearBoundary)
{
    // 以立春为界
    if (chineseNewYearBoundary == false)
    {
        if (this->Lyear == 0)
        {
            this->checkSSQ();
            long double D = SSQPtr->ZQ[3] + (this->d0 < SSQPtr->ZQ[3] ? -365 : 0) + 365.25 * 16 - 35; //以立春为界定纪年
            this->Lyear = int2(D / 365.2422 + 0.5);
        }
        return this->Lyear + 1984;
    }
    // 以春节为界
    if (this->Lyear0 == 0)
    {
        this->checkSSQ();
        int D = SSQPtr->HS[2]; //一般第3个月为春节
        for (int j = 0; j < 14; j++)
        { //找春节
            if (SSQPtr->ym[j] != 2 || SSQPtr->leap == j && j)
                continue;
            D = SSQPtr->HS[j];
            if (this->d0 < D)
            {
                D -= 365;
                break;
            } //无需再找下一个正月
        }
        D = D + 5810; //计算该年春节与1984年平均春节(立春附近)相差天数估计
        this->Lyear0 = int2(D / 365.2422 + 0.5);
    }
    return this->Lyear0 + 1984;
}

GZ Day::getYearGZ(bool chineseNewYearBoundary)
{
    //以春节为界
    if (chineseNewYearBoundary)
    {
        if (this->Lyear3 == NULL)
        {
            int year = this->getLunarYear(chineseNewYearBoundary) - 1984;
            int D = year + 12000;
            this->Lyear3 = new GZ(D % 10, D % 12);
        }
        return *(this->Lyear3);
    }

    // 以立春为界
    if (this->Lyear2 == NULL)
    {
        int year = this->getLunarYear(false) - 1984;
        int D = year + 12000;
        this->Lyear2 = new GZ(D % 10, D % 12);
    }
    return *(this->Lyear2);
}

GZ Day::getMonthGZ()
{
    if (this->Lmonth2 == NULL)
    {
        this->checkSSQ();
        int mk = int2((this->d0 - SSQPtr->ZQ[0]) / 30.43685);
        //相对大雪的月数计算,mk的取值范围0-12
        if (mk < 12 && this->d0 >= SSQPtr->ZQ[2 * mk + 1])
            mk++;
        //相对于1998年12月7(大雪)的月数,900000为正数基数
        int D = mk + int2((SSQPtr->ZQ[12] + 390) / 365.2422) * 12 + 900000;
        this->Lmonth2 = new GZ(D % 10, D % 12);
    }
    return *(this->Lmonth2);
}

GZ Day::getDayGZ()
{
    if (this->Lday2 == NULL)
    {
        int D = this->d0 - 6 + 9000000;
        this->Lday2 = new GZ(D % 10, D % 12);
    }
    return *(this->Lday2);
}

bool Day::isLunarLeap()
{
    this->checkLunarData();
    return this->Lleap;
}

int Day::getSolarYear()
{
    this->checkSolarData();
    return this->y;
}

uint8_t Day::getSolarMonth()
{
    this->checkSolarData();
    return this->m;
}

int Day::getSolarDay()
{
    this->checkSolarData();
    return this->d;
}

uint8_t Day::getWeek()
{
    if (this->week == 0xFF)
    {
        this->week = (this->d0 + J2000 + 1 + 7000000) % 7;
    }
    return this->week;
}

// 处于该月的第几周
uint8_t Day::getWeekIndex()
{
    int i = (this->getSolarDay() - 1) % 7;

    int w0 = 0;
    if (this->getWeek() >= i)
    {
        w0 = this->getWeek() - i;
    }
    else
    {
        w0 = this->getWeek() + 7 - i;
    }
    return int2((w0 + this->getSolarDay() - 1) / 7) + 1;
}
//是否有节气
bool Day::hasJieQi()
{
    this->checkJQData();
    return this->qk != -1;
}
// 获取节气
uint8_t Day::getJieQi()
{
    this->checkJQData();
    return this->qk;
}
double Day::getJieQiJD()
{
    if (this->hasJieQi())
    {
        if (this->jqjd != 0)
        {
            return this->jqjd + J2000;
        }

        long double d, xn, jd2 = this->d0 + dt_T(this->d0) - 8 / 24;
        long double w = XL::S_aLon(jd2 / 36525, 3);
        w = int2((w - 0.13) / pi2 * 24) * pi2 / 24;
        int D = 0;

        do
        {
            d = qi_accurate(w);
            D = int2(d + 0.5);
            // 计算出的节令值
            xn = int2(w / pi2 * 24 + 24000006.01) % 24;
            w += pi2 / 24;
            if (D > this->d0)
                break;
            if (D < this->d0)
                continue;
            if (D == this->d0)
            {
                this->jqjd = d;
                break;
            }
        } while (D + 12 < this->d0);

        return this->jqjd + J2000;
    }
    return 0;
}
// 获取星座
uint8_t Day::getConstellation()
{
    if (this->XiZ == 0xFF)
    {
        this->checkSSQ();
        int mk = int2((this->d0 - SSQPtr->ZQ[0] - 15) / 30.43685);
        //星座所在月的序数,(如果j=13,ob.d0不会超过第14号中气)
        if (mk < 11 && this->d0 >= SSQPtr->ZQ[2 * mk + 2])
        {
            mk++;
        }
        this->XiZ = (mk + 12) % 12;
    }
    return this->XiZ;
}

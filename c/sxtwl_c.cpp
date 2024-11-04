#include "sxtwl_c.h"

#include "sxtwl.h"

EXTERN_C_BEGIN

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

int sxtwl_getLunarMonth(sxtwl_Day *day)
{
    return ((Day *)day->ptr)->getLunarMonth();
}

int sxtwl_getLunarDay(sxtwl_Day *day)
{
    return ((Day *)day->ptr)->getLunarDay();
}

int sxtwl_getLunarYear(sxtwl_Day *day, bool chineseNewYearBoundary)
{
    return ((Day *)day->ptr)->getLunarYear(chineseNewYearBoundary);
}

void *sxtwl_getYearGZ(void *GZPtr, sxtwl_Day *day, bool chineseNewYearBoundary)
{
    auto ret = (GZ *)GZPtr;
    *ret = ((Day *)day->ptr)->getYearGZ(chineseNewYearBoundary);
    return ret;
}

void *sxtwl_getMonthGZ(void *GZPtr, sxtwl_Day *day)
{
    auto ret = (GZ *)GZPtr;
    *ret = ((Day *)day->ptr)->getMonthGZ();
    return ret;
}

void *sxtwl_getDayGZ(void *GZPtr, sxtwl_Day *day)
{
    auto ret = (GZ *)GZPtr;
    *ret = ((Day *)day->ptr)->getDayGZ();

    return ret;
}

void *sxtwl_getHourGZ(void *GZPtr, sxtwl_Day *day, uint8_t hour, bool isZaoWanZiShi)
{
    auto ret = (GZ *)GZPtr;
    *ret = ((Day *)day->ptr)->getHourGZ(hour, isZaoWanZiShi);

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

// 是否有节气
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

sxtwl_Day *sxtwl_newDay()
{
    sxtwl_Day *day = new sxtwl_Day();
    day->ptr = NULL;
    return day;
}

void sxtwl_freeDay(sxtwl_Day *day)
{
    if (day->ptr != NULL)
    {
        delete ((Day *)day->ptr);
    }
    delete day;
}

void *sxtwl_getShiGz(void *GZPtr, uint8_t dayTg, uint8_t hour, bool isZaoWanZiShi)
{
    auto ret = (GZ *)GZPtr;
    *ret = sxtwl::getShiGz(dayTg, hour, isZaoWanZiShi);
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

// 儒略日数转公历(返回的是Time指针)
void *sxtwl_JD2DD(double jd)
{
    Time t = sxtwl::JD2DD(jd);
    Time *time = new Time();
    time->Y = t.Y;
    time->M = t.M;
    time->D = t.D;

    time->h = t.h;
    time->m = t.m;
    time->s = t.s;
    return time;
}
// 公历转儒略日(参数是Time指针)
FFI_PLUGIN_EXPORT double sxtwl_toJD(void *time)
{
    Time *t = (Time *)time;
    return sxtwl::toJD(*t);
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

void *sxtwl_siZhu2Year(void *JdListPtr, void *yearGZ, void *yueGZ, void *riGZ, void *shiGZ, int fromYear, int toYear)
{
    auto ret = (std::vector<double> *)JdListPtr;
    *ret = sxtwl::siZhu2Year(*((GZ *)yearGZ), *((GZ *)yueGZ), *((GZ *)riGZ), *((GZ *)shiGZ), fromYear, toYear);
    return ret;
}

void *JdList_new()
{
    auto ret = new std::vector<double>();
    return ret;
}
void JdList_free(void *ptr)
{
    auto ret = (std::vector<double> *)ptr;
    delete ret;
}
int JdList_getNum(void *ptr)
{
    auto ret = (std::vector<double> *)ptr;
    return ret->size();
}

double JdList_indexOf(void *ptr, int index)
{
    auto ret = (std::vector<double> *)ptr;
    return ret->at(index);
}

void *GZ_new(int tg, int dz)
{
    return new GZ(tg, dz);
}
void GZ_free(void *ptr)
{
    auto ret = (GZ *)ptr;
    delete ret;
}
int GZ_getTg(void *ptr)
{
    auto ret = (GZ *)ptr;
    return ret->tg;
}
void GZ_setTg(void *ptr, int tg)
{
    auto ret = (GZ *)ptr;
    ret->tg = tg;
}
int GZ_getDz(void *ptr)
{
    auto ret = (GZ *)ptr;
    return ret->dz;
}
void GZ_setDz(void *ptr, int dz)
{
    auto ret = (GZ *)ptr;
    ret->dz = dz;
}

void *Time_new()
{
    return new Time();
}

void Time_free(void *ptr)
{
    auto ret = (Time *)ptr;
    delete ret;
}

int Time_getYear(void *ptr)
{
    auto ret = (Time *)ptr;
    return ret->getYear();
}

void Time_setYear(void *ptr, int year)
{
    auto ret = (Time *)ptr;
    ret->setYear(year);
}

void Time_setMonth(void *ptr, int month)
{
    auto ret = (Time *)ptr;
    ret->setMonth(month);
}

int Time_getMonth(void *ptr)
{
    auto ret = (Time *)ptr;
    return ret->getMonth();
}

int Time_getDay(void *ptr)
{
    auto ret = (Time *)ptr;
    return ret->getDay();
}

void Time_setDay(void *ptr, int day)
{
    auto ret = (Time *)ptr;
    ret->setDay(day);
}

double Time_getHour(void *ptr)
{
    auto ret = (Time *)ptr;
    return ret->getHour();
}

void Time_setHour(void *ptr, double hour)
{
    auto ret = (Time *)ptr;
    ret->setHour(hour);
}

double Time_getMin(void *ptr)
{
    auto ret = (Time *)ptr;
    return ret->getMin();
}

void Time_setMin(void *ptr, double min)
{
    auto ret = (Time *)ptr;
    return ret->setMin(min);
}

double Time_getSec(void *ptr)
{
    auto ret = (Time *)ptr;
    return ret->getSec();
}
void Time_setSec(void *ptr, double sec)
{
    auto ret = (Time *)ptr;
    ret->setSec(sec);
}

EXTERN_C_END
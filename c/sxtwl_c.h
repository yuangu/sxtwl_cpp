#pragma once

#include <stdint.h>

#include <stdbool.h>

#if _WIN32
#define FFI_PLUGIN_EXPORT __declspec(dllexport)
#else
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define FFI_PLUGIN_EXPORT EMSCRIPTEN_KEEPALIVE
#else
#define FFI_PLUGIN_EXPORT
#endif
#endif

#ifdef __cplusplus
#define EXTERN_C_BEGIN \
    extern "C"         \
    {
#else
#define EXTERN_C_BEGIN
#endif

#ifdef __cplusplus
#define EXTERN_C_END }
#else
#define EXTERN_C_END
#endif

EXTERN_C_BEGIN

typedef struct sxtwl_Time
{
    int year, month, day;
    double hour, min, sec;
} sxtwl_Time;

typedef struct sxtwl_Day
{
    void *ptr;
} sxtwl_Day;

// 内存管理
FFI_PLUGIN_EXPORT sxtwl_Day *sxtwl_newDay();
FFI_PLUGIN_EXPORT void sxtwl_freeDay(sxtwl_Day *day);

// 注意返回的sxtwl_Day指针，需要使用sxtwl_freeDay释放
FFI_PLUGIN_EXPORT sxtwl_Day *sxtwl_after(sxtwl_Day *day, sxtwl_Day *ret, int day_num);
FFI_PLUGIN_EXPORT sxtwl_Day *sxtwl_before(sxtwl_Day *day, sxtwl_Day *ret, int day_num);

FFI_PLUGIN_EXPORT int sxtwl_getLunarMonth(sxtwl_Day *day);
FFI_PLUGIN_EXPORT int sxtwl_getLunarDay(sxtwl_Day *day);
FFI_PLUGIN_EXPORT int sxtwl_getLunarYear(sxtwl_Day *day, bool chineseNewYearBoundary);

FFI_PLUGIN_EXPORT void *sxtwl_getYearGZ(void *GZPtr, sxtwl_Day *day, bool chineseNewYearBoundary);
FFI_PLUGIN_EXPORT void *sxtwl_getMonthGZ(void *GZPtr, sxtwl_Day *day);
FFI_PLUGIN_EXPORT void *sxtwl_getDayGZ(void *GZPtr, sxtwl_Day *day);
// 注意非早晚子时的时候，day要算第二天
FFI_PLUGIN_EXPORT void *sxtwl_getHourGZ(void *GZPtr, sxtwl_Day *day, uint8_t hour, bool isZaoWanZiShi);
FFI_PLUGIN_EXPORT bool sxtwl_isLunarLeap(sxtwl_Day *day);

FFI_PLUGIN_EXPORT int sxtwl_getSolarYear(sxtwl_Day *day);
FFI_PLUGIN_EXPORT uint8_t sxtwl_getSolarMonth(sxtwl_Day *day);
FFI_PLUGIN_EXPORT int sxtwl_getSolarDay(sxtwl_Day *day);
FFI_PLUGIN_EXPORT uint8_t sxtwl_getWeek(sxtwl_Day *day);
// 处于该月的第几周
FFI_PLUGIN_EXPORT uint8_t sxtwl_getWeekIndex(sxtwl_Day *day);
// 是否有节气
FFI_PLUGIN_EXPORT bool sxtwl_hasJieQi(sxtwl_Day *day);
// 获取节气
FFI_PLUGIN_EXPORT uint8_t sxtwl_getJieQi(sxtwl_Day *day);
FFI_PLUGIN_EXPORT double sxtwl_getJieQiJD(sxtwl_Day *day);
// 获取星座
FFI_PLUGIN_EXPORT uint8_t sxtwl_getConstellation(sxtwl_Day *day);

FFI_PLUGIN_EXPORT sxtwl_Day *sxtwl_fromSolar(sxtwl_Day *ret, int year, uint8_t month, int day);

FFI_PLUGIN_EXPORT sxtwl_Day *sxtwl_fromLunar(sxtwl_Day *ret, int year, uint8_t month, int day, bool isRun);

// 通过四柱获取年月日, 返回的是儒略日列表
FFI_PLUGIN_EXPORT void *sxtwl_siZhu2Year(void *JdListPtr, void *yearGZ, void *yueGZ, void *riGZ, void *shiGZ, int fromYear, int toYear);
////获取时辰上的那个天干
FFI_PLUGIN_EXPORT void *sxtwl_getShiGz(void *gzPtr, uint8_t dayTg, uint8_t hour, bool isZaoWanZiShi);

// 获取一年中的润月(不存，则返回0)
FFI_PLUGIN_EXPORT uint8_t sxtwl_getRunMonth(int By);
// 获取一月中的阴日数量
FFI_PLUGIN_EXPORT uint8_t sxtwl_getLunarMonthNum(int By, uint8_t month, bool isRun);
// 儒略日数转公历
FFI_PLUGIN_EXPORT sxtwl_Time sxtwl_JD2DD(double jd);
// 公历转儒略日
FFI_PLUGIN_EXPORT double sxtwl_toJD(sxtwl_Time *time);

// 反推的jd列表使用
FFI_PLUGIN_EXPORT void *JdList_new();
FFI_PLUGIN_EXPORT void JdList_free(void *ptr);
FFI_PLUGIN_EXPORT int JdList_getNum(void *ptr);
FFI_PLUGIN_EXPORT double JdList_indexOf(void *ptr, int index);

FFI_PLUGIN_EXPORT void *GZ_new(int tg, int dz);
FFI_PLUGIN_EXPORT void GZ_free(void *ptr);
FFI_PLUGIN_EXPORT int GZ_getTg(void *ptr);
FFI_PLUGIN_EXPORT void GZ_setTg(void *ptr, int tg);
FFI_PLUGIN_EXPORT int GZ_getDz(void *ptr);
FFI_PLUGIN_EXPORT void GZ_setDz(void *ptr, int dz);

EXTERN_C_END
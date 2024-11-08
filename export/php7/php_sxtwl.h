/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.1.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

#ifndef PHP_SXTWL_H
#define PHP_SXTWL_H

extern zend_module_entry sxtwl_module_entry;
#define phpext_sxtwl_ptr &sxtwl_module_entry

#ifdef PHP_WIN32
# define PHP_SXTWL_API __declspec(dllexport)
#else
# define PHP_SXTWL_API
#endif

static PHP_METHOD(JDList,__construct);
static PHP_METHOD(JDList,size);
static PHP_METHOD(JDList,capacity);
static PHP_METHOD(JDList,reserve);
static PHP_METHOD(JDList,clear);
static PHP_METHOD(JDList,push);
static PHP_METHOD(JDList,is_empty);
static PHP_METHOD(JDList,pop);
static PHP_METHOD(JDList,get);
static PHP_METHOD(JDList,set);
PHP_METHOD(JDList,__set);
PHP_METHOD(JDList,__get);
PHP_METHOD(JDList,__isset);
static PHP_METHOD(JQList,__construct);
static PHP_METHOD(JQList,size);
static PHP_METHOD(JQList,capacity);
static PHP_METHOD(JQList,reserve);
static PHP_METHOD(JQList,clear);
static PHP_METHOD(JQList,push);
static PHP_METHOD(JQList,is_empty);
static PHP_METHOD(JQList,pop);
static PHP_METHOD(JQList,get);
static PHP_METHOD(JQList,set);
PHP_METHOD(JQList,__set);
PHP_METHOD(JQList,__get);
PHP_METHOD(JQList,__isset);
static PHP_METHOD(Time,__construct);
static PHP_METHOD(Time,Y_set);
static PHP_METHOD(Time,Y_get);
static PHP_METHOD(Time,M_set);
static PHP_METHOD(Time,M_get);
static PHP_METHOD(Time,D_set);
static PHP_METHOD(Time,D_get);
static PHP_METHOD(Time,h_set);
static PHP_METHOD(Time,h_get);
static PHP_METHOD(Time,m_set);
static PHP_METHOD(Time,m_get);
static PHP_METHOD(Time,s_set);
static PHP_METHOD(Time,s_get);
static PHP_METHOD(Time,getYear);
static PHP_METHOD(Time,setYear);
static PHP_METHOD(Time,setMonth);
static PHP_METHOD(Time,getMonth);
static PHP_METHOD(Time,getDay);
static PHP_METHOD(Time,setDay);
static PHP_METHOD(Time,getHour);
static PHP_METHOD(Time,setHour);
static PHP_METHOD(Time,getMin);
static PHP_METHOD(Time,setMour);
static PHP_METHOD(Time,getSec);
static PHP_METHOD(Time,setSec);
PHP_METHOD(Time,__set);
PHP_METHOD(Time,__get);
PHP_METHOD(Time,__isset);
static PHP_METHOD(GZ,__construct);
static PHP_METHOD(GZ,tg_set);
static PHP_METHOD(GZ,tg_get);
static PHP_METHOD(GZ,dz_set);
static PHP_METHOD(GZ,dz_get);
PHP_METHOD(GZ,__set);
PHP_METHOD(GZ,__get);
PHP_METHOD(GZ,__isset);
static PHP_METHOD(Day,fromSolar);
static PHP_METHOD(Day,fromLunar);
static PHP_METHOD(Day,after);
static PHP_METHOD(Day,before);
static PHP_METHOD(Day,getLunarDay);
static PHP_METHOD(Day,getLunarMonth);
static PHP_METHOD(Day,getLunarYear);
static PHP_METHOD(Day,getYearGZ);
static PHP_METHOD(Day,getMonthGZ);
static PHP_METHOD(Day,getDayGZ);
static PHP_METHOD(Day,getHourGZ);
static PHP_METHOD(Day,isLunarLeap);
static PHP_METHOD(Day,getSolarYear);
static PHP_METHOD(Day,getSolarMonth);
static PHP_METHOD(Day,getSolarDay);
static PHP_METHOD(Day,getWeek);
static PHP_METHOD(Day,getWeekIndex);
static PHP_METHOD(Day,hasJieQi);
static PHP_METHOD(Day,getJieQi);
static PHP_METHOD(Day,getJieQiJD);
static PHP_METHOD(Day,getConstellation);
PHP_METHOD(Day,__set);
PHP_METHOD(Day,__get);
PHP_METHOD(Day,__isset);
static PHP_METHOD(JieQiInfo,jd_set);
static PHP_METHOD(JieQiInfo,jd_get);
static PHP_METHOD(JieQiInfo,jqIndex_set);
static PHP_METHOD(JieQiInfo,jqIndex_get);
static PHP_METHOD(JieQiInfo,__construct);
PHP_METHOD(JieQiInfo,__set);
PHP_METHOD(JieQiInfo,__get);
PHP_METHOD(JieQiInfo,__isset);
static PHP_FUNCTION(fromSolar);
static ZEND_NAMED_FUNCTION(_wrap_fromLunar);
static PHP_FUNCTION(siZhu2Year);
static ZEND_NAMED_FUNCTION(_wrap_getShiGz);
static PHP_FUNCTION(getRunMonth);
static ZEND_NAMED_FUNCTION(_wrap_getLunarMonthNum);
static PHP_FUNCTION(JD2DD);
static PHP_FUNCTION(toJD);
static PHP_FUNCTION(getJieQiByYear);
PHP_MINIT_FUNCTION(sxtwl);

#endif /* PHP_SXTWL_H */
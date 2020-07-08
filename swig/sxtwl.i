%module sxtwl

%{
#include "const.h"
#include "lunar.h"
#include "JD.h"
%}

%include "stdint.i"
%include "std_vector.i"
%include "std_string.i"

namespace std{
   %template(DayList) vector<Day>;
   %template(DoubleList) vector<double>;
   %template(IntList) vector<int>;
}

%constant int J2000=2451545;

%catches(LunarException) Lunar::getLunarMonthNum(int By, uint8_t month, bool isRun = false);
%catches(LunarException) Lunar::getDayBySolar(int year, uint8_t month, uint8_t day);
%catches(LunarException) Lunar::getDayByLunar(int year, uint8_t month, uint8_t day, bool isRun = false);

%include "../src/lunar.h"
%include "../src/JD.h"

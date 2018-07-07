%module sxtwl

%{
#include "const.h"
#include "lunar.h"
%}

%include "stdint.i"
%include "std_vector.i"
%include "std_string.i"

namespace std{
   %template(DayList) vector<Day>;
}

%constant int J2000=2451545;
%include "../src/lunar.h"

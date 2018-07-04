%module sxtwl

%{
#include "const.h"
#include "lunar.h"
%}

%include "stdint.i"
%include "std_vector.i"
%include "std_string.i"

namespace std{
   %template(VectorOfStructVector) vector<Day>;
}

%include "../src/lunar.h"
#php
# swig -c++ -php5 -outcurrentdir  -outdir export/php5 swig/sxtwl.i
# mv sxtwl_wrap.cxx  export/php5/sxtwl_wrap.cxx

swig -c++ -php7 -outcurrentdir  -outdir export/php7 swig/sxtwl.i
mv sxtwl_wrap.cxx  export/php7/sxtwl_wrap.cxx

# lua
swig -c++ -lua -outcurrentdir  -outdir export/lua swig/sxtwl.i
mv sxtwl_wrap.cxx  export/lua/sxtwl_wrap.cxx

# java
swig -c++ -java -outcurrentdir  -outdir export/java -package com.seantone.sxtwl swig/sxtwl.i
mv sxtwl_wrap.cxx  export/java/sxtwl_wrap.cxx

# C#
swig -c++ -csharp -outcurrentdir  -outdir export/C# swig/sxtwl.i
mv sxtwl_wrap.cxx  export/C#/sxtwl_wrap.cxx

# go
swig -c++  -go -cgo -intgosize 32 -outdir export/golang swig/sxtwl.i
mv swig/sxtwl_wrap.cxx  export/golang/sxtwl_wrap.cxx

# python
swig -c++ -python -outdir  python swig/sxtwl.i
mv swig/sxtwl_wrap.cxx  python/sxtwl_wrap.cxx
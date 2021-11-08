#!/usr/bin/env python
#-*-coding:utf-8-*-

import setuptools
from distutils import ccompiler
import os,sys
import shutil,os
import platform
import io 

long_description  = ""

try:
    if sys.version_info < (3, 0) :
        with open('README.md') as f:
            long_description = f.read()
    else:
        long_description = io.open('README.md', 'r', encoding="utf-8").read()
except Exception as e:
    long_description = ""
finally:
    pass

# if sys.version_info < (3, 0) and platform.system() == 'Windows':
#     long_description = long_description.decode("utf-8").encode("gbk")

if sys.version_info >= (3, 0) and platform.system() == 'Windows':
        try:
            if isinstance(long_description, unicode):
                tmp=copy.deepcopy(long_description)
                tmp.encode("mbcs")
        except Exception as e:
            long_description = ''
        else:
            pass


# if sys.version_info < (3, 0):
#     with open("README.md", "r") as fh:
#         long_description = fh.read()
#         if platform.system() == 'Windows':
#             long_description = long_description.decode("utf8").encode("gbk")
# else:
#     with open("README.md", "r", encoding='utf-8') as fh:
#         long_description = "".join(fh.readlines())
#         #发现了一个有趣的问题：http://www.queasy.me/rbsoaeod.html/questions/43255455/unicode+character+causing+error+with+bdist_wininst+on+python+3+but+not+python+2
#         try:
#             long_description.encode("mbcs")
#         except Exception as e:
#             long_description = ''
#         else:
#             pass
      


if os.path.isdir("../src"):
    if os.path.isdir("src"):
        shutil.rmtree("src")
    shutil.copytree("../src","src")

extra_compile_args = []
if ccompiler.get_default_compiler() == "msvc":
    extra_compile_args.append("/utf-8")
else:
    extra_compile_args.append('-std=c++11')


sxtwl_module = setuptools.Extension('_sxtwl',
                         include_dirs=['src'],
                         sources=['sxtwl_wrap.cxx',
                          'src/eph.cpp', 
                          'src/JD.cpp',
                          'src/SSQ.cpp',
                          'src/sxtwl.cpp',  
                          'src/day.cpp',                     
                          ], 
                          extra_compile_args=extra_compile_args, 
)


setuptools.setup(
    name="sxtwl",
    version="2.0.1",
    author="yuangu",
    author_email="seantone@126.com",
    description="sxtwl_cpp warpper for python",
    long_description=long_description,
    long_description_content_type="text/markdown",
    license = "BSD",   
    #package_dir={'src': '../src'},
    url="https://github.com/yuangu/sxtwl_cpp",
    packages=setuptools.find_packages(), 
    ext_modules = [sxtwl_module],
    py_modules = ["sxtwl"],  
    
)

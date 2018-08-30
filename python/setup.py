#!/usr/bin/env python
#-*-coding:utf-8-*-

import setuptools
from distutils import ccompiler
import os,sys
import shutil,os
if sys.version_info < (3, 0):
    with open("README.md", "r") as fh:
        long_description = fh.read()
else:
    with open("README.md", "r", encoding='utf-8') as fh:
        long_description = fh.read()


if os.path.isdir("../src"):
    if os.path.isdir("src"):
        shutil.rmtree("src")
    shutil.copytree("../src","src")

extra_compile_args = []
if ccompiler.get_default_compiler() == "msvc":
    extra_compile_args.append("/utf-8")


sxtwl_module = setuptools.Extension('_sxtwl',
                         include_dirs=['src'],
                         sources=['sxtwl_wrap.cxx',
                          'src/eph.cpp', 
                          'src/JD.cpp',
                          'src/SSQ.cpp',
                          'src/lunar.cpp',                      
                          ], 
                          extra_compile_args=extra_compile_args, 
)


setuptools.setup(
    name="sxtwl",
    version="1.0.3",
    author="yuangu",
    author_email="lifulinghan@aol.com",
    description="寿星天文历的python实现版本",
    long_description=long_description,
    long_description_content_type="text/markdown",
    license = "BSD",   
    #package_dir={'src': '../src'},
    url="https://github.com/yuangu/sxtwl_cpp",
    packages=setuptools.find_packages(), 
    ext_modules = [sxtwl_module],
    py_modules = ["sxtwl"],  
    
)

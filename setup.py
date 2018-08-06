#!/usr/bin/env python
#-*-coding:utf-8-*-

import distutils.spawn as _spawn
import distutils.command.build as _build
import distutils.dir_util as _dir_util
import os
import sys
from distutils.core import setup
from distutils.extension import Extension

def run_cmake():
    """
    Runs CMake to determine configuration for this build.
    """
    if _spawn.find_executable('cmake') is None:
        print("CMake is required to build this package.")
        print("Please install/load CMake and re-run setup.")
        sys.exit(-1)

    _build_dir = os.path.join(os.path.split(__file__)[0], 'build_setup_py')
    _dir_util.mkpath(_build_dir)
    os.chdir(_build_dir)

    try:
        _spawn.spawn(['cmake', '-DCMAKE_BUILD_TYPE=release', '-DSXTWL_WRAPPER_PYTHON=1', '..'])
    except _spawn.DistutilsExecError:
        print("Error while running CMake")
        sys.exit(-1)


class build(_build.build):

    def run(self):
        cwd = os.getcwd()
        run_cmake()

        try:
            _spawn.spawn(['cmake', '--build', '.'])
            os.chdir(cwd)
        except _spawn.DistutilsExecError:
            print("Error while running Make")
            sys.exit(-1)

        _build.build.run(self)




setup(name='polygons',
      version='1.0.0',
      description='Fast points-in-polygon test and distances to polygons.',
      author='yuangu',
      author_email='lifulinghan@aol.com',
      url='https://github.com/yuangu/sxtwl_cpp',
    #   packages=['build_setup_py/_sxtwl_python'],
      license='GPL-v3.0',
      install_requires=[],
      package_data={'.': ['build_setup_py/_sxtwl_python.so']},
    #   ext_modules = ["build_setup_py/_sxtwl_python.so"],
      cmdclass={ 'build': build}
)
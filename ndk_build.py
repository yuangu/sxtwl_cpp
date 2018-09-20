#-*-coding:utf-8-*-

import os
import shutil
import zipfile
import hashlib
import sys

class Utils():
    #如果目录不存，则创建。
    @staticmethod
    def mkDir(dirPath):
        if os.path.exists(dirPath) and os.path.isdir(dirPath):
            return
        parent = os.path.dirname(dirPath)
        if not (os.path.exists(parent) and os.path.isdir(parent)):
            Utils.mkDir(parent)
        
        os.mkdir(dirPath)
    
    #获取某个目录是否含有某个文件, extList获取指定的文件后缀
    @staticmethod
    def getAllDirFiles(dirPath, extList = None):
        ret = []    
        for file in os.listdir( dirPath):
            if os.path.isfile(os.path.join(dirPath, file)):
               ret.append(os.path.join(dirPath, file))
            else:
                ret += Utils.getAllDirFiles(os.path.join(dirPath, file))
        
        #需要过滤某些文件
        if extList != None:             
            extList = [tmp.lower() for tmp in extList]           
            ret = [path for path in ret if os.path.splitext(path)[1].lower()  in extList]    
        return ret

    #清理掉某个数据
    @staticmethod
    def cleanFile(path):
        if not os.path.exists(path):
            return
        if os.path.isdir(path):
            shutil.rmtree(path)
        elif os.path.isfile(path):
            os.remove(path)

    #将一个文件夹压缩成zip文件
    @staticmethod
    def makeZipFile(fileName, fromDir):        
        fileList = Utils.getAllDirFiles(fromDir)
        with zipfile.ZipFile(fileName , 'w')  as zip:
            for file in fileList:
                zip.write(file, os.path.relpath(file, fromDir))

#获取python文件所在的路径
def p():
    frozen = "not"
    if getattr(sys, 'frozen',False):
        frozen = "ever so"
        return os.path.dirname(sys.executable)

    return os.path.split(os.path.realpath(__file__))[0]


if '__main__' == __name__:
    pyPath = p()
    buildDir = os.path.join(pyPath, "build")
    outDir = os.path.join(pyPath, "out")

  

    Utils().cleanFile(outDir)
    Utils().mkDir(outDir)

    
    ANDROID_SDK= 'D:/android/sdk'
    ANDROID_NDK= 'D:/android/android-ndk-r16b'


    ANDROID_CMAKE= os.path.join(ANDROID_SDK, 'cmake/3.6.4111459/bin/cmake.exe')
    ANDROID_NINJA=os.path.join(ANDROID_SDK,'cmake/3.6.4111459/bin/ninja.exe')
    #需要打包的abi
    abiList = [
        'armeabi',
        'armeabi-v7a',
        "arm64-v8a",
        "x86",
        'x86_64',
        'mips',
        'mips64',
    ]
    for abi in abiList:
        os.chdir(pyPath)
        Utils().cleanFile(buildDir)
        Utils().mkDir(buildDir)
        os.chdir(buildDir)

        cmd = '''%s -DANDROID_ABI=%s   \
        -DANDROID_PLATFORM=android-16  \
        -DCMAKE_BUILD_TYPE=Release   \
        -DANDROID_NDK=%s    \
        -DCMAKE_CXX_FLAGS=-std=c++11 -frtti -fexceptions   \
        -DCMAKE_TOOLCHAIN_FILE=%s/build/cmake/android.toolchain.cmake    \
        -DCMAKE_MAKE_PROGRAM=%s -G "Ninja"    \
        -DCONSOLE=1   \
        -DSXTWL_WRAPPER_JAVA=1 ..'''%(ANDROID_CMAKE,abi,ANDROID_NDK,ANDROID_NDK,ANDROID_NINJA ) 

        os.system(cmd)
        os.system("%s --build ."%(ANDROID_CMAKE, ))

        outSoPath = os.path.join(outDir, "jniLibs/" + abi)
        Utils().cleanFile(outSoPath)
        Utils().mkDir(outSoPath)
        
        outJavaPath = os.path.join(outDir, "java/com/huoyaojing")
        if not os.path.isdir(outJavaPath):
            Utils().mkDir(outJavaPath)
            fileList = Utils.getAllDirFiles(buildDir, [".java"])
            for tmp in fileList:
                basename =  os.path.basename(tmp)
                shutil.move(tmp, os.path.join(outJavaPath, basename))


        shutil.move(os.path.join(buildDir, "libsxtwl_java.so"),os.path.join(outSoPath, "libsxtwl_java.so"))
        
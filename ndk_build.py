#-*-coding:utf-8-*-

import os
import shutil
import zipfile
import hashlib
import sys
import platform
import requests
import urllib
import subprocess

#工具类
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
    
    @staticmethod
    def extractZipFile(fileName, toDir = "."):
        file_zip = zipfile.ZipFile(fileName, 'r')
        for file in file_zip.namelist():
            file_zip.extract(file, toDir)
        file_zip.close()
            

    @staticmethod
    def sha256_checksum(filename, block_size=65536):
        sha256 = hashlib.sha256()
        with open(filename, 'rb') as f:
            for block in iter(lambda: f.read(block_size), b''):
                sha256.update(block)
        return sha256.hexdigest()

#获取python文件所在的路径
def p():
    frozen = "not"
    if getattr(sys, 'frozen',False):
        frozen = "ever so"
        return os.path.dirname(sys.executable)

    return os.path.split(os.path.realpath(__file__))[0]

#下载进度条回调 
def callbackfunc(blocknum, blocksize, totalsize):
    '''回调函数
    @blocknum: 已经下载的数据块
    @blocksize: 数据块的大小
    @totalsize: 远程文件的大小
    '''
    percent = 100.0 * blocknum * blocksize / totalsize
    if percent > 100:
        percent = 100
    
    max_arrow = 50 #进度条的长度
    num_arrow = int(percent * max_arrow/100.0) 

    process_bar = '\r[' + '>' * num_arrow + '#' * (max_arrow -  num_arrow) + ']'\
                      + '%.2f%%' % percent  #带输出的字符串，'\r'表示不换行回到最左边
    sys.stdout.write(process_bar) #这两句打印字符到终端
    sys.stdout.flush()

#andoird sdk 的操作sdk路径 
class  AndroidSDK():
    def __init__(self):
        self.ANDROID_SDK  =  self.getAndroidSDKPath()
        if self.ANDROID_SDK == None:
            self.ANDROID_SDK = self.installAndroidSDK()
            #更新android  sdk 
            self.updateSDK(['platforms;android-16'])
        
        self.cmakeDir = self.getCmakeDir()
        if self.cmakeDir == None:
            self.updateSDK(['cmake;3.6.4111459'])
            self.cmakeDir = self.getCmakeDir()

        self.NDKPath = self.getNDKPath()
        if self.NDKPath == None:
            self.updateSDK(['ndk-bundle'])
            self.NDKPath = self.getNDKPath()

        
    def installAndroidSDK(self):
        sysstr = platform.system().lower()
        
        SHA_256 = {
            "windows":'7e81d69c303e47a4f0e748a6352d85cd0c8fd90a5a95ae4e076b5e5f960d3c7a',
            'darwin':'ecb29358bc0f13d7c2fa0f9290135a5b608e38434aad9bf7067d0252c160853e',
            'linux':'92ffee5a1d98d856634e8b71132e8a95d96c83a63fde1099be3d86df3106def9',
        }
        
        #是否需要下载包
        needDownload  = True
        android_sdk_zip = "android_sdk.zip"
        if os.path.isfile(android_sdk_zip):
            sha256 = Utils.sha256_checksum(android_sdk_zip)
            if sha256.lower() == SHA_256[sysstr]:
                needDownload =  False
        
        print u"下载Android_sdk"
        #下载包
        if needDownload:
            sdk_download_url = 'https://dl.google.com/android/repository/sdk-tools-%s-4333796.zip'%(sysstr, )     
            urllib.urlretrieve(sdk_download_url, android_sdk_zip, callbackfunc)

        #解压文件
        Utils.extractZipFile(android_sdk_zip,  "./android_sdk")
        os.environ['ANDROID_HOME'] = os.path.realpath("android_sdk")
        return os.environ['ANDROID_HOME']

    def updateSDK(self, package = [ 'platforms;android-16', 'cmake;3.6.4111459', 'ndk-bundle' ]):
        sdkmanager = os.path.join(self.ANDROID_SDK, 'tools/bin/sdkmanager')
        if "windows" ==  platform.system().lower():
            sdkmanager =  sdkmanager + '.bat'
        else:
            cmd = 'chmod +x %s' %(sdkmanager,)
            os.system(cmd)

        args = ['"%s"' %(key) for key in package]

        args.insert(0, sdkmanager)
        cmd = 'echo y|' +  " ".join(args)
        os.system(cmd)
       

    #获取sdk里的 cmake 信息 
    def getCmakeDir(self):
        ndk_cmake_dir  = os.path.join(self.ANDROID_SDK,  "cmake")
        if  not  os.path.isdir(ndk_cmake_dir):
            return None
        
        cmake_dir_list = os.listdir(ndk_cmake_dir)
        list_len = len(cmake_dir_list)
        if list_len <= 0:
            return  None
       
        return os.path.join(ndk_cmake_dir, cmake_dir_list[list_len - 1] )
       
        
    def  getNDKPath(self):
        #通过系统变量来寻找
        environ_names = [
           'NDK_ROOT', 
        ]

        for name in environ_names:            
            #环境变量里不存在
            if name not  in os.environ.keys():
                continue

            android_ndk_path = os.environ[name]
            #验证如果不存在此目录 
            if not  os.path.isdir(android_ndk_path):
                continue
         
            return android_ndk_path
        
        ndk_bundle_dir  = os.path.join(self.ANDROID_SDK,  "ndk-bundle/toolchains")
        if os.path.isdir(ndk_bundle_dir):
            return  os.path.join(self.ANDROID_SDK, "ndk-bundle")

    
    # 根据系统变量android sdk的路径
    def getAndroidSDKPath(self):
        environ_names = [
           'ANDROID_HOME', 
           'ANDROID_SDK_ROOT'
        ]

        for name in environ_names:            
            #环境变量里不存在
            if name not  in os.environ.keys():
                continue

            android_sdk_path = os.environ[name]
            #验证如果不存在此目录 
            if not  os.path.isdir(android_sdk_path):
                continue
         
            return android_sdk_path
        
        #没有找到相应的sdk路径
        return None


if '__main__' == __name__:
    android_sdk = AndroidSDK()
    ANDROID_SDK = android_sdk.getAndroidSDKPath()
    ANDROID_NDK =android_sdk.getNDKPath()

    ANDROID_CMAKE = os.path.join(android_sdk.getCmakeDir(), 'bin/cmake')
    ANDROID_NINJA=os.path.join(android_sdk.getCmakeDir(),'bin/ninja')

    if "windows" ==  platform.system().lower():
        ANDROID_CMAKE =  ANDROID_CMAKE + '.exe'
        ANDROID_NINJA = ANDROID_NINJA + '.exe'

    pyPath = p()
    buildDir = os.path.join(pyPath, "build")
    outDir = os.path.join(pyPath, "out")


    Utils().cleanFile(outDir)
    Utils().mkDir(outDir)
    
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
        
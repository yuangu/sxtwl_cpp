#cmake has some strange defaults, this should help us a lot
#Please use them everywhere

#WINDOWS 	= 	Windows Desktop
#WINRT 		= 	Windows RT
#WP8 	  	= 	Windows Phone 8
#ANDROID    =	Android
#IOS		=	iOS
#MACOSX		=	MacOS X
#LINUX      =   Linux

#define PLATFORM_UNKNOWN 0
#define PLATFORM_IOS     1
#define PLATFORM_ANDROID 2
#define PLATFORM_WIN32   3
#define PLATFORM_WP8	 4
#define PLATFORM_WIN10	 5
#define PLATFORM_LINUX   6

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  if(WINRT)
    set(SYSTEM_STRING "Windows RT")
    add_definitions(-DTARGET_PLATFORM=5)
  elseif(WP8)
    set(SYSTEM_STRING "Windows Phone 8")
    add_definitions(-DTARGET_PLATFORM=4)
  else()
    set(WINDOWS TRUE)
    set(SYSTEM_STRING "Windows Desktop")
    add_definitions(-DTARGET_PLATFORM=3)
  endif()
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  if(ANDROID)
    set(SYSTEM_STRING "Android")
    add_definitions(-DTARGET_PLATFORM=2)
  else()
    set(LINUX TRUE)
    set(SYSTEM_STRING "Linux")
    add_definitions(-DTARGET_PLATFORM=6)
  endif()
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  if(IOS)
    set(SYSTEM_STRING "IOS")
    add_definitions(-DTARGET_PLATFORM=1)
  else()
    set(MACOSX TRUE)
    set(APPLE TRUE)
    set(SYSTEM_STRING "Mac OSX")
    add_definitions(-DTARGET_PLATFORM=6)
  endif()
elseif (EMSCRIPTEN)
    add_definitions(-DTARGET_PLATFORM=6)
else()
    message(ERROR "unrecognized target platform.")
endif()

SET(CMAKE_INSTALL_RPATH "@executable_path")

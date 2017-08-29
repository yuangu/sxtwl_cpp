#pragma once
//PI
#define PI (3.141592653589793)

//地球赤道半径(千米)
#define  cs_rEar (6378.1366) 
//平均半径
#define  cs_rEarA (0.99834*cs_rEar)
//地球极赤半径比
#define  cs_ba (0.99664719) 
//地球极赤半径比的平方
#define  cs_ba2 (cs_ba*cs_ba)
//天文单位长度(千米)
#define  cs_AU (1.49597870691e8) 
//sin(太阳视差)
#define  cs_sinP (cs_rEar / cs_AU)  
//太阳视差
#define  cs_PI ( Math.asin(cs_sinP))
//光速(行米/秒)
#define  cs_GS ( 299792.458) 
//每天文单位的光行时间(儒略世纪)
#define  cs_Agx ( cs_AU / cs_GS / 86400 / 36525) 
//#define  cs_xxHH ( new Array(116, 584, 780, 399, 378, 370, 367, 367)) //行星会合周期
//#define  xxName ( new Array('地球', '水星', '金星', '火星', '木星', '土星', '天王星', '海王星', '冥王星'))
//每弧度的角秒数
#define  rad ( 180 * 3600 / PI) 
//每弧度的度数
#define  radd ( 180 / PI)
#define  pi2 ( PI * 2)
#define  pi_2 ( PI / 2)
#define  J2000 ( 2451545)
//
#define  cs_k ( 0.2725076) //月亮与地球的半径比(用于半影计算)
#define  cs_k2 ( 0.2722810) //月亮与地球的半径比(用于本影计算)
#define  cs_k0 ( 109.1222)  //太阳与地球的半径比(对应959.64)
#define  cs_sMoon ( cs_k*cs_rEar*1.0000036*rad)  //用于月亮视半径计算
#define  cs_sMoon2 ( cs_k2*cs_rEar*1.0000036*rad) //用于月亮视半径计算
#define  cs_sSun ( 959.64) //用于太阳视半径计算

//转整型
#define int2(v) ((v)>=0?(int)(v):((int)(v)-1))
//求余
#define fmod2(v, n) ((v%n + n) % n)

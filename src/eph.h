#pragma once
#include <stdint.h>
#include <stdint.h>

struct Vector3
{
	Vector3(long double x, long double y, long double z) :
		x(x), y(y), z(z)
	{

	};

	Vector3(const Vector3& v) :
		x(v.x), y(v.y), z(v.z)
	{

	};
    
    

	Vector3() {};
	long double x, y, z;

	long double& operator [](const uint64_t index)////重载[]操作符，作为左值
	{
		switch (index)
		{
			case 0: return x;
			case 1: return y;
			case 2: return z;
		}

    };
	const long double& operator [](const uint64_t index) const////重载[]操作符，作为右值
	{
		switch (index)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		}
	};
};

struct Vector2
{
	Vector2(long double x, long double y) :
		x(x), y(y)
	{

	};
	Vector2() {};
	long double x, y;

	long double& operator [](const uint64_t index)////重载[]操作符，作为左值
	{
		switch (index)
		{
		case 0: return x;
		case 1: return y;		
		}

	};
	const long double& operator [](const uint64_t index) const////重载[]操作符，作为右值
	{
		switch (index)
		{
		case 0: return x;
		case 1: return y;	
		}
	};
};

//=================================数学工具=========================================

//对超过0-2PI的角度转为0-2PI;
long double rad2mrad(long double v);
//对超过-PI到PI的角度转为-PI到PI;
long double  rad2rrad(long double v);
//临界余数(a与最近的整倍数b相差的距离);
long double mod2(long double a, long double b);
//球面转直角坐标;
Vector3 llr2xyz(Vector3 JW);
//直角坐标转球;
Vector3 xyz2llr(Vector3  xyz);
//球面坐标旋转;
Vector3 llrConv(Vector3 JW, long double E);
//赤道坐标转为地平坐标;
Vector3 CD2DP(Vector3 z, long double L, long double fa, long double gst);
//求角度差;
long double j1_j2(long double J1, long double W1, long double J2, long double W2);
//日心球面转地心球面,Z星体球面坐标,A地球球面坐标;
//本含数是通用的球面坐标中心平移函数,行星计算中将反复使用;
Vector3  h2g(Vector3 z, Vector3 a);
//视差角(不是视差);
long double shiChaJ(long double gst, long double L, long double fa, long double J, long double W);

//物件XL : 日月黄道平分点坐标、视坐标、速度、已知经度反求时间等方面的计算
namespace XL
{
	//=====================
	//星历函数(日月球面坐标计算)

	long double E_Lon(long double t, int n); //地球经度计算,返回Date分点黄经,传入世纪数、取项数
	long double M_Lon(long double t, int n);     //月球经度计算,返回Date分点黄经,传入世纪数,n是项数比例
	//地球速度,t是世纪数,误差小于万分3													//=========================
	long double E_v(long double t);


	//月球速度计算,传入世经数
	long double M_v(long double t);

	//=========================
	//月日视黄经的差值
	long double  MS_aLon(long double t, long double Mn, long double Sn);

	//太阳视黄经
	long double S_aLon(long double t, long double n);

	//=========================
	//已知地球真黄经求时间
	long double E_Lon_t(long double W);

	//已知真月球黄经求时间
	long double M_Lon_t(long double W);

	//已知月日视黄经差求时间
	long double MS_aLon_t(long double W);

	//已知太阳视黄经反求时间
	long double S_aLon_t(long double W);
	
	//已知月日视黄经差求时间,高速低精度,误差不超过600秒(只验算了几千年)
	long double MS_aLon_t2(long double W);
	//已知太阳视黄经反求时间,高速低精度,最大误差不超过600秒
	long double  S_aLon_t2(long double  W);

	long double  moonIll(long double  t);

	//转入地平纬度及地月质心距离,返回站心视半径(角秒)
	long double moonRad(long double r, long double h);

	//求月亮近点时间和距离,t为儒略世纪数力学时
	Vector2 moonMinR(long double t, long double min);

	Vector3  moonNode(long double t, long double asc);

	//地球近远点
	Vector2 earthMinR(long double t, long double min);
};


//=================================deltat T计算=====================================
long double dt_T(long double t);

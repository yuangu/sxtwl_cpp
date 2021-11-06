#pragma once

class XL
{
public:
	//已知太阳视黄经反求时间
	static long double S_aLon_t(long double W);

	//地球速度,t是世纪数,误差小于万分3
	static long double E_v(long double t);

	//太阳视黄经
	static long double S_aLon(long double t, long double n);

	//地球经度计算,返回Date分点黄经,传入世纪数、取项数
	static long double E_Lon(long double t, long double n);

	//xt星体,zn坐标号,t儒略世纪数,n计算项数
	static long double XL0_calc(long double xt, int zn, long double t, int n);


};
#include "lunar.h"
#include "const.h"
#include "JD.h"
#include "eph.h"

//精气
inline long double qi_accurate(long double W)
{ 
	long double t = XL::S_aLon_t(W) * 36525; 
	return t - dt_T(t) + 8.0f / 24.f;
}

inline long double so_accurate(long double W)
{ 
	long double t = XL::MS_aLon_t(W) * 36525;
	return t - dt_T(t) + 8.0f / 24.0f;
} //精朔


Month Lunar::yueLiCalc(int By, uint8_t Bm)
{
	Month month;
	int  Bd0, Bdn;

	//日历物件初始化;
	Time t;
	t.h = 12, t.m = 0, t.s = 0.1;
	t.Y = By; t.M = Bm; t.D = 1;

	//公历月首的儒略日,中午;
	Bd0 = int2(JD::toJD(t)) - J2000;

	t.M++; if (t.M > 12) t.Y++, t.M = 1;
	//本月天数(公历);
	Bdn = int2(JD::toJD(t)) - J2000 - Bd0;

	//本月第一天的星期;
	int w0 = (Bd0 + J2000 + 1 + 7000000) % 7;
	month.y = By; //公历年份
	month.m = Bm; //公历月分

	//this.d0 = Bd0;
	//this.dn = Bdn;

	////所属公历年对应的农历干支纪年
	int  c = By - 1984 + 12000;

	//年天干地支
	month.yearGan = c % 10;
	month.yearZhi = c % 12;
	//年生肖
	month.ShX = c % 12;

	//下面开始提取各日的信息
	int  D, day, day2;
	long double w;
	std::vector<Day>& lun = month.days;

	//提取各日信息
	for (int i = 0, j = 0; i < Bdn; i++)
	{
		Day day;
		day.d0 = Bd0 + i; //儒略日,北京时12:00
		day.di = i;  //公历月内日序数
		day.y = By; //公历年
		day.m = Bm; //公历月
		day.dn = Bdn;   //公历月天数
		day.week0 = w0; //月首的星期
		day.week = (w0 + i) % 7; //当前日的星期
		day.weeki = int2((w0 + i) / 7); //本日所在的周序号
		day.weekN = int2((w0 + Bdn - 1) / 7) + 1;  //本月的总周数	
		Time time = JD::JD2DD(day.d0 + J2000);
		day.d = time.D; //公历日名称;

		//如果d0已在计算农历范围内则不再计算
		if (!mSSQ.ZQ.size() || day.d0 < mSSQ.ZQ[0] || day.d0 >= mSSQ.ZQ[24]) 
		{
			mSSQ.calcY(day.d0);
		}
			
		int mk = int2((day.d0 - mSSQ.HS[0]) / 30); 
		//农历所在月的序数;
		if (mk < 13 && mSSQ.HS[mk + 1] <= day.d0)
		{
			mk++;
		}

		day.Ldi = day.d0 - mSSQ.HS[mk];   //距农历月首的编移量,0对应初一
		
		day.cur_dz = day.d0 - mSSQ.ZQ[0];   //距冬至的天数
		day.cur_xz = day.d0 - mSSQ.ZQ[12];  //距夏至的天数
		day.cur_lq = day.d0 - mSSQ.ZQ[15];  //距立秋的天数
		day.cur_mz = day.d0 - mSSQ.ZQ[11];  //距芒种的天数
		day.cur_xs = day.d0 - mSSQ.ZQ[13];  //距小暑的天数
		if (day.d0 == mSSQ.HS[mk] || day.d0 == Bd0) { //月的信息			
			day.Lmc = mSSQ.ym[mk]; //月名称
			day.Ldn = mSSQ.dx[mk]; //月大小
			day.Lleap = (mSSQ.leap&&mSSQ.leap == mk); //闰状况
			day.Lmc2 = mk < 13 ? mk + 1 : -1; //下个月名称,判断除夕时要用到
		}
		else {
			Day day2 = lun[i - 1];
			day.Lmc = day2.Lmc, day.Ldn = day2.Ldn;
			day.Lleap = day2.Lleap, day.Lmc2 = day2.Lmc2;
		}
		int qk = int2((day.d0 - mSSQ.ZQ[0] - 7) / 15.2184); 
		
		//节气的取值范围是0-23
		if (qk < 23 && day.d0 >= mSSQ.ZQ[qk + 1])
		{
			qk++; 
		}
			
		day.qk = -1;
		if (day.d0 == mSSQ.ZQ[qk])
		{
			day.qk = qk;		
		}
	

		//day.yxmc = day.yxjd = day.yxsj = '';//月相名称,月相时刻(儒略日),月相时间串
		//day.jqmc = day.jqjd = day.jqsj = '';//定气名称,节气时刻(儒略日),节气时间串

		//干支纪年处理
		//以立春为界定年首
		D = mSSQ.ZQ[3] + (day.d0 < mSSQ.ZQ[3] ? -365 : 0) + 365.25 * 16 - 35; //以立春为界定纪年
		day.Lyear = floor(D / 365.2422 + 0.5); //农历纪年(10进制,1984年起算)
		//以下几行以正月初一定年首
		D = mSSQ.HS[2]; //一般第3个月为春节
		for (j = 0; j < 14; j++) { //找春节
			//正月的index = 3
			if (mSSQ.ym[j] != 2 || mSSQ.leap == j&&j) continue;
			D = mSSQ.HS[j];
			if (day.d0 < D) { D -= 365; break; } //无需再找下一个正月
		}
		D = D + 5810;  //计算该年春节与1984年平均春节(立春附近)相差天数估计
		day.Lyear0 = floor(D / 365.2422 + 0.5); //农历纪年(10进制,1984年起算)

		//干支纪年(立春)
		D = day.Lyear + 12000;
		day.Lyear2.tg = D % 10;
		day.Lyear2.dz = D % 12;
		
		//干支纪年(正月)
		D = day.Lyear0 + 12000;  
		day.Lyear3.tg = D % 10;
		day.Lyear3.dz = D % 12;
		
		//干支纪年(正月)
		day.Lyear4 = day.Lyear0 + 1984 + 2698; //黄帝纪年

		 //纪月处理,1998年12月7(大雪)开始连续进行节气计数,0为甲子
		mk = int2((day.d0 - mSSQ.ZQ[0]) / 30.43685); 
		//相对大雪的月数计算,mk的取值范围0-12
		if (mk < 12 && day.d0 >= mSSQ.ZQ[2 * mk + 1])
		{
			mk++;
		}

		D = mk + int2((mSSQ.ZQ[12] + 390) / 365.2422) * 12 + 900000; //相对于1998年12月7(大雪)的月数,900000为正数基数
		day.Lmonth = D % 12;	
		day.Lmonth2.tg = D % 10;
		day.Lmonth2.dz = D % 12;

		////纪日,2000年1月7日起算
		D = day.d0 - 6 + 9000000;
		day.Lday2.tg = D % 10;
		day.Lday2.dz = D % 12;
		
		//星座
		mk = int2((day.d0 - mSSQ.ZQ[0] - 15) / 30.43685); 
		//星座所在月的序数,(如果j=13,day.d0不会超过第14号中气)
		if (mk < 11 && day.d0 >= mSSQ.ZQ[2 * mk + 2]) mk++;
		day.XiZ = (mk + 12) % 12;
		////回历
		//daya.getHuiLi(day.d0, day);
		////节日
		//day.A = day.B = day.C = ''; day.Fjia = 0;
		//daya.getDayName(day, day); //公历
		//dayb.getDayName(day, day); //农历



		lun.push_back(day);
	}
	//以下是月相与节气的处理
	long double d, xn, jd2 = Bd0 + dt_T(Bd0) - (8.0 / 24.0);
	//月相查找
	w = XL::MS_aLon(jd2 / 36525, 10, 3);
	w = int2((w - 0.78) / PI * 2) * PI / 2;
	do {
		d = so_accurate(w);
		D = int2(d + 0.5);
		xn = int2(w / pi2 * 4 + 4000000.01) % 4;
		w += pi2 / 4;
		if (D >= Bd0 + Bdn) break;
		if (D < Bd0) continue;
		Day& ob = lun[D - Bd0];
		ob.yxmc = xn; //取得月相名称
		ob.yxjd = d;
		ob.yxsj = JD::timeStr(d);
	} while (D + 5 < Bd0 + Bdn);

	//节气查找
	w = XL::S_aLon(jd2 / 36525, 3);
	w = int2((w - 0.13) / pi2 * 24) *pi2 / 24;
	do {
		d = qi_accurate(w);
		D = int2(d + 0.5);
		xn = int2(w / pi2 * 24 + 24000006.01) % 24;
		w += pi2 / 24;
		if (D >= Bd0 + Bdn) break;
		if (D < Bd0) continue;
		Day& ob = lun[D - Bd0];
		ob.jqmc = xn; //取得节气名称
		ob.jqjd = d;
		ob.jqsj = JD::timeStr(d);
	} while (D + 12 < Bd0 + Bdn);
	return month;
}
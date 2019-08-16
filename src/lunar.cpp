#include "lunar.h"
#include "const.h"
#include "JD.h"
#include "eph.h"
#include <cmath>


//获取干支索引
short getGanZhiIndex(GZ value)
{
	short index = 0;
	for (int i = 0; i < 6; ++i)
	{
		if ((value.tg + i * 10) % 12 == value.dz) {
			index = value.tg + i * 10;
			break;
		}
	}
	return index;
}

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

//        for(auto it = mSSQ.ZQ.begin(); it != mSSQ.ZQ.end(); ++it)
//        {
//            day.cur_jq.push_back( *it );
//        }


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

//通过四柱获取年月日
std::vector<double> Lunar::siZhu2Year(GZ yearGz, GZ  yueGz, GZ  riGz, GZ  shiGz, int fromYear, int  toYear)
{
	auto fromDiff = fromYear - 1984;

	///月
	/*
	甲己之年丙作首，乙庚之岁戊为头。
	丙辛岁首寻庚起，丁壬壬位顺行流。
	若言戊癸何方求，甲寅之上好追求*/

	int startYueTg = 0, startYueDz = 0;
	if (yearGz.tg == 0 || yearGz.tg == 5)
	{
		startYueTg = 2;
		startYueDz = 2;
	}


	if (yearGz.tg == 1 || yearGz.tg == 6)
	{
		startYueTg = 4;
		startYueDz = 2;
	}

	if (yearGz.tg == 2 || yearGz.tg == 7)
	{
		startYueTg = 6;
		startYueDz = 2;
	}

	if (yearGz.tg == 3 || yearGz.tg == 8)
	{
		startYueTg = 8;
		startYueDz = 2;
	}

	if (yearGz.tg == 4 || yearGz.tg == 9)
	{
		startYueTg = 0;
		startYueDz = 2;
	}

	auto mayBeDzMoth = yueGz.dz - startYueDz;
	if (mayBeDzMoth < 0)
	{
		mayBeDzMoth = mayBeDzMoth + 12;
	}

	auto mayBeTGMoth = yueGz.tg - startYueTg;
	if (mayBeTGMoth < 0)
	{
		mayBeTGMoth = mayBeTGMoth + 10;
	}

	std::vector<double> ret;
	//说明没有合适的
	if (!(mayBeTGMoth == mayBeDzMoth || mayBeTGMoth + 10 == mayBeDzMoth))
	{
		return ret;
	}

	///时
	//    甲己日起甲子时
	//    乙庚日起丙子时
	//    丙辛日起戊子时
	//    丁壬日起庚子时
	//    戊葵日起壬子时

	uint8_t startHourtg = 0;
	if (riGz.tg == 0 || riGz.tg ==  5) {
		startHourtg = 0;
	}
	if (riGz.tg ==  1 || riGz.tg ==  6) {
		startHourtg = 2;
	}
	if (riGz.tg ==  2 || riGz.tg == 7) {
		startHourtg = 4;
	}

	if (riGz.tg ==  3 || riGz.tg ==  8) {
		startHourtg = 6;
	}

	if (riGz.tg ==  4 || riGz.tg ==  9) {
		startHourtg = 8;
	}


	auto mayBeTGHour = shiGz.tg - startHourtg;
	if (mayBeTGHour < 0)
	{
		mayBeTGHour = mayBeTGHour + 10;
	}

	
	//说明没有合适的
	if (!(mayBeTGHour == shiGz.dz || mayBeTGHour + 10 == shiGz.dz
		|| (shiGz.dz ==12 && mayBeTGHour + 10 == 12))) //晚子时
	{
		return ret;
	}


	///年
	GZ fromYearGz;
	if (fromDiff < 0)
	{
		fromYearGz.tg = fromDiff * -1 % 10;
		if (fromYearGz.tg > 0)
		{
			fromYearGz.tg = 10 - fromYearGz.tg;
		}
		fromYearGz.dz = fromDiff * -1 % 12;

		if (fromYearGz.dz > 0)
		{
			fromYearGz.dz = 12 - fromYearGz.dz;
		}
	}
	else {
		fromYearGz.tg = fromDiff  % 10;
		fromYearGz.dz = fromDiff  % 12;
	}

	

	//获取起始年天干支所在的位置
	auto fromGzPos = getGanZhiIndex(fromYearGz);
	//所查询的年需要的天干地支位置
	auto needGzPos = getGanZhiIndex(yearGz);

	int startMatchBeYear = 60;
	if (needGzPos >= fromGzPos) {
		startMatchBeYear = needGzPos - fromGzPos + fromYear;
	}
	else {
		startMatchBeYear = needGzPos + (60 - fromGzPos) + fromYear;
	}

	std::vector<int> matchYears;

	int loop = 0;
	while (startMatchBeYear + loop * 60 <= toYear)
	{
		matchYears.push_back(startMatchBeYear + loop * 60);
		loop += 1;
	}

	//理论是是第几个小时 (晚子时算成13)
	int hour  = (shiGz.dz % 13) * 2 - 1;
	if (hour < 0) hour = 0;
	

	//获取这个月应当需要落下的节气
	int jiqiIndex = 3 + (mayBeDzMoth * 2);
	bool needAddOne = false;//需要算到下一年
	if (jiqiIndex > 24)
	{
		jiqiIndex = jiqiIndex - 24;
		needAddOne = true;
	}

	//遍历符条件的年
	for (auto it = matchYears.begin(); it != matchYears.end(); ++it)
	{
		int year = *it;
		if (needAddOne)
		{
			year = year + 1;
		}

		//计算1月1号的信息
		Time t;
		t.h = 12, t.m = 0, t.s = 0.1;
		t.Y = year; t.M = 9; t.D = 1;

		//公历月首的儒略日,中午;
		int Bd0 = int2(JD::toJD(t)) - J2000;

		

		
		GZ startGz;
		GZ endGz;
		long double startJD = 0;
		long double endJD = 0;
		Time startT;
		Time endT;

		//纪月处理,1998年12月7(大雪)开始连续进行节气计数,0为甲子
		for (int i = 1; i >= 0; --i)
		{
			int index = jiqiIndex + 2 * i;
			//定节气范围
			if (i == 1 && jiqiIndex == 23)
			{
				if (!mSSQ.ZQ.size() || Bd0 + 360 < mSSQ.ZQ[0] || Bd0 + 360 >= mSSQ.ZQ[24])
				{
					mSSQ.calcY(Bd0 + 360);
				}
				index = 1;
			}
			else {
				if (!mSSQ.ZQ.size() || Bd0 < mSSQ.ZQ[0] || Bd0 >= mSSQ.ZQ[24])
				{
					mSSQ.calcY(Bd0);
				}
			}


			

			int mk = int2((mSSQ.ZQ[index] - mSSQ.ZQ[0]) / 30.43685);
			//相对大雪的月数计算,mk的取值范围0-12
			if (mk < 12 && mSSQ.ZQ[index] >= mSSQ.ZQ[2 * mk + 1])
			{
				mk++;
			}

			int D = mk + int2((mSSQ.ZQ[12] + 390) / 365.2422) * 12 + 900000; //相对于1998年12月7(大雪)的月数,900000为正数基数

			////纪日,2000年1月7日起算
			D = mSSQ.ZQ[index] - 6 + 9000000;
			
			if (i == 0)
			{
				startJD = mSSQ.ZQ[index];
				startT = JD::JD2DD(startJD + J2000);
				startGz.tg = D % 10;
				startGz.dz = D % 12;

				//获取准确节气的时间
				auto jd2 = mSSQ.ZQ[0] + dt_T(mSSQ.ZQ[0]) - (8.0 / 24.0);
				auto w = XL::S_aLon(jd2 / 36525, 3);
				w = int2((w - 0.13) / pi2 * 24) *pi2 / 24;


				for (int i = 0; i <= index; ++i)
				{
					long double  d = 0;
					while (true)
					{
						d = qi_accurate(w);
						D = int2(d + 0.5);
						auto xn = int2(w / pi2 * 24 + 24000006.01) % 24;
						w += pi2 / 24;
						if (D < mSSQ.ZQ[i]) continue;
						break;
					}

					if (index != i) continue;
					Time t1 = JD::JD2DD(d);
					startT.h = t1.h;
					startT.m = t1.m;
					startT.s = t1.s;
					break;
				}
			}
			else {
				endJD = mSSQ.ZQ[index];
				endT = JD::JD2DD(endJD + J2000);
				endGz.tg = D % 10;
				endGz.dz = D % 12;

				//获取准确节气的时间
				auto jd2 = mSSQ.ZQ[0] + dt_T(mSSQ.ZQ[0]) - (8.0 / 24.0);
				auto w = XL::S_aLon(jd2 / 36525, 3);
				w = int2((w - 0.13) / pi2 * 24) *pi2 / 24;


				for (int i = 0; i <= index; ++i)
				{
					long double  d = 0;
					while (true)
					{
						d = qi_accurate(w);
						D = int2(d + 0.5);
						auto xn = int2(w / pi2 * 24 + 24000006.01) % 24;
						w += pi2 / 24;
						if (D < mSSQ.ZQ[i]) continue;
						break;
					}

					if (index != i) continue;
					Time t1 = JD::JD2DD(d);
					endT.h = t1.h;
					endT.m = t1.m;
					endT.s = t1.s;
					break;
				}
			}
		}

		int diff = getGanZhiIndex(riGz) - getGanZhiIndex(startGz);
		long double startDay = 0;
		if (diff  <  0)
		{
        
			diff =  60 + diff;
		}
		
		startDay = startJD + diff;
	
		/*Time st = JD::JD2DD(startJD + J2000);
		Time et = JD::JD2DD(endJD + J2000);*/
		
		do {

			Time mayBet = JD::JD2DD(startDay + J2000);
			mayBet.h = hour;
			mayBet.m = 0;
			mayBet.s = 0;

			if (diff == 0)
			{
				bool isMatch = false;
				

				//此时算上一个月的
				if (hour > t.h)
				{
					isMatch = true;
				}
				else
				{
					if (hour != 0 || hour != 23)
					{
						mayBet.h = hour;
						mayBet.m = 59;
						mayBet.s = 0;

					}
					else
					{
						mayBet.h = hour;
						mayBet.m = 59;
						mayBet.s = 0;
					}


					if (mayBet.h >= t.h && t.m < 59)
					{
						isMatch = true;
					}
				}

				if (!isMatch)
				{
					break;
				}
			}

			else if (diff == int2(endJD - startJD))
			{
				bool isMatch = false;
				
				//此时算上一个月的
				if (hour < endT.h)
				{
					isMatch = true;
				}
				else
				{
					if (mayBet.h == endT.h && endT.m > 0)
					{
						isMatch = true;
					}
				}

				if (!isMatch)
				{
					break;
				}
			}

			if (diff > int2(endJD - startJD))
			{
				break;
			}

			ret.push_back(JD::toJD(mayBet));
		} while (false);

	}
	return ret;

}


Day Lunar::getDayBySolar(int _year, uint8_t _month, uint8_t _day)
{
	if ((_month > 12 || _month <= 0))
	{
		throw  LunarException(ErrorCode_DateError);
	}

	static int num[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
	if (_day <= 0 || _day > num[_month])
	{
        int year = _year;
        if(year < 0) year++; //公元前润年  
		if (!(year % 4 == 0 && _month == 2 && _day == 29)
                || year == 0) //公元记年法没有0年，公元从1年开始，公元前从-1开始  
		{
			throw LunarException(ErrorCode_DateError);
		}
	}

	Month month;
	int  Bd0, Bdn;


	Time t;
	t.h = 12, t.m = 0, t.s = 0.1;
	t.Y = _year; t.M = _month; t.D = 1;

	//公历月首的儒略日,中午;
	Bd0 = int2(JD::toJD(t)) - J2000;

	t.M++;

	if (t.M > 12) t.Y++, t.M = 1;

	//本月天数(公历);
	Bdn = int2(JD::toJD(t)) - J2000 - Bd0;

	//本月第一天的星期;
	int w0 = (Bd0 + J2000 + 1 + 7000000) % 7;


	////所属公历年对应的农历干支纪年
	int  c = _year - 1984 + 12000;

	//年天干地支
	month.yearGan = c % 10;
	month.yearZhi = c % 12;

	//年生肖
	month.ShX = c % 12;


	uint8_t i = _day - 1;
	//提取各日信息
	Day day;
	day.d0 = Bd0 + i; //儒略日,北京时12:00
	day.di = i;  //公历月内日序数
	day.y = _year; //公历年
	day.m = _month; //公历月
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

  /*  for(auto it = mSSQ.ZQ.begin(); it != mSSQ.ZQ.end(); ++it)
	{
		day.cur_jq.push_back(  *it );
	}*/


	if (day.d0 == mSSQ.HS[mk] || day.d0 == Bd0) { //月的信息
		day.Lmc = mSSQ.ym[mk]; //月名称
		day.Ldn = mSSQ.dx[mk]; //月大小
		day.Lleap = (mSSQ.leap&&mSSQ.leap == mk); //闰状况
		day.Lmc2 = mk < 13 ? mk + 1 : -1; //下个月名称,判断除夕时要用到
	}
	else {
		Day day2 = getDayBySolar(_year, _month, _day - 1);
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


	int  D = mSSQ.ZQ[3] + (day.d0 < mSSQ.ZQ[3] ? -365 : 0) + 365.25 * 16 - 35; //以立春为界定纪年
	day.Lyear = floor(D / 365.2422 + 0.5); //农历纪年(10进制,1984年起算)
	//以下几行以正月初一定年首
	D = mSSQ.HS[2]; //一般第3个月为春节
	for (int j = 0; j < 14; j++) { //找春节
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
    //一年固定有12个中气日，平均每个中气日间隔30.43685天
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

	//以下是月相与节气的处理
	long double d, xn, jd2 = Bd0 + dt_T(Bd0) - (8.0 / 24.0);
	//月相查找
	long double w = XL::MS_aLon(jd2 / 36525, 10, 3);
	w = int2((w - 0.78) / PI * 2) * PI / 2;
	do {
		d = so_accurate(w);
		D = int2(d + 0.5);
		xn = int2(w / pi2 * 4 + 4000000.01) % 4;
		w += pi2 / 4;
		if (D >= Bd0 + Bdn) break;
		if (D < Bd0) continue;
		if (D - Bd0 == _day - 1)
		{
			day.yxmc = xn; //取得月相名称
			day.yxjd = d;
			day.yxsj = JD::timeStr(d);
			break;
		}

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

		if (D - Bd0 == _day - 1)
		{
			day.jqmc = xn; //取得节气名称
			day.jqjd = d;
			day.jqsj = JD::timeStr(d);
			break;
		}

	} while (D + 12 < Bd0 + Bdn);


	//获取准确节气的时间
	jd2 = mSSQ.ZQ[0] + dt_T(mSSQ.ZQ[0]) - (8.0 / 24.0);
	w = XL::S_aLon(jd2 / 36525, 3);
	w = int2((w - 0.13) / pi2 * 24) *pi2 / 24;


	for (auto it = mSSQ.ZQ.begin(); it != mSSQ.ZQ.end(); ++it)
	{
		while (true)
		{
			d = qi_accurate(w);
			D = int2(d + 0.5);
			xn = int2(w / pi2 * 24 + 24000006.01) % 24;
			w += pi2 / 24;
			if (D < *it) continue;
			break;

		}
		Time t1 = JD::JD2DD(d);
		Time t2 = JD::JD2DD(*it);

		t2.h = t1.h;
		t2.m = t1.m;
		t2.s = t1.s;


		auto jd = JD::toJD(t2);
		Time t3 = JD::JD2DD(jd + J2000);

		day.cur_jq.push_back(jd);
		day.cur_cn.push_back((int)xn);
	}


	return day;
}


Day Lunar::getDayByLunar(int year, uint8_t month, uint8_t day, bool isRun)
{
	if (month > 10)
	{
		year = year + 1;
	}

	if (month > 12 || month < 0)
	{
		throw LunarException(ErrorCode_DateError);
	}


	//计算1月1号的信息
	Time t;
	t.h = 12, t.m = 0, t.s = 0.1;
	t.Y = year; t.M = 1; t.D = 1;

	//公历月首的儒略日,中午;
	int Bd0 = int2(JD::toJD(t)) - J2000;

	if (!mSSQ.ZQ.size() || Bd0 < mSSQ.ZQ[0] || Bd0 >= mSSQ.ZQ[24])
	{
		mSSQ.calcY(Bd0);
	}

	//{ "十一", "十二", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十" }
   // static int mkIndex[] = { 11, 12, 1,2,3,4,5,6,7, 8,9,10 };
	static int yueIndex[] = { 11, 12, 1,2,3,4,5,6,7, 8,9,10 };


	int yue = 0;

	for (int i = 0; i < sizeof(yueIndex); ++i)
	{
		if (*(yueIndex + i) == month)
		{
			yue = i;
			break;
		}
	}

	int mk = 0;
	int leap = mSSQ.leap - 1;

	if (isRun && ((leap < 0) || (leap >= 0 && month != yueIndex[leap])))
	{
		throw LunarException(ErrorCode_NotRun);
	}

	for (auto it = mSSQ.ym.begin(); it != mSSQ.ym.end(); ++it)
	{

		if (leap < 0)
		{
			if (*it == yue)
			{
				break;
			}
		}
		else {
			if (yue < leap && *it == yue)
			{
				break;
			}

			if (yue == leap && *it == yue && isRun)
			{
				++mk;
				break;
			}

			if (yue == leap && *it == yue && !isRun)
			{
				break;
			}

			if (yue > leap && *it == yue)
			{
				break;
			}

		}
		++mk;
	}

	//    if(mSSQ.leap > 0 && (mSSQ.leap < month || (isRun && mSSQ.leap == month)))
	//    {
	//        mk = mSSQ.ym[month + 1];
	//    }else{
	//        mk = mSSQ.ym[month];
	//    }




	//    static int mkIndex[] = { 0, 2, 3,4,5,6,7,8,9, 10,11,0,1 };
	//    mk = mkIndex[month];


	//    if ((isRun  &&  (mSSQ.leap&&mSSQ.leap ==  mkIndex[month + 1]) ) ||
	//        (mSSQ.leap > 0 && mSSQ.leap < mkIndex[month + 1 ]) )
	//    {
	//         mk = mkIndex[month + 1];
	//    }

		//阴历首月的儒略日
	int bdi = mSSQ.HS[mk];

	//mk会不会超出超出预期？
	if (mSSQ.HS[mk + 1] - mSSQ.HS[mk] < day || day <= 0)
	{
		throw LunarException(ErrorCode_DateError);
	}

	int jd = bdi + day - 1;

	t = JD::JD2DD(J2000 + jd);

	return getDayBySolar(t.Y, t.M, t.D);
}


GZ  Lunar::getShiGz(uint8_t dayTg, uint8_t hour)
{
	GZ ret;
	//    甲己日起甲子时
	//    乙庚日起丙子时
	//    丙辛日起戊子时
	//    丁壬日起庚子时
	//    戊葵日起壬子时

	uint8_t step = (hour + 1) / 2;
	if (step >= 12) {
		ret.dz = 0;
	}

	switch (dayTg) {
	case 0:
	case 5:
		ret.tg = 0 + step;
		break;
	case 1:
	case 6:
		ret.tg = 2 + step;
		break;

	case 2:
	case 7:
		ret.tg = 4 + step;
		break;
	case 3:
	case 8:
		ret.tg = 6 + step;
		break;
	case 4:
	case 9:
		ret.tg = 8 + step;
		break;
	default:
		break;
	}
	ret.tg = ret.tg % 10;
	ret.dz = step % 12;
	return ret;
}

Year Lunar::getYearCal(int By)
{
	Year ret;
	ret.y = By;
	////所属公历年对应的农历干支纪年
	int  c = By - 1984 + 12000;

	//年天干地支
	ret.yearGan = c % 10;
	ret.yearZhi = c % 12;

	//年生肖
	ret.ShX = c % 12;

	return ret;
}


uint8_t Lunar::getRunMonth(int By)
{
	//计算1月1号的信息
	Time t;
	t.h = 12, t.m = 0, t.s = 0.1;
	t.Y = By; t.M = 1; t.D = 1;

	//公历月首的儒略日,中午;
	int Bd0 = int2(JD::toJD(t)) - J2000;

	if (!mSSQ.ZQ.size() || Bd0 < mSSQ.ZQ[0] || Bd0 >= mSSQ.ZQ[24])
	{
		mSSQ.calcY(Bd0);
	}

	//{ "十一", "十二", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十" }
	// static int mkIndex[] = { 11, 12, 1,2,3,4,5,6,7, 8,9,10 };
	static int yueIndex[] = { 11, 12, 1,2,3,4,5,6,7, 8,9,10 };

    //需要排除11月和12月的，这个可能属于上一个月的信息
	int leap = mSSQ.leap - 1;
    if(leap > 1)
    {
        return yueIndex[leap];
    }
	
    //看看11月和12月是否有闰
    t.Y = By + 1;
    Bd0 = int2(JD::toJD(t)) - J2000;
    if (!mSSQ.ZQ.size() || Bd0 < mSSQ.ZQ[0] || Bd0 >= mSSQ.ZQ[24])
    {
        mSSQ.calcY(Bd0);
    }
    leap = mSSQ.leap - 1;
    if (leap > 1 || leap < 0)
    {
        return 0;
    }
    return yueIndex[leap];
    

}

uint8_t Lunar::getLunarMonthNum(int year, uint8_t month, bool isRun)
{
	if (month > 10)
	{
		year = year + 1;
	}


	//计算1月1号的信息
	Time t;
	t.h = 12, t.m = 0, t.s = 0.1;
	t.Y = year; t.M = 1; t.D = 1;

	//公历月首的儒略日,中午;
	int Bd0 = int2(JD::toJD(t)) - J2000;

	if (!mSSQ.ZQ.size() || Bd0 < mSSQ.ZQ[0] || Bd0 >= mSSQ.ZQ[24])
	{
		mSSQ.calcY(Bd0);
	}

	//{ "十一", "十二", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十" }
	// static int mkIndex[] = { 11, 12, 1,2,3,4,5,6,7, 8,9,10 };
	static int yueIndex[] = { 11, 12, 1,2,3,4,5,6,7, 8,9,10 };


	int yue = 0;

	for (int i = 0; i < sizeof(yueIndex); ++i)
	{
		if (*(yueIndex + i) == month)
		{
			yue = i;
			break;
		}
	}

	int mk = 0;
	int leap = mSSQ.leap - 1;

	if (isRun && ((leap < 0) || (leap >= 0 && month != yueIndex[leap])))
	{
		throw LunarException(ErrorCode_NotRun);
	}

	for (auto it = mSSQ.ym.begin(); it != mSSQ.ym.end(); ++it)
	{

		if (leap < 0)
		{
			if (*it == yue)
			{
				break;
			}
		}
		else {
			if (yue < leap && *it == yue)
			{
				break;
			}

			if (yue == leap && *it == yue && isRun)
			{
				++mk;
				break;
			}

			if (yue == leap && *it == yue && !isRun)
			{
				break;
			}

			if (yue > leap && *it == yue)
			{
				break;
			}

		}
		++mk;
	}


	//阴历首月的儒略日
	int bdi = mSSQ.HS[mk];

	return mSSQ.HS[mk + 1] - mSSQ.HS[mk];
}

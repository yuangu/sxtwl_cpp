#include "sxtwl.h"
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
		if ((value.tg + i * 10) % 12 == value.dz)
		{
			index = value.tg + i * 10;
			break;
		}
	}
	return index;
}

namespace sxtwl
{

	Day *fromSolar(int year, uint8_t month, int day)
	{
		return Day::fromSolar(year, month, day);
	}

	Day *fromLunar(int year, uint8_t month, int day, bool isRun)
	{
		return Day::fromLunar(year, month, day, isRun);
	}

	//通过四柱获取年月日
	std::vector<double> siZhu2Year(GZ yearGz, GZ yueGz, GZ riGz, GZ shiGz, int fromYear, int toYear)
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
		if (riGz.tg == 0 || riGz.tg == 5)
		{
			startHourtg = 0;
		}
		if (riGz.tg == 1 || riGz.tg == 6)
		{
			startHourtg = 2;
		}
		if (riGz.tg == 2 || riGz.tg == 7)
		{
			startHourtg = 4;
		}

		if (riGz.tg == 3 || riGz.tg == 8)
		{
			startHourtg = 6;
		}

		if (riGz.tg == 4 || riGz.tg == 9)
		{
			startHourtg = 8;
		}

		auto mayBeTGHour = shiGz.tg - startHourtg;
		if (mayBeTGHour < 0)
		{
			mayBeTGHour = mayBeTGHour + 10;
		}

		//说明没有合适的
		if (!(mayBeTGHour == shiGz.dz || mayBeTGHour + 10 == shiGz.dz || (shiGz.dz == 12 && mayBeTGHour + 10 == 12))) //晚子时
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
		else
		{
			fromYearGz.tg = fromDiff % 10;
			fromYearGz.dz = fromDiff % 12;
		}

		//获取起始年天干支所在的位置
		auto fromGzPos = getGanZhiIndex(fromYearGz);
		//所查询的年需要的天干地支位置
		auto needGzPos = getGanZhiIndex(yearGz);

		int startMatchBeYear = 60;
		if (needGzPos >= fromGzPos)
		{
			startMatchBeYear = needGzPos - fromGzPos + fromYear;
		}
		else
		{
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
		int hour = (shiGz.dz % 13) * 2 - 1;
		if (hour < 0)
			hour = 0;

		//获取这个月应当需要落下的节气
		int jiqiIndex = 3 + (mayBeDzMoth * 2);
		bool needAddOne = false; //需要算到下一年
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
			t.Y = year;
			t.M = 9;
			t.D = 1;

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
					if (!SSQPtr->ZQ.size() || Bd0 + 360 < SSQPtr->ZQ[0] || Bd0 + 360 >= SSQPtr->ZQ[24])
					{
						SSQPtr->calcY(Bd0 + 360);
					}
					index = 1;
				}
				else
				{
					if (!SSQPtr->ZQ.size() || Bd0 < SSQPtr->ZQ[0] || Bd0 >= SSQPtr->ZQ[24])
					{
						SSQPtr->calcY(Bd0);
					}
				}

				int mk = int2((SSQPtr->ZQ[index] - SSQPtr->ZQ[0]) / 30.43685);
				//相对大雪的月数计算,mk的取值范围0-12
				if (mk < 12 && SSQPtr->ZQ[index] >= SSQPtr->ZQ[2 * mk + 1])
				{
					mk++;
				}

				int D = mk + int2((SSQPtr->ZQ[12] + 390) / 365.2422) * 12 + 900000; //相对于1998年12月7(大雪)的月数,900000为正数基数

				////纪日,2000年1月7日起算
				D = SSQPtr->ZQ[index] - 6 + 9000000;

				if (i == 0)
				{
					startJD = SSQPtr->ZQ[index];
					startT = JD::JD2DD(startJD + J2000);
					startGz.tg = D % 10;
					startGz.dz = D % 12;

					//获取准确节气的时间
					auto jd2 = SSQPtr->ZQ[0] + dt_T(SSQPtr->ZQ[0]) - (8.0 / 24.0);
					auto w = XL::S_aLon(jd2 / 36525, 3);
					w = int2((w - 0.13) / pi2 * 24) * pi2 / 24;

					for (int i = 0; i <= index; ++i)
					{
						long double d = 0;
						while (true)
						{
							d = qi_accurate(w);
							D = int2(d + 0.5);
							auto xn = int2(w / pi2 * 24 + 24000006.01) % 24;
							w += pi2 / 24;
							if (D < SSQPtr->ZQ[i])
								continue;
							break;
						}

						if (index != i)
							continue;
						Time t1 = JD::JD2DD(d);
						startT.h = t1.h;
						startT.m = t1.m;
						startT.s = t1.s;
						break;
					}
				}
				else
				{
					endJD = SSQPtr->ZQ[index];
					endT = JD::JD2DD(endJD + J2000);
					endGz.tg = D % 10;
					endGz.dz = D % 12;

					//获取准确节气的时间
					auto jd2 = SSQPtr->ZQ[0] + dt_T(SSQPtr->ZQ[0]) - (8.0 / 24.0);
					auto w = XL::S_aLon(jd2 / 36525, 3);
					w = int2((w - 0.13) / pi2 * 24) * pi2 / 24;

					for (int i = 0; i <= index; ++i)
					{
						long double d = 0;
						while (true)
						{
							d = qi_accurate(w);
							D = int2(d + 0.5);
							auto xn = int2(w / pi2 * 24 + 24000006.01) % 24;
							w += pi2 / 24;
							if (D < SSQPtr->ZQ[i])
								continue;
							break;
						}

						if (index != i)
							continue;
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
			if (diff < 0)
			{

				diff = 60 + diff;
			}

			startDay = startJD + diff;

			/*Time st = JD::JD2DD(startJD + J2000);
		Time et = JD::JD2DD(endJD + J2000);*/

			do
			{

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

	GZ getShiGz(uint8_t dayTg, uint8_t hour)
	{
		GZ ret;
		//    甲己日起甲子时
		//    乙庚日起丙子时
		//    丙辛日起戊子时
		//    丁壬日起庚子时
		//    戊葵日起壬子时

		uint8_t step = (hour + 1) / 2;
		if (step >= 12)
		{
			ret.dz = 0;
		}

		switch (dayTg)
		{
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

	uint8_t getRunMonth(int By)
	{
		//计算1月1号的信息
		Time t;
		t.h = 12, t.m = 0, t.s = 0.1;
		t.Y = By;
		t.M = 1;
		t.D = 1;

		//公历月首的儒略日,中午;
		int Bd0 = int2(JD::toJD(t)) - J2000;

		if (!SSQPtr->ZQ.size() || Bd0 < SSQPtr->ZQ[0] || Bd0 >= SSQPtr->ZQ[24])
		{
			SSQPtr->calcY(Bd0);
		}

		//{ "十一", "十二", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十" }
		// static int mkIndex[] = { 11, 12, 1,2,3,4,5,6,7, 8,9,10 };
		static int yueIndex[] = {11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

		//需要排除11月和12月的，这个可能属于上一个月的信息
		int leap = SSQPtr->leap - 1;
		if (leap > 1)
		{
			return yueIndex[leap];
		}

		//看看11月和12月是否有闰
		t.Y = By + 1;
		Bd0 = int2(JD::toJD(t)) - J2000;
		if (!SSQPtr->ZQ.size() || Bd0 < SSQPtr->ZQ[0] || Bd0 >= SSQPtr->ZQ[24])
		{
			SSQPtr->calcY(Bd0);
		}
		leap = SSQPtr->leap - 1;
		if (leap > 1 || leap < 0)
		{
			return 0;
		}
		return yueIndex[leap];
	}

	uint8_t getLunarMonthNum(int year, uint8_t month, bool isRun)
	{
		if (month > 10)
		{
			year = year + 1;
		}

		//计算1月1号的信息
		Time t;
		t.h = 12, t.m = 0, t.s = 0.1;
		t.Y = year;
		t.M = 1;
		t.D = 1;

		//公历月首的儒略日,中午;
		int Bd0 = int2(JD::toJD(t)) - J2000;

		if (!SSQPtr->ZQ.size() || Bd0 < SSQPtr->ZQ[0] || Bd0 >= SSQPtr->ZQ[24])
		{
			SSQPtr->calcY(Bd0);
		}

		//{ "十一", "十二", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十" }
		// static int mkIndex[] = { 11, 12, 1,2,3,4,5,6,7, 8,9,10 };
		static int yueIndex[] = {11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

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
		int leap = SSQPtr->leap - 1;

		if (isRun && ((leap < 0) || (leap >= 0 && month != yueIndex[leap])))
		{
			//throw CalendarException(ErrorCode_NotRun);
		}

		for (auto it = SSQPtr->ym.begin(); it != SSQPtr->ym.end(); ++it)
		{

			if (leap < 0)
			{
				if (*it == yue)
				{
					break;
				}
			}
			else
			{
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
		int bdi = SSQPtr->HS[mk];

		return SSQPtr->HS[mk + 1] - SSQPtr->HS[mk];
	}
	
	//儒略日数转公历
	Time JD2DD(double jd)
	{
		return JD::JD2DD(jd);
	}

	//公历转儒略日
	double toJD(Time& time)
	{
		return JD::toJD(time);
	}
}

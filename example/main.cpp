#include <iostream>
#include <sstream>
#include <vector>
#include "const.h"
#include "lunar.h"
#include "JD.h"

//===============================================================
#ifdef _WIN32
class MBuf : public std::stringbuf
{
  public:
	int sync()
	{
		fputs(str().c_str(), stdout);
		str("");
		return 0;
	}
};
#endif
//===============================================================

static const char *Gan[] = {"甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"};
static const char *Zhi[] = {"子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"};
static const char *ShX[] = {"鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗", "猪"};
static const char *numCn[] = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"}; //中文数字
static const char *jqmc[] = {"冬至", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种", "夏至", "小暑", "大暑", "立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪"};
static const char *ymc[] = {"十一", "十二", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十"}; //月名称,建寅
static const char *rmc[] = {"初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十", "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十", "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十", "卅一"};

void printDay(Day &day)
{
	std::cout << "\n===================================================" << std::endl;
	;
	std::cout << "公历：" << day.y << "年" << (int)day.m << "月" << day.d << "日" << std::endl;
	if (day.Lleap)
	{
		std::cout << "润" << ymc[day.Lmc] << "月" << rmc[day.Ldi] << "日" << std::endl;
	}
	else
	{
		std::cout << ymc[day.Lmc] << "月" << rmc[day.Ldi] << "日" << std::endl;
	}

	std::cout << "儒略历:JD" << J2000 + day.d0 << std::endl;
	std::cout << "星期" << numCn[day.week] << std::endl;

	std::cout << Gan[day.Lyear2.tg] << Zhi[day.Lyear2.dz] << "年"
			  << Gan[day.Lmonth2.tg] << Zhi[day.Lmonth2.dz] << "月"
			  << Gan[day.Lday2.tg] << Zhi[day.Lday2.dz] << "日"
			  << std::endl;

	std::cout << "距冬至" << day.cur_dz << "天" << std::endl;
	std::cout << "距夏至" << day.cur_xz << "天" << std::endl;
	std::cout << "距立秋" << day.cur_lq << "天" << std::endl;
	std::cout << "距芒种" << day.cur_mz << "天" << std::endl;
	std::cout << "距小暑" << day.cur_xs << "天" << std::endl;
};

int main()
{
	
#ifdef _WIN32
	////http://m.blog.csdn.net/article/details?id=52789570
	//https://stackoverflow.com/questions/45575863/how-to-print-utf-8-strings-to-stdcout-on-windows
	setvbuf(stdout, nullptr, _IONBF, 0);
	MBuf buf;
	std::cout.rdbuf(&buf);
#endif
	Lunar lunar;


	// 根据天干地支反查
	///1990-6-15 庚午年 壬午月 辛亥日

	GZ yearGz(6, 6);
	GZ yueGz(8, 6);
	GZ riGz(7, 11);
	GZ shiGz(2, 8);
	lunar.siZhu2Year(yearGz, yueGz, riGz, shiGz, 1990, 2100);


	//=============================================================================================================
	//获取年的润月
	int run_month = lunar.getRunMonth(1990);
	//============================================================================================================
	int month_num = lunar.getLunarMonthNum(1990, 5);
	//=============================================================================================================
	//通过阳历获取当天的信息。（可以用于阳历转阴历,获取当天星期几等功能）
	try
	{
		Day day = lunar.getDayBySolar(2010, 13, 10);
		printDay(day);
	}
	catch (LunarException e)
	{
		std::cout << e.what() << std::endl;
	}

	//=============================================================================================================
	//通过阴历获取当天的信息。（可以用于阴历转阳历等,获取当天星期几等功能）
	Day day2 = lunar.getDayByLunar(1995, 12, 10);
	printDay(day2);
	//=============================================================================================================
	Month month = lunar.yueLiCalc(1990, 7);

	std::cout << month.y << "年" << (int)month.m << "月" << std::endl;
	std::cout << Gan[month.yearGan] << Zhi[month.yearZhi] << "年" << std::endl;
	std::cout << "生肖：" << ShX[month.ShX] << std::endl;

	std::vector<Day> &days = month.days;
	for (auto it = days.begin(); it != days.end(); ++it)
	{
		printDay((*it));
	}
	//=============================================================================================================
	return 0;
}

#include <iostream>
#include <sstream>
#include <vector>
#include "const.h"
#include "sxtwl.h"


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
static const char *ymc[] = { "正", "二", "三", "四", "五", "六", "七", "八", "九", "十","十一", "十二"}; //月名称,建寅
static const char *rmc[] = {"初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十", "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十", "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十", "卅一"};
static const char *WeekCn[] = {"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};


GZ getGZ(std::string tgStr, std::string dzStr) {
	int tg = -1;
	int dz = -1;
	for (size_t i = 0; i < 10; i++)
	{
		if (std::string(Gan[i]) == tgStr) {
			tg = i;
			break;
		}
	}

	for (size_t i = 0; i < 12; i++)
	{
		if (std::string(Zhi[i]) == dzStr) {
			dz = i;
			break;
		}
	}
	return GZ(tg, dz);
}



void printDay(Day& day)
{
    
	std::cout << "\n===================================================" << std::endl;
	
    // 公历
	std::cout << "公历：" << day.getSolarYear() << "年"
                << (int)day.getSolarMonth() << "月"
                << day.getSolarDay() << "日" << std::endl;
        
    // 农历
    std::cout << "农历：" << (day.isLunarLeap()? "闰":"")
                << ymc[day.getLunarMonth() - 1] << "月"
                << rmc[day.getLunarDay() - 1] << "日" << std::endl;

	// 星期几
	std::cout << "星期:" << WeekCn[day.getWeek()] << std::endl;
    
    //年天二地支
	std::cout  << "天干:"
               << Gan[day.getYearGZ().tg] << Zhi[day.getYearGZ().dz] << "年"
			   << Gan[day.getMonthGZ().tg] << Zhi[day.getMonthGZ().dz] << "月"
			   << Gan[day.getMonthGZ().tg] << Zhi[day.getMonthGZ().dz] << "日"
			   << std::endl;
};

int round_double(double number)
{
	return (number > 0.0) ? (number + 0.5) : (number - 0.5);
}

int main()
{
	
//#ifdef _WIN32
	////http://m.blog.csdn.net/article/details?id=52789570
	//https://stackoverflow.com/questions/45575863/how-to-print-utf-8-strings-to-stdcout-on-windows
	setvbuf(stdout, nullptr, _IONBF, 0);
	MBuf buf;
	std::cout.rdbuf(&buf);
//#endif
    
	


    GZ hourGZ =  sxtwl::getShiGz(0, 23, false);

	do {
		Day* day = sxtwl::fromSolar(2021, 11, 14);
		day->getHourGZ(8);
		auto ret = sxtwl::siZhu2Year(day->getYearGZ(), day->getMonthGZ(), day->getDayGZ(), getGZ("癸", "巳"), 2021, 2025);
		printf("%d", ret.size());
	} while (false);
		
	do {
		Day* day = sxtwl::fromSolar(1392, 1, 1);
		for (int i = 0; i < 365; ++i) {
			day = day->after(1);
			if (day->hasJieQi()) {
				auto  jd = day->getJieQiJD();
				auto t = sxtwl::JD2DD(jd);
				std::cout <<jqmc[day->getJieQi()] << ": " << t.getYear() << "-" << t.getMonth() << "-"
					<< t.getDay() << " " << int(t.getHour()) << ":" << int(t.getMin()) << ":" << round_double(t.getSec())
					<< std::endl;
			}
		}
	} while (false);
	

	
	
    // 获取一年当中的闰月
    for(auto i = 1; i <= 3000; ++i)
    {
        auto lunMonth = (int)sxtwl::getRunMonth(i);
        if(lunMonth <= 0) continue;
        printf("%d年 闰%d月\n", i, lunMonth);
    }
    
    //从阳历获取一个day对像
    Day* day = sxtwl::fromSolar(118, 10, 3);
    printDay(*day);
    for(auto i = 0; i < 100; ++i){
        day =  day->after(1);
        printDay(*day);
    }
    
    // 阳历转阴历
    {
        Day* day = sxtwl::fromSolar(2021, 11, 7);
        std::cout  << "农历:" << day->getLunarYear() << "年" << (int)day->getLunarMonth() << "月" << day-> getLunarDay() << "日"  << std::endl;
    
    
        day = sxtwl::fromLunar( day->getLunarYear(), day->getLunarMonth(), day-> getLunarDay(), day->isLunarLeap());
        std::cout  << "公历:" << day->getSolarYear() << "年" << (int)day->getSolarMonth() << "月" << day-> getSolarDay() << "日"  << std::endl;
        
        sxtwl::getShiGz(day->getDayGZ().tg, 0);
        
       auto c =  day->getConstellation();
       printf("%c", c);
        
        if( day->hasJieQi()){
            auto  jd = day->getJieQiJD();
            auto t = sxtwl::JD2DD(jd);
            jd = sxtwl::toJD(t);
        }
        
       auto ret = sxtwl::siZhu2Year(day->getYearGZ(), day->getMonthGZ(), day->getDayGZ(),
                          GZ(0, 0)
                          , 2003, 2029);
        

      /*  printf("finish");*/
        
    }
	{

	

		Day* day = sxtwl::fromSolar(202, 1, 20);
		if (day->hasJieQi()) {
			auto t = sxtwl::JD2DD(day->getJieQiJD());
			std::cout << jqmc[day->getJieQi()] << ": " << t.getYear() << "-" << t.getMonth() << "-"
				<< t.getDay() << " " << int(t.getHour()) << ":" << int(t.getMin()) << ":" << round_double(t.getSec())
				<< std::endl;
		}
		delete day;
		
	}


 //===========================================================================   
	auto ret = sxtwl::getJieQiByYear(202);

	for (auto it = ret.begin(); it != ret.end(); ++it) {
		auto t = sxtwl::JD2DD(it->jd);
		std::cout << jqmc[it->jqIndex] << ": " << t.getYear() << "-" << t.getMonth() << "-"
			<< t.getDay() << " " << int(t.getHour()) << ":" << int(t.getMin()) << ":" << round_double(t.getSec())
			<< std::endl;
	}

	Time t(202, 1, 23, 12, 0, 0);
	auto jd = sxtwl::toJD(t) - J2000;
//=========================================================================== 
    return 0;
}

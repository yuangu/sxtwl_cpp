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

int main()
{
	
#ifdef _WIN32
	////http://m.blog.csdn.net/article/details?id=52789570
	//https://stackoverflow.com/questions/45575863/how-to-print-utf-8-strings-to-stdcout-on-windows
	setvbuf(stdout, nullptr, _IONBF, 0);
	MBuf buf;
	std::cout.rdbuf(&buf);
#endif
    

    
    
    // 获取一年当中的闰月
    for(auto i = 1; i <= 3000; ++i)
    {
        auto lunMonth = (int)sxtwl::getRunMonth(i);
        if(lunMonth <= 0) continue;
        printf("%d年 闰%d月\n", i, lunMonth);
    }
    
    //从阳历获取一个day对像
    Day* day = sxtwl::fromLunar(2018, 12, 1);
    printDay(*day);
    for(auto i = 0; i < 100; ++i){
        day =  day->after(1);
        printDay(*day);
    }
    
    // 阳历转阴历
    {
        Day* day = sxtwl::fromSolar(2018, 12, 1);
        std::cout  << "农历:" << day->getLunarYear() << "年" << (int)day->getLunarMonth() << "月" << day-> getLunarDay() << "日"  << std::endl;
    
    
        day = sxtwl::fromLunar( day->getLunarYear(), day->getLunarMonth(), day-> getLunarDay(), day->isLunarLeap());
        std::cout  << "公历:" << day->getSolarYear() << "年" << (int)day->getSolarMonth() << "月" << day-> getSolarDay() << "日"  << std::endl;
    }
    
   
    
    return 0;
}

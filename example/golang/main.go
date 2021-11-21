package main

import (
	"fmt"
	"sxtwl_example/sxtwl"
)

/**
* 本例子只给了win64的静态库，如果需要其它库请自行编译
$ cd ~/code/sxtwl_cpp
$ mkdir build
$ cd build
$ cmake .. -G "Unix Makefiles" -DSXTWL_WRAPPER_GO=1
$ cmake --build .

编译好了，请把sxtwl.a及sxtwl_cgo.a放到lib目录下，并在sxtwl目录下的init.go加好链接路径


注: 如果使用win系统的话，请安装mingw32, 作者推荐安装tdm-gcc
然后cmake 命令改成 cmake -G "MinGW Makefiles" .. -DSXTWL_WRAPPER_GO=1
*/

var Gan = [...]string{"甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"}

var Zhi = [...]string{"子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"}
var ShX = [...]string{"鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗", "猪"}
var MumCn = [...]string{"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"}
var Jqmc = [...]string{"冬至", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏",
	"小满", "芒种", "夏至", "小暑", "大暑", "立秋", "处暑", "白露", "秋分", "寒露", "霜降",
	"立冬", "小雪", "大雪"}
var Ymc = [...]string{"十一", "十二", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十"}
var Rmc = [...]string{"初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十",
	"十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十",
	"廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十", "卅一"}
var XiZ = [...]string{"摩羯", "水瓶", "双鱼", "白羊", "金牛", "双子", "巨蟹", "狮子", "处女", "天秤", "天蝎", "射手"}
var WeekCn = [...]string{"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"}

func main() {
	day := sxtwl.DayFromSolar(2021, 11, 7)

	// day := sxtwl.DayFromLunar(2020, 12, 1)
	// 	# 如果是想查闰月，第四个参数加一个True即可
	// # day = sxtwl.DayFromLunar(2020, 12, 1, true)

	fmt.Printf("公历:%v年%v月%v日\n", day.GetSolarYear(), day.GetSolarMonth(), day.GetSolarDay())

	// 星期几
	fmt.Printf("%v\n", WeekCn[day.GetWeek()])

	// 这个月的第几周
	fmt.Printf("该日属于这个月的第%v周\n", day.GetWeekIndex())

	//以春节为界的农历(注getLunarYear如果没有传参，或者传true，是以春节为界的)
	runStr := ""
	if day.IsLunarLeap() {
		runStr = "闰"
	}
	fmt.Printf("农历:%d年%s%d月%d日\n", day.GetLunarYear(), runStr, day.GetLunarMonth(), day.GetLunarDay())

	//不以立春为界的农历
	fmt.Printf("农历:%d年%s%d月%d日\n", day.GetLunarYear(true), runStr, day.GetLunarMonth(), day.GetLunarDay())

	//剩下的例子以后再补，也可以直接参考python的例子来写

}

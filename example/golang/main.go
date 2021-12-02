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
$ cmake .. -DSXTWL_WRAPPER_GO=1
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
var Ymc = [...]string{"正", "二", "三", "四", "五", "六", "七", "八", "九", "十", "十一", "十二"}
var Rmc = [...]string{"初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十",
	"十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十",
	"廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十", "卅一"}
var XiZ = [...]string{"摩羯", "水瓶", "双鱼", "白羊", "金牛", "双子", "巨蟹", "狮子", "处女", "天秤", "天蝎", "射手"}
var WeekCn = [...]string{"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"}

//=============================================================================================
//做反推的辅助函数
// copy form https://blog.csdn.net/qianguozheng/article/details/52795395
func SubString(str string, begin, length int) string {
	fmt.Println("Substring =", str)
	rs := []rune(str)
	lth := len(rs)
	fmt.Printf("begin=%d, end=%d, lth=%d\n", begin, length, lth)
	if begin < 0 {
		begin = 0
	}
	if begin >= lth {
		begin = lth
	}
	end := begin + length

	if end > lth {
		end = lth
	}
	fmt.Printf("begin=%d, end=%d, lth=%d\n", begin, length, lth)
	return string(rs[begin:end])
}

func GetGZ(gzStr string) sxtwl.GZ {
	tg := -1
	dz := -1

	for i, v := range Gan {
		if SubString(gzStr, 0, 1) == v {
			tg = i
			break
		}
	}

	for i, v := range Zhi {
		if SubString(gzStr, 1, 1) == v {
			tg = i
			break
		}
	}

	return sxtwl.NewGZ(tg, dz)
}

//=============================================================================================

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

	// 以春节为界的天干地支
	yTG := day.GetYearGZ(true)
	fmt.Printf("以春节为界的年干支:%v", Gan[yTG.GetTg()]+Zhi[yTG.GetDz()])
	fmt.Printf("以春节为界的生肖:%v", ShX[yTG.GetDz()])

	// 以立春为界的天干地支 （注，如果没有传参，或者传false，是以立春为界的。刚好和getLunarYear相反）
	yTG = day.GetYearGZ()
	fmt.Printf("以立春为界的年干支:%v", Gan[yTG.GetTg()]+Zhi[yTG.GetDz()])
	fmt.Printf("以立春为界的生肖:%v", ShX[yTG.GetDz()])

	//月干支
	mTG := day.GetMonthGZ()
	fmt.Printf("月干支:%v", Gan[mTG.GetTg()]+Zhi[mTG.GetDz()])

	//日干支
	dTG := day.GetDayGZ()
	fmt.Printf("日干支%v", Gan[dTG.GetTg()]+Zhi[dTG.GetDz()])

	//时干支,传24小时制的时间，分早晚子时
	hour := 18
	sTG := day.GetHourGZ((byte)(hour))
	fmt.Printf("%d时的干支%v", hour, Gan[sTG.GetTg()]+Zhi[sTG.GetDz()])

	// 第二种获取时干支方法
	//第一个参数为该天的天干，第二个参数为小时
	hTG := sxtwl.GetShiGz(dTG.GetTg(), (byte)(hour))
	fmt.Printf("%d时天干地支:%v", (hour), Gan[hTG.GetTg()]+Zhi[hTG.GetDz()])

	// 当日是否有节气
	if day.HasJieQi() {
		fmt.Printf("节气：%s", Jqmc[day.GetJieQi()])
		//获取节气的儒略日数
		jd := day.GetJieQiJD()
		// 将儒略日数转换成年月日时秒
		t := sxtwl.JD2DD(jd)
		fmt.Printf("节气时间:%v", t)
		//注意，t.s是小数，需要四舍五入
		//fmt.Printf("节气时间:%d-%d-%d %d:%d:%d", t.GetY(), t.GetM(), t.GetD(), t.GetH(), t.GetM(), math.Round(t.GetS()))
	} else {
		fmt.Println("当天不是节气日")
	}

	// 四注反查 分别传的是年天干，月天干，日天干，时天干， 开始查询年，结束查询年  返回满足条件的儒略日数
	jds := sxtwl.SiZhu2Year(GetGZ("辛丑"), GetGZ("己亥"), GetGZ("丙寅"), GetGZ("癸巳"), 2003, 2029)
	for i := 0; i < int(jds.Size()); i++ {
		jd := jds.Get(i)
		t := sxtwl.JD2DD(jd)
		fmt.Printf("符合条件的时间:%v", t)
		// print("符合条件的时间:%d-%d-%d %d:%d:%d"%(t.Y, t.M, t.D, t.h, t.m, round(t.s)))
	}

	// 获取一年中的闰月
	year := 2020
	month := sxtwl.GetRunMonth(year)
	if month >= 0 {
		fmt.Printf("%d年的闰月是%d", year, month)
	} else {
		fmt.Println("没有闰月")
	}

	// 一个农历月的天数
	year = 2020    //农历年
	month = 4      //农历月
	isRun := false //是否是闰月
	daynum := sxtwl.GetLunarMonthNum(year, month, isRun)
	runStr = ""
	if isRun {
		runStr = "闰"
	}
	fmt.Printf("农历%v年%v%v月的天数:%v", year, runStr, month, daynum)

	//儒略日数转公历
	jd := sxtwl.J2000
	t := sxtwl.JD2DD((float64)(jd))

	//公历转儒略日
	// jd = sxtwl.ToJD(t)

	// 获取某天的后面几天
	num := 1             //你喜欢写多少天 也多少天，可以写负数，相当于往前
	day = day.After(num) //获取num天后的日信息
	fmt.Printf("公历:%d年%d月%d日\n", day.GetSolarYear(), day.GetSolarMonth(), day.GetSolarDay())

	//  同上
	day = day.Before(num)
	fmt.Printf("公历:%d年%d月%d日\n", day.GetSolarYear(), day.GetSolarMonth(), day.GetSolarDay())

	// 查找某日前后的节气
	for true {
		// 这里可以使用after或者before，不用担心速度，这里的计算在底层仅仅是+1这么简单
		day = day.After(1)
		// hasJieQi的接口比getJieQiJD速度要快，你也可以使用getJieQiJD来判断是否有节气。
		if day.HasJieQi() {
			fmt.Printf("节气：%s", Jqmc[day.GetJieQi()])
			//获取节气的儒略日数， 如果说你要计算什么时间的相距多少，直接比对儒略日要方便，相信我。
			jd := day.GetJieQiJD()

			// 将儒略日数转换成年月日时秒
			t = sxtwl.JD2DD(jd)
			fmt.Printf("节气时间:%v", t)
			// # 注意，t.s是小数，需要四舍五入
			// print("节气时间:%d-%d-%d %d:%d:%d"%(t.Y, t.M, t.D, t.h, t.m, round(t.s)))

			break
		}

	}

}

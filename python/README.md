### 介绍 

sxtwl_cpp是参考[寿星天文历](http://www.nongli.net/sxwnl/)并使用C++实现日历库。因为其依据天文历法算法实现，故其可查询范围广(BC722年以后与实历相符，支持1800年以前及2200年以后的日历查询)。支持Android、IOS、Windows、MacOS、Linux等平台。使用swig暴露接口给python,lua,java等语言使用。


### 安装方法

```
pip install sxtwl
```

旧工程代码兼容
如果有已使用V1.x版本的工程，想兼容代码
```
pip install sxtwl==1.1.0
```
或者在requirements.txt里修改
```
sxtwl 1.1.0
```

具体使用方法参考：
https://pypi.org/project/sxtwl/

本项目 [GitHub](https://github.com/yuangu/sxtwl_cpp) / [Gitee（码云）](https://gitee.com/yuangu/sxtwl)。


### 使用方法 

<font color='red'> 注:因为pip上传后不能二次修改，参考事例可能会有错误无法修改，如果发现下面例子不能用，请以为准: [传送门](https://github.com/yuangu/sxtwl_cpp/blob/master/example/main.py)  </font>


1. 因为考虑到繁体和简体字的原因，所以本库不以硬编码的形式显示结果。下面是参考的简单索引
```
Gan = ["甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"]
Zhi = ["子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"]
ShX = ["鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗", "猪"]
numCn = ["零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"]
jqmc = ["冬至", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏",
     "小满", "芒种", "夏至", "小暑", "大暑", "立秋", "处暑","白露", "秋分", "寒露", "霜降", 
     "立冬", "小雪", "大雪"]
ymc = ["十一", "十二", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十" ]
rmc = ["初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十", 
    "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十", 
    "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十", "卅一"]
XiZ = ['摩羯', '水瓶', '双鱼', '白羊', '金牛', '双子', '巨蟹', '狮子', '处女', '天秤', '天蝎', '射手']
WeekCn = ["星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"]
```

2. 引入本库
```
import  sxtwl

# 从公历年月日获取一天的信息
day = sxtwl.fromSolar(2021, 11, 7) 

# 从农历年月日获取一天的信息
day = sxtwl.fromLunar(2020, 12, 1)
```

3. 获取某天的信息(这里的信息有，阴历，阳历，二十四节气，天干地支，星期几等)

```
# 公历的年月日
s = "公历:%d年%d月%d日" % (day.getSolarYear(), day.getSolarMonth(), day.getSolarDay())
print(s)

# 星期几
print(WeekCn[day.getWeek()])

# 这个月的第几周
print('该日属于这个月的第%d周'%(day.getWeekIndex(),))

# 星座(有bug?待修复)
print("星座:", XiZ[day.getConstellation()])

# 以春节为界的农历(注getLunarYear如果没有传参，或者传true，是以春节为界的)
s = "农历:%d年%s%d月%d日" % (day.getLunarYear(), 
    '闰' if day.isLunarLeap() else '', day.getLunarMonth(), day.getLunarDay())
print(s)

# 以立春为界的农历
s = "农历:%d年%s%d月%d日" % (day.getLunarYear(False), 
    '闰' if day.isLunarLeap() else '', day.getLunarMonth(), day.getLunarDay())
print(s)


# 以春节为界的天干地支 
yTG = day.getYearGZ(True)
print("以春节为界的年干支", Gan[yTG.tg] + Zhi[yTG.dz]) 
print("以春节为界的生肖:", ShX[yTG.dz])

# 以立春为界的天干地支 （注，如果没有传参，或者传false，是以立春为界的。刚好和getLunarYear相反）
yTG = day.getYearGZ()
print("以立春为界的年干支", Gan[yTG.tg] + Zhi[yTG.dz]) 
print("以立春为界的生肖:", ShX[yTG.dz])

#月干支
mTG = day.getMonthGZ()
print("月干支", Gan[mTG.tg] + Zhi[mTG.dz]) 

#日干支
dTG  = day.getDayGZ()
print("日干支", Gan[dTG.tg] + Zhi[dTG.dz]) 


#时干支,传24小时制的时间，分早晚子时
hour = 18
sTG = day.getHourGZ(hour)
print("%d时的干支"%(hour, ), Gan[sTG.tg] + Zhi[sTG.dz]) 


#时干支
for hour in range(24):
    # 第一个参数为该天的天干，第二个参数为小时
    hTG  = sxtwl.getShiGz(dTG.tg, hour)
    print("%d时天干地支:"%(hour), Gan[hTG.tg] + Zhi[hTG.dz])


# 当日是否有节气
if day.hasJieQi():
    print('节气：%s'% jqmc[day.getJieQi()])
    #获取节气的儒略日数
    jd = day.getJieQiJD()
    # 将儒略日数转换成年月日时秒
    t = sxtwl.JD2DD(jd )
    
    # 注意，t.s是小数，需要四舍五入
    print("节气时间:%d-%d-%d %d:%d:%d"%(t.Y, t.M, t.D, t.h, t.m, round(t.s)))
else:
    print("当天不是节气日")

```

4. 获取某日的前几天或者后几天的信息 （可以用到很多场景中）
```
# 获取某天的后面几天
num = 1    #你喜欢写多少天 也多少天，可以写负数，相当于往前
day = day.after(num)  #获取num天后的日信息
s = "公历:%d年%d月%d日" % (day.getSolarYear(), day.getSolarMonth(), day.getSolarDay())
print(s)

# 同上
day = day.before(num)
s = "公历:%d年%d月%d日" % (day.getSolarYear(), day.getSolarMonth(), day.getSolarDay())
print(s)
```


5. 获取一年中的闰月
```
# 获取一年中的闰月
year = 2020
month = sxtwl.getRunMonth(year)
if month >= 0:
    print("%d年的闰月是%d"%(year, month) )
else:
    print("没有闰月")
```

6. 获取一个农历月的天数
```
# 一个农历月的天数
year = 2020 #农历年
month  = 4 #农历月
isRun = False #是否是闰月
daynum = sxtwl.getLunarMonthNum(year, month, isRun)
print("农历%d年%s%d月的天数:"%(year, '闰'if isRun else '', month), daynum)

```

7. 儒略日数与公历的互转
```
#儒略日数转公历
jd = sxtwl.J2000
t = sxtwl.JD2DD(jd )

#公历转儒略日
jd = sxtwl.toJD(t)
```


8. 查找某日之前或者之后的节气
```
# 查找某日前后的节气,此例为之后，之前把after替换成before
while True:
    # 这里可以使用after或者before，不用担心速度，这里的计算在底层仅仅是+1这么简单
    day = day.after(1)
    # hasJieQi的接口比getJieQiJD速度要快，你也可以使用getJieQiJD来判断是否有节气。
    if day.hasJieQi():
        print('节气：%s'% jqmc[day.getJieQi()])
        #获取节气的儒略日数， 如果说你要计算什么时间的相距多少，直接比对儒略日要方便，相信我。
        jd = day.getJieQiJD()
    
        # 将儒略日数转换成年月日时秒
        t = sxtwl.JD2DD(jd )
        
        # 注意，t.s是小数，需要四舍五入
        print("节气时间:%d-%d-%d %d:%d:%d"%(t.Y, t.M, t.D, t.h, t.m, round(t.s)))
        
        break
```

9. 四柱反查 (好像还有bug，待修复)
```
###==================================================================================
# 四柱反查工具方法
# 实际项目中不要这样子搞哈，因为汉字utf-8，GBK2312不同的编码。建议还是直接使用天干地支的数字索引 
def getGZ(gzStr):
    tg = -1
    dz = -1
    for i, v in enumerate(Gan):
        if gzStr[0]  == v:
            tg = i
            break

    for i, v in enumerate(Zhi):
        if  gzStr[1] == v:
            dz = i
            break   
    return sxtwl.GZ(tg, dz)
###==================================================================================

# 四注反查 分别传的是年天干，月天干，日天干，时天干， 开始查询年，结束查询年  返回满足条件的儒略日数
jds = sxtwl.siZhu2Year(getGZ('辛丑'), getGZ('己亥'), getGZ('丙寅'), getGZ('癸巳'), 2003, 2029);
for jd in jds:
    t = sxtwl.JD2DD(jd )
    print("符合条件的时间:%d-%d-%d %d:%d:%d"%(t.Y, t.M, t.D, t.h, t.m, round(t.s)))

```

### 联系作者及广告

作者微信二维码:

![微信二维码](https://gitee.com/yuangu/sxtwl/raw/master/doc/img/WechatIMG5.jpeg)  

请您扫码支持作者的海棠万年历(广告):

![海棠万年历](https://gitee.com/yuangu/sxtwl/raw/master/doc/img/qrcode_for_gh_d48e82be45fb_430.jpg)

如果您想请作者喝杯奶茶，扫如下微信支付码：

![微信收款](https://gitee.com/yuangu/sxtwl/raw/master/doc/img/WechatIMG7.jpeg)
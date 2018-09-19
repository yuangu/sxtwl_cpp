### 安装方法

`pip install sxtwl`


### 使用方法 


1. 因为考虑到繁体和简体字的原因，所以本库不以硬编码的形式显示结果。下面是参考的简单索引
```
Gan = ["甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"]
Zhi = ["子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"]
ShX = ["鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗", "猪"]
numCn = ["零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"]
jqmc = ["冬至", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种", "夏至", "小暑", "大暑", "立秋", "处暑","白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪"]
ymc = ["十一", "十二", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十" ]
rmc = ["初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十", "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十", "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十", "卅一"]

```

2. 引入本库
```
import  sxtwl
lunar = sxtwl.Lunar()  #实例化日历库
#下面可以使用lunar做些日历的操作
```

3. 获取某天的信息(这里的信息有，阴历，阳历，二十四节气，天干地支，星期几等)


* 通过阳历获取查询日期信息
```
day = lunar.getDayBySolar(2018, 10, 20)  # 查询2018年10月20日
```

* 通过阴历获取查询日期信息

```
day = lunar.getDayByLunar(2018, 10, 20 , False)  #查询阴历2018年10月20日的信息，最后一个False表示是否是润月，填True的时候只有当年有润月的时候才生效
```
或
```
day = lunar.getDayByLunar(2018, 10, 20 )
```


4. 处理你的日历信息（比如实现阴历转阴历，阳历转阴历）

```
print("公历:", day.y, "年", day.m, "月", day.d, "日")
if day.Lleap:
    print("润", ymc[day.Lmc], "月", rmc[day.Ldi], "日")
else:
    print(ymc[day.Lmc], "月", rmc[day.Ldi], "日")

print("儒略日:JD", sxtwl.J2000 + day.d0)
print("星期", numCn[day.week])

print(Gan[day.Lyear2.tg], Zhi[day.Lyear2.dz], "年", Gan[day.Lmonth2.tg], Zhi[day.Lmonth2.dz], "月",\
        Gan[day.Lday2.tg], Zhi[day.Lday2.dz], "日")

print("距冬至", day.cur_dz, "天")
print("距夏至", day.cur_xz, "天")
print("距立秋", day.cur_lq, "天")
print("距芒种", day.cur_mz, "天")
print("距小暑", day.cur_xs, "天")
```


### 获取月历信息
```
import  sxtwl
import sys
type = sys.getfilesystemencoding()

Gan = ["甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"]
Zhi = ["子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"]
ShX = ["鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗", "猪"]
numCn = ["零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"]
jqmc = ["冬至", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种", "夏至", "小暑", "大暑", "立秋", "处暑","白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪"]
ymc = ["十一", "十二", "正", "二", "三", "四", "五", "六", "七", "八", "九", "十" ]
rmc = ["初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十", "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十", "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十", "卅一"]

month = sxtwl.Lunar().yueLiCalc(2017, 12)

#打印做一个中间转换
def log(*arg):
    s = ""
    for v in arg:
        s += str(v)
    print(s.decode('UTF-8').encode(type))

log(month.y, "年", month.m, "月")
log(Gan[month.yearGan], Zhi[month.yearZhi], "年")
log("生肖:", ShX[month.ShX])

days = month.days
J2000 = 2451545
for day in days:
    log("===================================================")
    log("公历:", day.y, "年", day.m, "月", day.d, "日")
    if day.Lleap:
        log("润", ymc[day.Lmc], "月", rmc[day.Ldi], "日")
    else:
        log(ymc[day.Lmc], "月", rmc[day.Ldi], "日")

    log("儒略日:JD", sxtwl.J2000 + day.d0)
    log("星期", numCn[day.week])

    log(Gan[day.Lyear2.tg], Zhi[day.Lyear2.dz], "年", Gan[day.Lmonth2.tg], Zhi[day.Lmonth2.dz], "月",\
          Gan[day.Lday2.tg], Zhi[day.Lday2.dz], "日")

    log("距冬至", day.cur_dz, "天")
    log("距夏至", day.cur_xz, "天")
    log("距立秋", day.cur_lq, "天")
    log("距芒种", day.cur_mz, "天")
    log("距小暑", day.cur_xs, "天")
```


### 如果您是一个命理学爱好者，还提供二条接口

```
#获取八字时辰的天干地支
gz = lunar.getShiGz(2,  12)  #第一个参数为生日的日天干,参数二为出生的时间(小时)
print( Gan[gz.tg], Zhi[gz.dz])
    

#获取一年的信息
year = lunar.getYearCal(2018)
```

### 懒人安装包下载地址 （只提供Win版本）：

https://pan.baidu.com/s/1VR4MtPVV9iP9SSHNDjKZnQ

## 交流沟通群

如果在使用过程中遇到困难，可加微信群(如二维码过期，邮件:lifulinghan@aol.com索要新的):
![微信群](https://raw.githubusercontent.com/yuangu/sxtwl_cpp/master/doc/img/wxq.jpg)

###  最后

* 如果想加入此项目请联系 元谷(lifulinghan@aol.com)
* 如果使用此项目，请告之一下作者
* 如果您使用了此项目的代码，为了表示对寿星天文历原作者的尊重，请您项目适当的位置表达对许剑伟先生感谢
* 如果你觉得本库不错，欢迎打赏作者


微信用户打赏入口：

![微信支付](https://raw.githubusercontent.com/yuangu/sxtwl_cpp/master/doc/img/webchat.png)    


支付宝用户打赏入口：

![支付宝](https://raw.githubusercontent.com/yuangu/sxtwl_cpp/master/doc/img/ali.png)
import sxtwl

# 公历
day = sxtwl.fromSolar(2020, 12, 1)

s = "公历:%d年%d月%d日" % (day.getSolarYear(), day.getSolarMonth(), day.getSolarDay())
print(s)

s = "农历:%d年%s%d月%d日" % (day.getLunarYear(), '闰' if day.isLunarLeap() else '', day.getLunarMonth(), day.getLunarDay())
print(s)

# 农历
day = sxtwl.fromLunar(2020, 12, 1)

s = "公历:%d年%d月%d日" % (day.getSolarYear(), day.getSolarMonth(), day.getSolarDay())
print(s)

s = "农历:%d年%s%d月%d日" % (day.getLunarYear(), '闰' if day.isLunarLeap() else '', day.getLunarMonth(), day.getLunarDay())
print(s)

jqmc = ["冬至", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种", "夏至", "小暑", "大暑", "立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪"]
while True:
    day = day.after(1)
    if day.hasJieQi():
        print('节气：%s'% jqmc[day.getJieQi()])
        break
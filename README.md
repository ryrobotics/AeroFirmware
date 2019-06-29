# 概述

## 软件环境

- Toolchain/IDE : MDK-ARM V5

## 版本说明

### Ver 0.1.1

该版本删除了代码中无用的注释，同时对一部分注释进行了修改

- 关键更新：摇杆上锁时间缩短为0.5s

### Ver0.1

该版本修改了key.c文件，改善了oled显示界面，删除了GPS有关的信息。

- 第6行超声波改为US100，第二个数据改为竖直方向惯导融合高度估计信息
- 第7行变为SINS信息，第一个数据为竖直方向原始加速度信息，第二个为高度方向惯导估计的速度
- 第8行为陀螺仪信息，第一个为升温度标志位，第二个为温度采集数据

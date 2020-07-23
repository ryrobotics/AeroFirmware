# Introduction

This code is used for TI TM4C flight controller designed by our team. We use ToF sensor(TF-mini) for altitude mode and optical flow for position mode. OpenMV acts as vision sensor for searching fire, which is simulated by LED. The remote control was not used during the whole process of flight.

Here is the demo video: https://www.youtube.com/watch?v=CTNGVZQZacY

## 软件环境

- Toolchain/IDE : MDK-ARM V5

## 版本说明

### Ver LAST

代码的实际效果可以参照master分支下的mp4文件。

事实上，这套代码在后期由于各种原因，已经没有精力详细记录每个版本的更新信息，往往是刚写好代码，测试好，就push上来，之后再做别的功能。如果有任何问题请联系我，我会尽力解答。

### Ver 0.4

该版本实现了SDK+openmv自动任务完成，尝试了对长焦镜头的标定与优化，还需要进一步测试。

### Ver 0.3

该版本验证了一键上锁方式的稳定性，同时解决了起飞漂移的问题。问题主要来自一键起飞过程中，会自动清空融合数据，而不改变摇杆的情况下会影响光流的融合数据。同时改变了SDK运行速度，改善了openmv的安装偏移。

### Ver 0.2

该版本优化了上锁解锁方式，但是还存在一些细节问题，删去了原始的时间判断，后续版本需在此基础上进行深入开发
同时该版本删除了GPS对应的模式以及相关代码，6通道改变为上锁通道。

### Ver 0.1.1

该版本删除了代码中无用的注释，同时对一部分注释进行了修改

- 关键更新：摇杆上锁时间缩短为0.5s

### Ver0.1

该版本修改了key.c文件，改善了oled显示界面，删除了GPS有关的信息。

- 第6行超声波改为US100，第二个数据改为竖直方向惯导融合高度估计信息
- 第7行变为SINS信息，第一个数据为竖直方向原始加速度信息，第二个为高度方向惯导估计的速度
- 第8行为陀螺仪信息，第一个为升温度标志位，第二个为温度采集数据

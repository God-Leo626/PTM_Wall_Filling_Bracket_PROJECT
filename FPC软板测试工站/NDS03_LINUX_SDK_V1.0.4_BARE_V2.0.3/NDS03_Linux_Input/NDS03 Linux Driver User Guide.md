# NDS03 Linux Driver User Guide


## 介绍
------------------------

这里提供了NDS03在Linux平台的驱动和测试程序。程序基于树莓派4B+(kernel version: 6.6.47)运行，同时适用于其它的Linux平台。

应用层通过Input子系统获取NDS03测量数据。


## input子系统
------------------------

nds03的测量数据通过统一的数据结构提交给内核，数据结构包括类型，代码，数值。数据通过使用/dev/input目录下的文件接口传递到用户空间

nds03 input子系统的数据结构 :

```
类型 (type)：绝对位置事件 (EV_ABS)
代码 (code)：ABS_X, ABS_Y, ABS_Z：
数值（value）：深度值 (depth)，置信度 (config)，计数值 (count)

以下是使用evtest工具获取到的事件信息：
（注意：使用evtest工具前，先启动测距）

Input device name: "tof_nds03"
Supported events:
  Event type 0 (EV_SYN)
  Event type 3 (EV_ABS)
    Event code 0 (ABS_X)
      Value      0
      Min        0
      Max    65535
    Event code 1 (ABS_Y)
      Value      0
      Min        0
      Max    65535
    Event code 2 (ABS_Z)
      Value      0
      Min        0
      Max    65535
Properties:
Testing ... (interrupt to exit)
Event: time 1712130535.564701, type 3 (EV_ABS), code 0 (ABS_X), value 0
Event: time 1712130535.564701, type 3 (EV_ABS), code 1 (ABS_Y), value 0
Event: time 1712130535.564701, type 3 (EV_ABS), code 2 (ABS_Z), value 0
Event: time 1712130535.564701, -------------- SYN_REPORT ------------
Event: time 1712130536.068725, type 3 (EV_ABS), code 0 (ABS_X), value 90
Event: time 1712130536.068725, type 3 (EV_ABS), code 1 (ABS_Y), value 100
Event: time 1712130536.068725, type 3 (EV_ABS), code 2 (ABS_Z), value 47650
Event: time 1712130536.068725, -------------- SYN_REPORT ------------
Event: time 1712130536.580817, type 3 (EV_ABS), code 2 (ABS_Z), value 47588
Event: time 1712130536.580817, -------------- SYN_REPORT ------------
Event: time 1712130537.091979, type 3 (EV_ABS), code 2 (ABS_Z), value 47628
Event: time 1712130537.091979, -------------- SYN_REPORT ------------
Event: time 1712130537.604783, type 3 (EV_ABS), code 2 (ABS_Z), value 47530
Event: time 1712130537.604783, -------------- SYN_REPORT ------------
Event: time 1712130538.125198, type 3 (EV_ABS), code 2 (ABS_Z), value 47686
Event: time 1712130538.125198, -------------- SYN_REPORT ------------
Event: time 1712130538.628729, type 3 (EV_ABS), code 2 (ABS_Z), value 47689
Event: time 1712130538.628729, -------------- SYN_REPORT ------------
Event: time 1712130539.140776, type 3 (EV_ABS), code 0 (ABS_X), value 122
Event: time 1712130539.140776, type 3 (EV_ABS), code 2 (ABS_Z), value 38940
Event: time 1712130539.140776, -------------- SYN_REPORT ------------
Event: time 1712130539.652783, type 3 (EV_ABS), code 0 (ABS_X), value 427
Event: time 1712130539.652783, type 3 (EV_ABS), code 1 (ABS_Y), value 61



```

## SysFS属性列表
------------------------

SysFS属性列表提供了一种方便的方式，让用户和开发人员在程序运行时获取和配置与内核和设备相关的信息。

启动测距(需要足够的权限)
```shell
$ echo 1 > /sys/class/input/inputX/is_meas
``` 
查看测距状态
```shell
$ cat /sys/class/input/inputX/is_meas
``` 

文件属性路径：/sys/class/input/<inputX>

|   SysFS Attribute                                   |   Read / Write    |   Format  |
|-----------------------------------------------------|:-----------------:|:---------:|
|[is_meas](#is_meas)                                  |       R/W         |  string   |
|[meas_delay_ms](#meas_delay_ms)                      |       R/W         |  string   |
|[meas_mode](#poll_mode)                              |       R/W         |  string   |
|[enable_debug](#enable_debug)                        |       R/W         |  string   |
|[tof_reset](#sensor_reset)                           |       W           |  string   |
|[tof_calib](#offset_calib)                           |       R/W         |  string   |

-----------------------

### is_meas
|    | value  | Description  |
|----|--------|--------------|
| R/W|  0     | 停止测距 |
| R/W|  1     | 开始测距  |	

### meas_delay_ms
|    | value  | Description  |
|----|--------|--------------|
| R/W|   >=0  | 测距间隔(ms)  |

### meas_mode
|    | value   | Description  |
|----|---------|--------------|
| R/W|   0     | 中断模式      | 
| R/W| 	 1     | 轮训模式      |	

### enable_debug
|    | value  | Description  |
|----|--------|--------------|
| R/W|   0    |   关闭打印    | 
| R/W| 	 1    |   打开打印    |	

### tof_reset
|    | value | Description  |
|----|-------|--------------|
| W  | 1     | 传感器复位   | 

### tof_calib
|    | value  | Description  |
|----|--------|--------------|
| W  | 	>=0   | 标定距离(mm)  | 
| R  |  int   | 标定结果      |


##  目录结构

        .
        |-- kernel
        |   |-- Makefile                        ---编译驱动模块
        |   |-- NDS03_SDK                       ---nds03的sdk
        |   |-- arch
        |   |   `-- arm
        |   |       `-- boot
        |   |           `-- dts
        |   |               `-- nds03_overlay.dts   ---设备树
        |   |-- nds03.h
        |   |-- nds03_module.c                      ---nds03驱动具体实现
        |   |-- nds03_module_i2c.c                  ---I2C总线驱动
        |   `-- platform
        |       |-- nds03_platform.c                ---提供给sdk的接口函数
        |       `-- nds03_platform.h
        `-- user
        |-- Makefile                            ---编译应用程序
        |-- example             
        |   |-- NDS03_Calibration_Example.c    ---用于标定
        |   `-- NDS03_Ranging_Example.c        ---用于测距
        `-- platform
                |-- platform.c                ---平台相关
                `-- platform.h

## 树莓派4B平台实例
------------------------

### 安装树莓派内核源代码头文件

参考树莓派官方文档下载与您的内核版本匹配的头文件

```shell
$ sudo apt-get install raspberrypi-kernel-headers
```

### 添加设备树
```shell
$ sudo vim /boot/config.txt
dtoverlay=nds03_overlay
```

### 编译设备树
```shell
$ make dtb 
$ sudo mv nds03_overlay.dtbo /boot/overlays
$ sudo reboot
```

### 编译驱动程序
```shell
$ make modules
$ sudo insmod tof_nds03.ko
```

### 编译应用程序
```shell
$ cd ../user/
在Makefile中选择你要编译的example，默认测距的demo
$ make
```

### 运行example程序
注意: 应用程序需要足够的权限才能操作'/sys/class/input/inputX/'目录下的文件, 请使用sudo运行
```shell
$ sudo ./NDS03_Ranging_Example
```


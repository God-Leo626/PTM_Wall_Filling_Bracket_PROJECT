# NDS03 Linux Driver User Guide

## 介绍：
------------------------

这里提供了NDS03在Linux平台的驱动和测试程序。程序基于树莓派4B+(kernel version: 6.6.47)运行，同时适用于其它的Linux平台。

为用户提供了两种选择：

- 内核模式：编译并运行此驱动程序，使用一个负责处理I2C总线和中断的内核模块
  
- 用户模式：在完全用户模式下编译并运行这个驱动程序，其中I2C通信是使用/dev/i2c-1文件描述符处理的

内核模式支持中断读取ND03的数据，但需要编译并插入内核模块

用户模式适用于简单的应用程序，但是需要/dev/i2c-1可用

（注意：使用用户模式还是内核模式通过应用层程序中的 TOF_NDS03_KERNEL 宏定义来确定 ）

## 如何在树莓派上运行一个测试应用程序
    注意下面是在 raspberrypi 4上测试的 

### 安装内核头文件(仅在内核模式)
    参考raspberrypi 官方文档下载与内核版本匹配的头文件
    $ sudo apt-get install raspberrypi-kernel-headers

### 更新/boot/config.txt 文件 (仅在内核模式)
    $ sudo nano /boot/config.txt
    --> 在/boot/config.txt文件末尾添加或取消注释以下行
    dtparam=i2c_arm=on
    dtparam=i2c1=on
    dtparam=i2c1_baudrate=400000
    dtoverlay=nds03_overlay
    
### 编译设备树 (仅在内核模式)
```
$ cd NDS03_Linux_I2C/kernel
$ make dtb
$ sudo cp nds03_overlay.dtbo /boot/overlay
$ sudo reboot
```

### 编译内核模块 (仅在内核模式)
```
$ cd NDS03_Linux_I2C/kernel
$ make clean 
$ make
$ sudo insmod nds03_tof.ko
中断模式：
$ sudo insmod nds03_tof.ko intr_gpio_num=17 (使用gpio17作为中断引脚)
```

### 切换为root用户

```
对于树莓派，默认用户是pi
$ sudo passwd root 
-->对于没有root用户的先要开启，输入密码即可
$ su root
——>切换为root
（注意：使用的是非root用户时，操作/sys/class/gpio/目录下的文件，会出现打开文件失败）
```

### 编译测试例程

打开NDS03_Linux_I2C/user/Makefile，选择需要编译的应用程序
```
# cd NDS03_Linux_I2C/user
# make 
```

### 运行example程序

```
    # ./ND03NDS03_SingleRanging_Example
```
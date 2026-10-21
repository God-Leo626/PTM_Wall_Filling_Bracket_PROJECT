NDS03_SDK := NDS03_SDK

ccflags-y	+= -I$(src)/$(NDS03_SDK)/inc -I$(src)/platform/ -DNDS03_PLATFORM=PLATFORM_LINUX_DRIVER


obj-m	:= tof_nds03.o
tof_nds03-objs += nds03_module.o
tof_nds03-objs += nds03_module_i2c.o
tof_nds03-objs += platform/nds03_platform.o
tof_nds03-objs += $(NDS03_SDK)/src/nds03_calib.o
tof_nds03-objs += $(NDS03_SDK)/src/nds03_comm.o
tof_nds03-objs += $(NDS03_SDK)/src/nds03_data.o
tof_nds03-objs += $(NDS03_SDK)/src/nds03_dev.o


KDIR ?= /lib/modules/`uname -r`/build
ENV   := ARCH=arm64  CROSS_COMPILE=aarch64-linux-gnu-
PWD:= $(shell pwd)

all: modules dtb

modules:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
	@-rm -rf nds03_overlay.dtbo
dtb:
	dtc -I dts -O dtb -o nds03_overlay.dtbo arch/arm/boot/dts/nds03_overlay.dts

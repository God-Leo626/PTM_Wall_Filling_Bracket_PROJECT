obj-m += tof_nds03.o
tof_nds03-objs := nds03_module.o

KDIR ?= /lib/modules/`uname -r`/build
ENV   := ARCH=arm64  CROSS_COMPILE=aarch64-linux-gnu-
PWD:= $(shell pwd)

all:modules dtb

modules:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
	@-rm -rf nds03_overlay.dtbo

dtb:
	dtc -I dts -O dtb -o nds03_overlay.dtbo arch/arm/boot/dts/nds03_overlay.dts

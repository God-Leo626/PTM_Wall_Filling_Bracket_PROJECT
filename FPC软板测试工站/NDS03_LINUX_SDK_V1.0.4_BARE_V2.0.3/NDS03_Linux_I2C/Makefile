OUTPUT_DIR = $(notdir $(shell pwd))_output

all: driver app
	-mkdir $(OUTPUT_DIR)
	cp kernel/tof_nds03.ko kernel/nds03_overlay.dtbo $(OUTPUT_DIR)
	cp user/example_output/* $(OUTPUT_DIR)

clean: driver_clean app_clean
	rm -rf $(OUTPUT_DIR)

driver:
	cd kernel && make

driver_clean:
	cd kernel && make clean

app:
	cd user && bash ./build.sh

app_clean:
	cd user && bash ./build.sh clean


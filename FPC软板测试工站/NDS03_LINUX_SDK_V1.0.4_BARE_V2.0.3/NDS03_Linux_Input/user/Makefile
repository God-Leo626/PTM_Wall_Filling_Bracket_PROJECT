APP_SRC ?= NDS03_Ranging_Example.c
# APP_SRC ?= NDS03_Calibration_Example.c
# APP_SRC ?= NDS03_Setpulsenum_Example.c
# APP_SRC ?= NDS03_Getxtalk_Example.c
# APP_SRC ?= NDS03_Calibration_Example.c


TARGET_APP		:= $(patsubst %.c,%,$(APP_SRC))
CROSS_COMPILE	:= aarch64-linux-gnu-
CC	= $(CROSS_COMPILE)gcc

#inc
APP_INCLUDE_DIR += -Iplatform/

#src
SOURCE := example/$(APP_SRC)
SOURCE += platform/platform.c

CFLAGS := $(APP_INCLUDE_DIR)
CFLAGS += -lm -O3 -Wall -Werror -fno-strict-aliasing

OBJS = $(patsubst %.c,%.o,$(SOURCE))

all:   $(OBJS) 
	@$(CC) $(OBJS) -o $(TARGET_APP) $(CFLAGS)
	@echo -e "\033[35m    [CROSS_COMPILE]  $(CROSS_COMPILE) \033[0m"
	@echo -e "\033[35m    [Target]  $(APPDIR)/$(TARGET_APP) \033[0m"
	
%.o:%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo -e "\033[34m    [CC]  $< \033[0m"	

clean:
	@-rm -r $(OBJS)  $(TARGET_APP)
	@echo -e "\033[35m    [Delete]  $(OBJS) \033[0m"
	@echo -e "\033[35m    [Delete]  $(TARGET_APP) \033[0m"	

.PHONY: all clean 

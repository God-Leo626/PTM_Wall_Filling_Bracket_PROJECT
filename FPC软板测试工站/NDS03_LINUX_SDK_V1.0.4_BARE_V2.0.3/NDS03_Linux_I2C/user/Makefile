# APP_SRC ?= NDS03_Calibration_Example.c 
# APP_SRC ?= NDS03_ContiunousRangingInterrupt_Example.c  
# APP_SRC ?= NDS03_ContiunousRanging_Example.c  
APP_SRC ?= NDS03_SingleRangingInterrupt_Example.c  
# APP_SRC ?= NDS03_SingleRanging_Example.c

TARGET_APP		:= $(patsubst %.c,%,$(APP_SRC))

CROSS_COMPILE	:= aarch64-linux-gnu-
CC	= $(CROSS_COMPILE)gcc


SDK_PLATFORM := PLATFORM_NOT_C51
NDS03_SDK 	 := NDS03_SDK



OBJ_DIR := Object
CFLAGS		 := -DNDS03_PLATFORM=$(SDK_PLATFORM) -DTOF_NDS03_KERNEL

#inc
APP_INCLUDE_DIR := -I$(NDS03_SDK)/inc -I./library -I./platform
#src
SRC_DIRS	 := $(NDS03_SDK)/src ./library ./platform/

SRC := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c)) 

APP_OBJ := $(OBJ_DIR)/$(APP_SRC:.c=.o)

OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRC))) $(APP_OBJ)

CFLAGS += $(APP_INCLUDE_DIR)
CFLAGS += -lm -O3 -Wall -Werror -fno-strict-aliasing

vpath %.c $(SRC_DIRS)

all:   $(OBJS)
	@$(CC) $(OBJS) -o $(TARGET_APP) $(CFLAGS)
	@echo -e "\033[35m    [CROSS_COMPILE]  $(CROSS_COMPILE) \033[0m"
	@echo -e "\033[35m    [Target]  $(APPDIR)/$(TARGET_APP) \033[0m"

$(OBJ_DIR)/%.o : %.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo -e "\033[34m    [CC]  $< \033[0m"	

$(APP_OBJ) : example/$(APP_SRC)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo -e "\033[34m    [CC]  $< \033[0m"	

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)


clean:
	@-rm -rf $(OBJ_DIR)  $(TARGET_APP)
	@echo -e "\033[35m    [Delete]  $(OBJS) \033[0m"
	@echo -e "\033[35m    [Delete]  $(TARGET_APP) \033[0m"	

scp:
	scp $(TARGET_APP) pi@192.168.3.19:/home/pi


.PHONY: all clean 
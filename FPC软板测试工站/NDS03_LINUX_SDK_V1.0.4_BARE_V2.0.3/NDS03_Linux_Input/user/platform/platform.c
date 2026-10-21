/*!
 @file platform.c
 @brief 
 @author lull
 @date 2025-06
 @copyright Copyright (c) 2025  Shenzhen Nephotonics  Semiconductor Technology Co., Ltd.
 @license BSD 3-Clause License
          This file is part of the Nephotonics sensor SDK.
          It is licensed under the BSD 3-Clause License.
          A copy of the license can be found in the project root directory, in the file named LICENSE.
 */
#include "platform.h"
#include <sys/ioctl.h>
#include <unistd.h> 
#include <string.h>
#include <linux/input.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MAX_PATH_LEN 256
#define SYSFS_CONTENT_MAX_LEN   256

static char *strjoin_with_separator(char *a, char *b)
{
	char *res = malloc(strlen(a) + 1 + strlen(b) + 1);

	if (res) {
		strcpy(res, a);
		res[strlen(a)] = '/';
		res[strlen(a)+1] = '\0';
		strcat(res, b);
	}

	return res;
}


static char *test_event_by_name(char *name)
{
	FILE *deviceFile = NULL;
	char b[SYSFS_CONTENT_MAX_LEN] = "\n";
	
	char *full_name_path = strjoin_with_separator("/dev/input", name);
	if (!full_name_path)
		goto error;
	deviceFile = fopen(full_name_path, "rb");
	if(ioctl(fileno(deviceFile), EVIOCGNAME(sizeof(b)), b) < 0){
		goto error;
	}
    if (strcmp(b, TOF_NDS03_DRV_NAME) != 0) {
        goto error;
    }
    return full_name_path;
error:
	if (full_name_path)
		free(full_name_path);
	if (deviceFile == NULL)
		fclose(deviceFile);
	return NULL;
 
}

static char *find_event_node() 
{
    struct dirent *entry;
    char *event_path;
    DIR *dp = opendir("/dev/input");
    if (dp == NULL) {
        printf("opendir");
        return NULL;
    }
    while ((entry = readdir(dp))) {
        if (strncmp(entry->d_name, "event", 5) == 0) {
                event_path = test_event_by_name(entry->d_name);
                if(event_path != NULL)
                    break;
        }
    }
    closedir(dp);
    return event_path;
}

static int nds03_event_open(int flags)
{
    int fd = -1;
    char *event_path = find_event_node();
    if(event_path == NULL)
	{
		printf("nds03 input device not found\n");
        return -1 ;
	}
    fd = open(event_path, flags);
	if(fd == -1)
	{
		printf("fail to open %s , df:%d\n", event_path, fd);
	}
    free(event_path);
    return fd;
}


static char *test_sysfs_root(char *dirname)
{
	char *full_dir = strjoin_with_separator("/sys/class/input", dirname);
	char *full_name_path = NULL;
	int fd = -1;
	char b[SYSFS_CONTENT_MAX_LEN] = "\n";
	int rc;

	if (!full_dir)
		goto error;

	full_name_path = strjoin_with_separator(full_dir, "name");
	if (!full_name_path)
		goto error;

	fd = open(full_name_path, O_RDONLY);
	if (fd < 0)
		goto error;
	rc = read(fd, b, SYSFS_CONTENT_MAX_LEN);
	if (rc < 0)
		goto error;

	/* replace new line with end of string */
	b[rc-1] = '\0';
	if (strcmp(TOF_NDS03_DRV_NAME, b))
		goto error;

	free(full_name_path);
	close(fd);

	return full_dir;

error:
	if (full_dir)
		free(full_dir);
	if (full_name_path)
		free(full_name_path);
	if (fd >= 0)
		close(fd);

	return NULL;
}


static char *nds03_sysfs_find_device()
{
	static char *sysfs_root = NULL;

	if (!sysfs_root) {
		DIR *d;
		struct dirent *dir;

		d = opendir("/sys/class/input");
		if (d) {
			while ((dir = readdir(d)) != NULL) {
				sysfs_root = test_sysfs_root(dir->d_name);
				if (sysfs_root)
					break;
			}
			closedir(d);
		}
	}

	return sysfs_root;
}

static int nds03_sysfs_open(char *name, int flags)
{
	int fd = -1;
	char *path = nds03_sysfs_find_device();
	char *full_path;
	if (!path)
		return -1;
	full_path = strjoin_with_separator(path, name);
	if (!full_path)
		return -1;
	fd = open(full_path,flags);
	free(full_path);

	return fd;
}



int8_t nds03_platform_init(NDS03_Platform_t *pdev)
{
	/* !> open /dev/input/eventX */
    pdev->eventFd = nds03_event_open(O_RDONLY);
    if(pdev->eventFd == -1){
        return -1;
    }
    return 0;
}


int8_t nds03_platform_uninit(NDS03_Platform_t *pdev)
{
	/* !> open /dev/input/eventX */
    pdev->eventFd = nds03_event_open(O_RDONLY);
    if(pdev->eventFd == -1){
        return -2;
    }

    return 0;
}

static int8_t nds03_meas_control(NDS03_Platform_t *pdev, uint8_t state)
{
	int ret, count;
	int fd = nds03_sysfs_open("is_meas", O_WRONLY);
	char b[SYSFS_CONTENT_MAX_LEN];
	count = sprintf(b, "%d", state);
	ret = write(fd, b, count);
	if(ret < 0)
	{
		printf("%s: sysfs write failed\n", __func__);
		return -1;
	}
	close(fd);
	return 0;
}

int8_t nds03_meas_start(NDS03_Platform_t *pdev)
{
	return nds03_meas_control(pdev, 1);
}

int8_t nds03_meas_stop(NDS03_Platform_t *pdev)
{
	return nds03_meas_control(pdev, 0);
}

int8_t nds03_tof_reset(NDS03_Platform_t *pdev)
{
	int ret;
	int fd = nds03_sysfs_open("tof_reset", O_WRONLY);
	ret = write(fd, "1", 1);
	if(ret < 0)
	{
		printf("%s: sysfs write failed\n", __func__);
		close(fd);
		return -1;
	}
	close(fd);	
	return 0;
}

int8_t nds03_tof_calib(NDS03_Platform_t *pdev, uint16_t dist_mm)
{
	int ret, count;
	int16_t calibResult;
	int fd = nds03_sysfs_open("tof_calib", O_RDWR);
	char b[SYSFS_CONTENT_MAX_LEN];
	count = sprintf(b, "%d", dist_mm);
	ret = write(fd, b, count);
	if(ret < 0)
	{	
		goto calib_err;
	}
	return 0;
calib_err:
	ret = read(fd, b, sizeof(b));
	if(ret < 0)
	{
		printf("%s: sysfs read error\n", __func__);
		return -1;
	}
	calibResult = atol(b);
	printf("calibResult: %d\n", calibResult);
	close(fd);	
	return -1;
}

int8_t nds03_set_polling_interval(NDS03_Platform_t *pdev, uint32_t ms)
{
	int ret, count;
	char b[SYSFS_CONTENT_MAX_LEN];
	int fd = nds03_sysfs_open("meas_delay_ms", O_WRONLY);
	count = sprintf(b, "%d", ms);
	ret = write(fd, b, count);
	if(ret < 0)
	{	
		printf("%s: sysfs write failed\n", __func__);
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}


int8_t nds03_get_tof_xtalk(NDS03_Platform_t *pdev, uint16_t* xtalk_value)
{
	int ret;
	char b[SYSFS_CONTENT_MAX_LEN];	
	int fd = nds03_sysfs_open("tof_xtalk", O_RDWR);
	ret = read(fd, b, sizeof(b));
	if(ret < 0)
	{
		printf("%s: sysfs read error\n", __func__);
		close(fd);
		return -1;
	}	
	*xtalk_value = atol(b);
	close(fd);
	return 0;
}

int8_t nds03_set_tof_pulsenum(NDS03_Platform_t *pdev, uint32_t pulse_num)
{
	int ret, count;
	int fd = nds03_sysfs_open("tof_pulsenum", O_WRONLY);
	char b[SYSFS_CONTENT_MAX_LEN];
	count = sprintf(b, "%u", pulse_num);
	ret = write(fd, b, count);
	if(ret < 0)
	{
		printf("%s: sysfs write failed\n", __func__);
		return -1;
	}
	close(fd);
	return 0;
}




int8_t nds03_get_single_rang(NDS03_Platform_t *pdev)
{ 
    uint16_t isSynced = 0;
    struct input_event ev;
	int *pdata = (int *)&pdev->ranging_data[0];
	
	do{
		ssize_t bytes = read(pdev->eventFd, &ev, sizeof(struct input_event));    
		if (bytes == sizeof(struct input_event)) {
			if (ev.type == EV_ABS) {
				*(pdata + ev.code) = ev.value;
			} else if (ev.type == EV_SYN) {
				isSynced = 1;
			}
		}
	}while(!isSynced);

	return 0;
}
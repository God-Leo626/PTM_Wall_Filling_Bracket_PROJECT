#ifndef NDS03_APP_H
#define NDS03_APP_H
#include <stdint.h>

#define TARGET_DISTANCE_MM     117U  //检测的距离是120mm，实际距离117mm
#define DISTANCE_TOLERANCE_PCT 6U    //误差在±6%，最大误差不超过10%

/* Run after the fixture key handler. enabled=0 releases the sensor bus. */
void nds03_app_process(uint8_t enabled);

#endif

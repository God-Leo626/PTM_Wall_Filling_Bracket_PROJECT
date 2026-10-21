#ifndef NDS03_APP_H
#define NDS03_APP_H
#include <stdint.h>

/* Run after the fixture key handler. enabled=0 releases the sensor bus. */
void nds03_app_process(uint8_t enabled);

#endif

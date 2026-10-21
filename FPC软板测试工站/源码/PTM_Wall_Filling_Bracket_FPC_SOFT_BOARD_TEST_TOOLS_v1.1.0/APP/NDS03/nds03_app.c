#include <stdio.h>
#include <string.h>
#include "nds03_app.h"
#include "nds03_board.h"
#include "nds03_dev.h"
#include "nds03_data.h"
#include "nds03_comm.h"
#include "timer.h"

typedef enum { SENSOR_OFF, SENSOR_INIT, SENSOR_IDLE, SENSOR_WAIT } SensorState;
static NDS03_Dev_t sensor;
static SensorState state = SENSOR_OFF;
static uint32_t state_time, sample_time, poll_time;

static void fail(const char *stage, NDS03_Error error)
{
    printf("[NDS03] %s failed, error=%d; retry in %u ms\r\n",
           stage, (int)error, (unsigned)NDS03_RETRY_PERIOD_MS);
    nds03_platform_uninit(&sensor.platform, NULL);
    state = SENSOR_INIT;
    state_time = vTimeGetSysTime();
}

static NDS03_Error sensor_init(void)
{
    NDS03_Error ret;
    memset(&sensor, 0, sizeof(sensor));
    sensor.platform.i2c_dev_addr = NDS03_DEFAULT_SLAVE_ADDR;
    CHECK_RET(nds03_platform_init(&sensor.platform, NULL));
    CHECK_RET(NDS03_WaitDeviceBootUp(&sensor));
    CHECK_RET(NDS03_IsNDS03(&sensor));
    CHECK_RET(NDS03_InitDevice(&sensor));
    CHECK_RET(NDS03_SetTargetNum(&sensor, 1));
    CHECK_RET(NDS03_SetFrameTime(&sensor, 100000U));
    CHECK_RET(NDS03_SetGpio1Config(&sensor, NDS03_GPIO1_FUNCTIONALITY_OFF,
                                  NDS03_GPIO1_POLARITY_LOW));
    return NDS03_ERROR_NONE;
}

void nds03_app_process(uint8_t enabled)
{
    NDS03_Error ret;
    uint8_t ready;
    uint32_t now = vTimeGetSysTime();
    const NDS03_RangingData_t *data = &sensor.ranging_data[0];

    if (!enabled) {
        if (state != SENSOR_OFF) {
            nds03_platform_uninit(&sensor.platform, NULL);
            state = SENSOR_OFF;
            printf("[NDS03] stopped\r\n");
        }
        return;
    }

    if (state == SENSOR_OFF) {
        state = SENSOR_INIT;
        state_time = now - NDS03_RETRY_PERIOD_MS;
    }
    if (state == SENSOR_INIT) {
        if ((uint32_t)(now - state_time) < NDS03_RETRY_PERIOD_MS) return;
        ret = sensor_init();
        if (ret != NDS03_ERROR_NONE) { fail("init", ret); return; }
        printf("[NDS03] ready, addr=0x%02X, firmware=0x%08lX\r\n",
               (unsigned)sensor.platform.i2c_dev_addr,
               (unsigned long)sensor.chip_info.fw_version);
        state = SENSOR_IDLE;
        now = vTimeGetSysTime();
        sample_time = now - NDS03_SAMPLE_PERIOD_MS;
    }
    if (state == SENSOR_IDLE) {
        if ((uint32_t)(now - sample_time) < NDS03_SAMPLE_PERIOD_MS) return;
        ret = NDS03_StartSingleMeasurement(&sensor);
        if (ret != NDS03_ERROR_NONE) { fail("start", ret); return; }
        state = SENSOR_WAIT;
        sample_time = vTimeGetSysTime();
        poll_time = sample_time;
        return;
    }
    if ((uint32_t)(now - sample_time) >= NDS03_MEASUREMENT_TIMEOUT_MS) {
        fail("measurement timeout", NDS03_ERROR_TIMEOUT);
        return;
    }
    if ((uint32_t)(now - poll_time) < NDS03_POLL_PERIOD_MS) return;
    poll_time = now;
    ret = NDS03_ReadByte(&sensor, NDS03_REG_DAT_VAL, &ready);
    if (ret != NDS03_ERROR_NONE) { fail("poll", ret); return; }
    if (ready != NDS03_DEPTH_DATA_FLAG) return;
    ret = NDS03_ReadRangingData(&sensor);
    if (ret == NDS03_ERROR_NONE) ret = NDS03_ClearDataValidFlag(&sensor);
    if (ret != NDS03_ERROR_NONE) { fail("read", ret); return; }
    if (data->depth == NDS03_DEPTH_INVALID_VALUE || data->confi <= NDS03_CONFIDENCE_MIN) {
        printf("[NDS03] no valid target, raw_depth=%u, confidence=%u\r\n",
               (unsigned)data->depth, (unsigned)data->confi);
    } else {
        printf("[NDS03] distance=%u mm, confidence=%u, count=%u\r\n",
               (unsigned)data->depth, (unsigned)data->confi, (unsigned)data->count);
    }
    state = SENSOR_IDLE;
}

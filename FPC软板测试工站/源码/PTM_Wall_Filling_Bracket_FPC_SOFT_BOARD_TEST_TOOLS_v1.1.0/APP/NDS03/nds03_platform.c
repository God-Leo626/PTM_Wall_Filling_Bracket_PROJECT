/* STM32F407 standard peripheral library port for NDS03 SDK 2.0.3.
 * Call from the main loop only; PB6/PB7 must not be used by other drivers.
 */
#include "nds03_def.h"
#include "nds03_board.h"
#include "time.h"
#include "timer.h"

static uint32_t half_period_us = 5U;

static void sda(uint8_t high)
{
    GPIO_WriteBit(NDS03_SDA_PORT, NDS03_SDA_PIN, high ? Bit_SET : Bit_RESET);
}

static void scl_low(void)
{
    GPIO_ResetBits(NDS03_SCL_PORT, NDS03_SCL_PIN);
}

static void bus_delay(void)
{
    vTimeDelayUs(half_period_us);
}

/* Open-drain SCL, including a bounded clock-stretch wait. */
static int8_t scl_high(void)
{
    uint16_t retry = 1000;
    GPIO_SetBits(NDS03_SCL_PORT, NDS03_SCL_PIN);
    while (!GPIO_ReadInputDataBit(NDS03_SCL_PORT, NDS03_SCL_PIN)) {
        if (--retry == 0) return NDS03_ERROR_I2C;
        vTimeDelayUs(1);
    }
    return NDS03_ERROR_NONE;
}

static int8_t bus_stop(void)
{
    int8_t ret;
    scl_low();
    sda(0);
    bus_delay();
    ret = scl_high();
    bus_delay();
    sda(1);
    bus_delay();
    return ret;
}

static int8_t bus_start(void)
{
    sda(1);
    bus_delay();
    if (scl_high() != 0) return NDS03_ERROR_I2C;
    bus_delay();
    if (!GPIO_ReadInputDataBit(NDS03_SDA_PORT, NDS03_SDA_PIN))
        return NDS03_ERROR_I2C;
    sda(0);
    bus_delay();
    scl_low();
    return NDS03_ERROR_NONE;
}

static int8_t write_byte(uint8_t data)
{
    uint8_t i, nack;
    for (i = 0; i < 8; ++i) {
        sda((data & 0x80U) != 0);
        bus_delay();
        if (scl_high() != 0) return NDS03_ERROR_I2C;
        bus_delay();
        scl_low();
        data <<= 1;
    }
    sda(1);
    bus_delay();
    if (scl_high() != 0) return NDS03_ERROR_I2C;
    bus_delay();
    nack = GPIO_ReadInputDataBit(NDS03_SDA_PORT, NDS03_SDA_PIN);
    scl_low();
    return nack ? NDS03_ERROR_I2C : NDS03_ERROR_NONE;
}

static int8_t read_byte(uint8_t *data, uint8_t ack)
{
    uint8_t i, value = 0;
    sda(1);
    for (i = 0; i < 8; ++i) {
        bus_delay();
        if (scl_high() != 0) return NDS03_ERROR_I2C;
        bus_delay();
        value = (uint8_t)((value << 1) |
            GPIO_ReadInputDataBit(NDS03_SDA_PORT, NDS03_SDA_PIN));
        scl_low();
    }
    sda(!ack);
    bus_delay();
    if (scl_high() != 0) return NDS03_ERROR_I2C;
    bus_delay();
    scl_low();
    sda(1);
    *data = value;
    return NDS03_ERROR_NONE;
}

int8_t nds03_platform_init(NDS03_Platform_t *pdev, void *arg)
{
    GPIO_InitTypeDef io;
    uint8_t i;
    (void)arg;
    if (pdev == NULL) return NDS03_ERROR_API;
    RCC_AHB1PeriphClockCmd(NDS03_SCL_RCC | NDS03_SDA_RCC |
                           NDS03_XSHUT_RCC | NDS03_INT_RCC, ENABLE);
    GPIO_StructInit(&io);
    io.GPIO_Mode = GPIO_Mode_OUT;
    io.GPIO_OType = GPIO_OType_OD;
    io.GPIO_Speed = GPIO_Speed_50MHz;
    io.GPIO_PuPd = GPIO_PuPd_UP;
    sda(1);
    GPIO_SetBits(NDS03_SCL_PORT, NDS03_SCL_PIN);
    io.GPIO_Pin = NDS03_SDA_PIN;
    GPIO_Init(NDS03_SDA_PORT, &io);
    io.GPIO_Pin = NDS03_SCL_PIN;
    GPIO_Init(NDS03_SCL_PORT, &io);
    io.GPIO_Pin = NDS03_XSHUT_PIN;
    io.GPIO_OType = GPIO_OType_PP;
    io.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_ResetBits(NDS03_XSHUT_PORT, NDS03_XSHUT_PIN);
    GPIO_Init(NDS03_XSHUT_PORT, &io);
    io.GPIO_Pin = NDS03_INT_PIN;
    io.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(NDS03_INT_PORT, &io);
    pdev->xshut_pin = 1; /* This board has a connected XSHUT. */
    /* Release an incomplete transfer left by a host reset. */
    for (i = 0; i < 9; ++i) {
        scl_low();
        bus_delay();
        if (scl_high() != 0) return NDS03_ERROR_I2C;
        bus_delay();
    }
    if (bus_stop() != 0) return NDS03_ERROR_I2C;
    return GPIO_ReadInputDataBit(NDS03_SDA_PORT, NDS03_SDA_PIN) ?
        NDS03_ERROR_NONE : NDS03_ERROR_I2C;
}

int8_t nds03_platform_uninit(NDS03_Platform_t *pdev, void *arg)
{
    GPIO_InitTypeDef io;
    (void)arg;
    if (pdev == NULL) return NDS03_ERROR_API;
    GPIO_ResetBits(NDS03_XSHUT_PORT, NDS03_XSHUT_PIN);
    GPIO_StructInit(&io);
    io.GPIO_Mode = GPIO_Mode_IN;
    io.GPIO_PuPd = GPIO_PuPd_NOPULL;
    io.GPIO_Pin = NDS03_SDA_PIN;
    GPIO_Init(NDS03_SDA_PORT, &io);
    io.GPIO_Pin = NDS03_SCL_PIN;
    GPIO_Init(NDS03_SCL_PORT, &io);
    return NDS03_ERROR_NONE;
}

int8_t nds03_i2c_write_nbytes(NDS03_Platform_t *pdev, uint8_t reg,
                             uint8_t *data, uint16_t len)
{
    uint16_t i;
    int8_t ret;
    if (pdev == NULL || data == NULL || len == 0 || pdev->i2c_dev_addr > 0x7F)
        return NDS03_ERROR_API;
    ret = bus_start();
    if (!ret) ret = write_byte((uint8_t)(pdev->i2c_dev_addr << 1));
    if (!ret) ret = write_byte(reg);
    for (i = 0; !ret && i < len; ++i) ret = write_byte(data[i]);
    if (bus_stop() != 0) ret = NDS03_ERROR_I2C;
    return ret;
}

int8_t nds03_i2c_read_nbytes(NDS03_Platform_t *pdev, uint8_t reg,
                            uint8_t *data, uint16_t len)
{
    uint16_t i;
    int8_t ret;
    if (pdev == NULL || data == NULL || len == 0 || pdev->i2c_dev_addr > 0x7F)
        return NDS03_ERROR_API;
    ret = bus_start();
    if (!ret) ret = write_byte((uint8_t)(pdev->i2c_dev_addr << 1));
    if (!ret) ret = write_byte(reg);
    if (!ret) ret = bus_start(); /* Repeated START: no STOP here. */
    if (!ret) ret = write_byte((uint8_t)((pdev->i2c_dev_addr << 1) | 1U));
    for (i = 0; !ret && i < len; ++i) ret = read_byte(&data[i], i + 1U < len);
    if (bus_stop() != 0) ret = NDS03_ERROR_I2C;
    return ret;
}

int8_t nds03_i2c_get_clock_frequency(NDS03_Platform_t *pdev, uint32_t *frequency)
{
    if (pdev == NULL || frequency == NULL) return NDS03_ERROR_API;
    *frequency = 500000U / half_period_us; /* Nominal; GPIO/IRQ overhead lowers it. */
    return NDS03_ERROR_NONE;
}

int8_t nds03_i2c_set_clock_frequency(NDS03_Platform_t *pdev, uint32_t frequency)
{
    if (pdev == NULL || frequency < 10000U || frequency > 100000U)
        return NDS03_ERROR_API;
    half_period_us = (500000U + frequency - 1U) / frequency;
    return NDS03_ERROR_NONE;
}

int8_t nds03_delay_1ms(NDS03_Platform_t *pdev, uint32_t ms)
{
    (void)pdev;
    vTimeDelayMs(ms);
    return NDS03_ERROR_NONE;
}

int8_t nds03_delay_10us(NDS03_Platform_t *pdev, uint32_t count)
{
    (void)pdev;
    while (count--) vTimeDelayUs(10);
    return NDS03_ERROR_NONE;
}

int8_t nds03_get_system_clk_ms(NDS03_Platform_t *pdev, int32_t *ms)
{
    if (pdev == NULL || ms == NULL) return NDS03_ERROR_API;
    *ms = (int32_t)vTimeGetSysTime();
    return NDS03_ERROR_NONE;
}

int8_t nds03_set_xshut_pin_level(NDS03_Platform_t *pdev, int8_t level)
{
    if (pdev == NULL || !pdev->xshut_pin) return NDS03_ERROR_API;
    GPIO_WriteBit(NDS03_XSHUT_PORT, NDS03_XSHUT_PIN, level ? Bit_SET : Bit_RESET);
    return NDS03_ERROR_NONE;
}

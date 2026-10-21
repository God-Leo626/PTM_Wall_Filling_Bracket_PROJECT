#include "user.h"

void sht4xx_init(void)
{
    bsp_iic_init();
    vTimeDelayMs(10);
}

uint8_t sht4xx_soft_reset(void)
{
    iic_start();
    iic_send_byte((SHT4_SLAVE_ADDRESS << 1) | 0x00);
    if (iic_wait_ack())
    {
        TRACE_WP("[sht4xx_soft_reset] nack for w_slave_addr\r\n");
        iic_stop();
        return 1;
    }
    iic_send_byte(SHT4_SOFT_RESET);
    if (iic_wait_ack())
    {
        TRACE_WP("[sht4xx_soft_reset] nack for reset_addr\r\n");
        iic_stop();
        return 1;
    }
    iic_stop();
    vTimeDelayMs(1);
    return 0;

}

uint8_t sht4xx_write_bytes(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data_buf, uint8_t len)
{
    uint8_t i;
    uint8_t ret = 0;
    iic_start();
    iic_send_byte((slave_addr << 1) | 0x00);
    if (iic_wait_ack())
    {
        TRACE_WP("[sht4xx_write_bytes] nack for w_slave_addr\r\n");
        goto cmd_fail;
    }

    iic_send_byte(reg_addr);
    if (iic_wait_ack())
    {
        TRACE_WP("[sht4xx_write_bytes]nack for reg_addr\r\n");
        goto cmd_fail;
    }

    for (i = 0; i < len; i++)
    {
        iic_send_byte(data_buf[i]);
        if (iic_wait_ack())
        {
            TRACE_WP("[sht4xx_write_bytes]nack for w_data[%d]\r\n", i);
            goto cmd_fail;
        }
        vTimeDelayUs(40);
    }
    iic_stop();
    return 0;

cmd_fail:
    iic_stop();
    return 1;
}

uint8_t sht4xx_read_bytes(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data_buf, uint8_t len)
{
    uint8_t i;
    uint8_t ret = 0;

    iic_start();
    iic_send_byte((slave_addr << 1) | 0x00);
    if (iic_wait_ack())
    {
        TRACE_WP("[sht4xx_read_bytes] nack for w_slave_addr[0x%02x]\r\n", slave_addr);
        goto cmd_fail;
    }

    iic_send_byte(reg_addr);
    if (iic_wait_ack())
    {
        TRACE_WP("[sht4xx_read_bytes]nack for reg_addr[0x%02x]\r\n", reg_addr);
        goto cmd_fail;
    }
    iic_stop();

    vTimeDelayMs(10);

    iic_start();
    iic_send_byte((slave_addr << 1) | 0x01);
    if (iic_wait_ack())
    {
        TRACE_WP("[sht4xx_read_bytes] nack for r_slave_addr[0x%02x]\r\n", slave_addr);
        goto cmd_fail;
    }

    for (i = 0; i < (len - 1); i++)
    {
        data_buf[i] = iic_read_byte(1);
        vTimeDelayUs(40);
    }
    data_buf[i] = iic_read_byte(0);
    iic_stop();
    return 0;


cmd_fail:
    iic_stop();
    return 1;
}

uint8_t crc_check(const uint8_t *data, uint16_t count)
{
    uint16_t current_byte;
    uint8_t crc = 0xFF;
    uint8_t crc_bit;

    /* calculates 8-Bit checksum with given polynomial */
    for (current_byte = 0; current_byte < count; ++current_byte)
    {
        crc ^= (data[current_byte]);
        for (crc_bit = 8; crc_bit > 0; --crc_bit)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc = (crc << 1);
        }
    }
    return crc;
}

uint8_t sht4x_read_temp_and_hum(float *temperature, float *humidity)
{
    uint8_t ret, crc;
    uint8_t buf[6] = {0};
    ret = sht4xx_read_bytes(SHT4_SLAVE_ADDRESS, SHT4_MEASURE_HIGH_PRECISION, buf, 6);
    TRACE_WP("<< sht4x_read_temp_and_hum >> sht4_read_data:\r\n");
    for (uint8_t i = 0; i < 6; i++)
    {
        TRACE_WP("<< sht4x_read_temp_and_hum >> data[%d]: %02x\r\n", i, buf[i]);
    }
    if (ret)
    {
        TRACE_WP("<< sht4x_read_temp_and_hum >>\r\n");
        return ret;
    }
    else
    {
        crc = crc_check(&buf[0], 2);
        if (crc != buf[2])
        {
            TRACE_WP("<< sht4x_read_temp_and_hum >>, temperature crc erro[%02x]\r\n", crc);
            return 1;
        }

        crc = crc_check(&buf[3], 2);
        if (crc != buf[5])
        {
            TRACE_WP("<< sht4x_read_temp_and_hum >>, humidity crc erro[%02x]\r\n", crc);
            return 1;
        }

        *temperature = -45 + 175 * (((buf[0] << 8) | buf[1]) / 65535.0);
        *humidity = -6 + 125 * (((buf[3] << 8) | buf[4]) / 65535.0);
        TRACE_WP("<< sht4x_read_temp_and_hum >> temperature %.2f C Humidity: %.2f %%\r\n", *temperature, *humidity);
        return 0;
    }
}


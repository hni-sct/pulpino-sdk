#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "gpio_spi.h"
#include "gpio.h"
#include "utils.h"


static uint32_t spi_cs;

void spi_begin(uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t cs, uint32_t max_slave_freq) {
    set_pin_function(cs, FUNC_GPIO);
    set_gpio_pin_direction(cs, DIR_OUT);
    set_gpio_pin_value(cs, 1);

    set_pin_function(clk, FUNC_GPIO);
    set_gpio_pin_direction(clk, DIR_OUT);
    set_gpio_pin_value(clk, 1);

    set_pin_function(mosi, FUNC_GPIO);
    set_gpio_pin_direction(mosi, DIR_OUT);
    set_gpio_pin_value(mosi, 0);

    set_pin_function(miso, FUNC_GPIO);
    set_gpio_pin_direction(miso, DIR_IN);
}

uint8_t spi_transfer_byte(uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t byte)
{
    uint8_t res = 0;

    for (int i=7; i >= 0; i--) {
        set_gpio_pin_value(mosi, (byte >> i) & 1);
        set_gpio_pin_value(clk, 0);
        res |= get_gpio_pin_value(miso) << i;
        set_gpio_pin_value(clk, 1);
        sleep_busy(1000);
    }
    return res;
}

int8_t spi_write(uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t cs, uint8_t reg_addr, uint8_t *data,
                 uint16_t len)
{
    set_gpio_pin_value(cs, 0);

    spi_transfer_byte(clk, miso, mosi, reg_addr);
    for (int i = 0; i < len; ++i) {
        spi_transfer_byte(clk, miso, mosi, data[i]);
    }

    set_gpio_pin_value(cs, 1);
    return 0;
}

int8_t spi_read(uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t cs, uint8_t reg_addr, uint8_t *data,
                uint16_t len)
{
    uint8_t result;
    set_gpio_pin_value(cs, 0);
    spi_transfer_byte(clk, miso, mosi, reg_addr);

    for (int i = 0; i < len-1; ++i) {
        data[i] = spi_transfer_byte(clk, miso, mosi, reg_addr);
    }
    data[len-1] = spi_transfer_byte(clk, miso, mosi, reg_addr);
    set_gpio_pin_value(cs, 1);
    return 0;
}

#include <stdint.h>
#include <stdio.h>
#include "gpio_spi.h"
#include "gpio.h"
#include "utils.h"

#define SCK_PIN 13
#define MISO_PIN 12
#define MOSI_PIN 11

static uint32_t spi_cs;

void spi_begin(uint32_t cs, uint32_t max_slave_freq) {
    spi_cs = cs;

    set_pin_function(cs, FUNC_GPIO);
    set_gpio_pin_direction(cs, DIR_OUT);
    set_gpio_pin_value(cs, 1);

    set_pin_function(SCK_PIN, FUNC_GPIO);
    set_gpio_pin_direction(SCK_PIN, DIR_OUT);
    set_gpio_pin_value(SCK_PIN, 1);

    set_pin_function(MOSI_PIN, FUNC_GPIO);
    set_gpio_pin_direction(MOSI_PIN, DIR_OUT);
    set_gpio_pin_value(MOSI_PIN, 0);

    set_pin_function(MISO_PIN, FUNC_GPIO);
    set_gpio_pin_direction(MISO_PIN, DIR_IN);
}

uint8_t spi_transfer_byte(uint8_t byte)
{
    uint8_t res = 0;

    for (int i=7; i >= 0; i--) {
        set_gpio_pin_value(MOSI_PIN, (byte >> i) & 1);
        set_gpio_pin_value(SCK_PIN, 0);
        res |= get_gpio_pin_value(MISO_PIN) << i;
        set_gpio_pin_value(SCK_PIN, 1);
        sleep_busy(100);
    }
    return res;
}

int8_t spi_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data,
                 uint16_t len)
{
    set_gpio_pin_value(spi_cs, 0);

    spi_transfer_byte(reg_addr);
    for (int i = 0; i < len; ++i) {
        spi_transfer_byte(data[i]);
    }

    set_gpio_pin_value(spi_cs, 1);
    return 0;
}

int8_t spi_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data,
                uint16_t len)
{
    uint8_t result;
    printf("spi_read %x", reg_addr);
    set_gpio_pin_value(spi_cs, 0);
    spi_transfer_byte(reg_addr);

    for (int i = 0; i < len-1; ++i) {
        data[i] = spi_transfer_byte(reg_addr);
    }
    data[len-1] = spi_transfer_byte(reg_addr);
    set_gpio_pin_value(spi_cs, 1);
    return 0;
}

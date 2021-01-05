#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "bmi160.h"
#include "gpio_spi.h"

#define CS 0

void delay_ms(uint32_t period) {
    sleep_busy(period*10);
}

int main()
{
    uint8_t data[2] = {0x13, 0x37};
    struct bmi160_dev sensor;

    spi_begin(CS, 10000);
    sensor.read = &spi_read;
    sensor.write = &spi_write;
    sensor.delay_ms = &delay_ms;
    sensor.interface = BMI160_SPI_INTF;
    uint8_t res = bmi160_init(&sensor);
    printf("Res=%d\n", res);
}

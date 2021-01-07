#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "bmi160.h"
#include "gpio_spi.h"

#define BMI_CS 0
#define BMI_SCK 13
#define BMI_MISO 12
#define BMI_MOSI 11

void delay_ms(uint32_t period)
{
    sleep_busy(period*10);
}

int8_t bmi_spi_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    return spi_write(BMI_SCK, BMI_MISO, BMI_MOSI, BMI_CS, reg_addr, data, len);
}


int8_t bmi_spi_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    return spi_read(BMI_SCK, BMI_MISO, BMI_MOSI, BMI_CS, reg_addr, data, len);
}

int main()
{
    struct bmi160_dev sensor;

    spi_begin(BMI_SCK, BMI_MISO, BMI_MOSI, BMI_CS, 10000);
    sensor.read = &bmi_spi_read;
    sensor.write = &bmi_spi_write;
    sensor.delay_ms = &delay_ms;
    sensor.interface = BMI160_SPI_INTF;
    uint8_t res = bmi160_init(&sensor);
    printf("Res=%d\n", res);
}

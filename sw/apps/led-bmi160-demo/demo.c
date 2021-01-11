#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "bmi160.h"
#include "gpio_spi.h"
#include "led-matrix.h"

#define BMI_SCK 13
#define BMI_MISO 12
#define BMI_MOSI 11
#define BMI_CS 10

void delay_ms(uint32_t period)
{
    sleep_busy(period*100);
}

int8_t bmi_spi_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    return spi_write(BMI_SCK, BMI_MISO, BMI_MOSI, BMI_CS, reg_addr, data, len);
}


int8_t bmi_spi_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    return spi_read(BMI_SCK, BMI_MISO, BMI_MOSI, BMI_CS, reg_addr, data, len);
}

int8_t config_sensors(struct bmi160_dev *sensor)
{
    int8_t rslt = BMI160_OK;

    /* Select the Output data rate, range of accelerometer sensor */
    sensor->accel_cfg.odr = BMI160_ACCEL_ODR_1600HZ;
    sensor->accel_cfg.range = BMI160_ACCEL_RANGE_2G;
    sensor->accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;

    /* Sel->ct the power mode of accelerometer sensor */
    sensor->accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

    /* Sel->ct the Output data rate, range of Gyroscope sensor */
    sensor->gyro_cfg.odr = BMI160_GYRO_ODR_3200HZ;
    sensor->gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
    sensor->gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;

    /* Select the power mode of Gyroscope sensor */
    sensor->gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;

    /* Set the sensor configuration */
    rslt = bmi160_set_sens_conf(sensor);
    return rslt;
}

void get_sensor_data(struct bmi160_dev *sensor, int16_t *x, int16_t *y, int16_t *z)
{
    int8_t rslt = BMI160_OK;
    struct bmi160_sensor_data accel;
    struct bmi160_sensor_data gyro;

    /* To read only Accel data */
    rslt = bmi160_get_sensor_data(BMI160_ACCEL_SEL, &accel, NULL, sensor);

    *x = accel.x;
    *y = accel.y;
    *z = accel.z;
}

typedef struct {
    float delta_x, delta_y;
    float fx, fy;
    uint8_t x, y;
} LEDCoordinates;


void render(LEDCoordinates *coord)
{
    coord->x = (uint8_t) coord->fx;
    coord->y = (uint8_t) coord->fy;

    clear();
    print_at(coord->x, coord->y);
}

void move_by_x(LEDCoordinates *led)
{
    float nx = led->fx + led->delta_x;
    if ( nx > 0.0 && nx < 32.0) {
        led->fx = nx;
    }
}

void move_by_y(LEDCoordinates *led)
{
    float ny = led->fy + led->delta_y;
    if ( ny > 0.0 && ny < 8.0) {
        led->fy = ny;
    }
}

float smooth_data(int16_t x)
{
    return (x / 17000.0);
}

int main()
{
    int16_t x, y, z;
    struct bmi160_dev sensor;
    LEDCoordinates led;

    spi_begin(BMI_SCK, BMI_MISO, BMI_MOSI, BMI_CS, 10000);

    sensor.id = 0;
    sensor.read = &bmi_spi_read;
    sensor.write = &bmi_spi_write;
    sensor.delay_ms = &delay_ms;
    sensor.interface = BMI160_SPI_INTF;
    bmi160_init(&sensor);
    int res = config_sensors(&sensor);

    setup_matrix();

    led.fx = 15.0;
    led.fy = 4.0;

    while(1) {
        get_sensor_data(&sensor, &x, &y, &z);
        led.delta_x = smooth_data(x);
        led.delta_y = smooth_data(y);


        if (led.delta_x > 0.03 || led.delta_x < -0.03) {
            move_by_x(&led);
        }
        if (led.delta_y > 0.03 || led.delta_y < -0.03) {
            move_by_y(&led);
        }
        render(&led);
    }
}

#include <stdio.h>
#include <stdint.h>
#include "gpio.h"
#include "gpio_spi.h"

#define DIN 2
#define CS 3
#define CLK 4
#define DOUT 5 // unused

#define max7219_reg_noop        0x00
#define max7219_reg_digit0      0x01
#define max7219_reg_digit1      0x02
#define max7219_reg_digit2      0x03
#define max7219_reg_digit3      0x04
#define max7219_reg_digit4      0x05
#define max7219_reg_digit5      0x06
#define max7219_reg_digit6      0x07
#define max7219_reg_digit7      0x08
#define max7219_reg_decodeMode  0x09
#define max7219_reg_intensity   0x0a
#define max7219_reg_scanLimit   0x0b
#define max7219_reg_shutdown    0x0c
#define max7219_reg_displayTest 0x0f

const int num_matrices = 4;
uint8_t matrix [32][8];

static void send_byte(uint8_t b)
{
    for (int i = 7; i >=0; i--) {
        set_gpio_pin_value(DIN, (b >> i) & 1);
        set_gpio_pin_value(CLK, 1);
        set_gpio_pin_value(CLK, 0);
    }

}

void send_command(uint8_t cmd, uint8_t val)
{
    set_gpio_pin_value(CS, 0);
    for(int i=0; i < num_matrices; i++) {
        send_byte(cmd);
        send_byte(val);
    }
    set_gpio_pin_value(CS, 1);
}

void set_column(uint8_t col, uint8_t val)
{
    uint8_t n = col / 8;
    uint8_t c = col % 8;

    set_gpio_pin_value(CS, 0);
    for (int i=0; i < num_matrices; i++) {
        if (i == n) {
            send_byte(c + 1);
            send_byte(val);
        } else {
            send_byte(0);
            send_byte(0);
        }
    }
    set_gpio_pin_value(CS, 1);
}

void set_column_all(uint8_t col, uint8_t val)
{
    set_gpio_pin_value(CS, 0);
    for (int i=0; i<num_matrices; i++) {
        send_byte(col + 1);
        send_byte(val);
    }
    set_gpio_pin_value(CS, 1);
}

void clear(void)
{
    for (int i=0; i < 8; ++i) {
        set_column_all(i, 0);
    }
}

void print_at(uint32_t x, uint32_t y)
{
    uint32_t rx = x % 8;
    uint32_t yf = x / 8;
    uint8_t val = (1 << (7 - rx));
    set_column(y + 8 * yf, val);
}

void setup_matrix(void)
{
    spi_begin(CLK, DOUT, DIN, CS, 10000);

    send_command(max7219_reg_scanLimit, 0x07);
    send_command(max7219_reg_decodeMode, 0x00);
    send_command(max7219_reg_shutdown, 0x01);
    send_command(max7219_reg_displayTest, 0x00);

    clear();

    send_command(max7219_reg_intensity, 0x0f);
}

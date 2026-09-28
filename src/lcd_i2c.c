#include "lcd_i2c.h"

static void LCD_send_nibble(uint8_t nibble, uint8_t mode)
{
    uint8_t data = (nibble & 0xF0) | mode | 0x08; // backlight on

    I2C_start();
    I2C_write(LCD_ADDR << 1);
    I2C_write(data | 0x04);  // EN = 1
    _delay_us(1);
    I2C_write(data & ~0x04); // EN = 0
    I2C_stop();
}

void LCD_cmd(uint8_t cmd)
{
    LCD_send_nibble(cmd & 0xF0, 0);
    LCD_send_nibble(cmd << 4, 0);
}

void LCD_write(uint8_t data)
{
    LCD_send_nibble(data & 0xF0, 1);
    LCD_send_nibble(data << 4, 1);
}

void LCD_init(void)
{
    _delay_ms(50);

    LCD_send_nibble(0x30, 0);
    _delay_ms(5);
    LCD_send_nibble(0x30, 0);
    _delay_us(150);
    LCD_send_nibble(0x20, 0);

    LCD_cmd(0x28);
    LCD_cmd(0x0C);
    LCD_cmd(0x06);
    LCD_cmd(0x01);
    _delay_ms(2);
}

void LCD_print(const char *str)
{
    while (*str) LCD_write(*str++);
}

void LCD_goto(uint8_t row, uint8_t col)
{
    static uint8_t addr[] = {0x80, 0xC0, 0x94, 0xD4};
    LCD_cmd(addr[row] + col);
}

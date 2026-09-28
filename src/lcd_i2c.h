#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"

#define LCD_ADDR 0x27

void LCD_init(void);
void LCD_cmd(uint8_t cmd);
void LCD_write(uint8_t data);
void LCD_print(const char *str);
void LCD_goto(uint8_t row, uint8_t col);

#endif

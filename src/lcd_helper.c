#include "lcd_helper.h"
#include "lcd_i2c.h"
#include <stdlib.h>
#include <util/delay.h>

void lcd_print_int(int num) {
    char buf[8];
    itoa(num, buf, 10);
    LCD_print(buf);
}

void lcd_clear_refresh(void) {
    LCD_cmd(0x01);
    _delay_ms(2);
}

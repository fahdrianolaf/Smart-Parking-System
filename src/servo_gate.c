#include "servo_gate.h"
#include <avr/io.h>
#include <util/delay.h>

void servo_init(void) {
    DDRD |= (1 << SERVO_PIN);
    PORTD &= ~(1 << SERVO_PIN);
}

void servo_open(void) {
    for (int i = 0; i < 100; i++) {
        PORTD |= (1 << SERVO_PIN);
        _delay_us(1500);  // 90 degrees
        PORTD &= ~(1 << SERVO_PIN);
        _delay_us(18500);
    }
}

void servo_close(void) {
    for (int i = 0; i < 100; i++) {
        PORTD |= (1 << SERVO_PIN);
        _delay_us(1000);  // 0 degrees
        PORTD &= ~(1 << SERVO_PIN);
        _delay_us(19000);
    }
}
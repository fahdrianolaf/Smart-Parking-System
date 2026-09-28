#include "ultrasonic.h"
#include <avr/io.h>
#include <util/delay.h>

void ultrasonic_init(void) {
    // Trig pins as output
    DDRD |= (1 << US_IN_TRIG_PIN);    // PD2
    DDRB |= (1 << US_OUT_TRIG_PIN);   // PB1
    
    // Echo pins as input (no pull-up)
    DDRC &= ~((1 << US_IN_ECHO_PIN) | (1 << US_OUT_ECHO_PIN));
    PORTC &= ~((1 << US_IN_ECHO_PIN) | (1 << US_OUT_ECHO_PIN));
    
    // Trig pins LOW
    PORTD &= ~(1 << US_IN_TRIG_PIN);
    PORTB &= ~(1 << US_OUT_TRIG_PIN);
}

float ultrasonic_read(uint8_t is_entry) {
    uint32_t duration = 0;
    uint8_t echo_bit, trig_bit;
    
    if (is_entry) {
        // Entry gate: Trig=PD2, Echo=PC1
        echo_bit = US_IN_ECHO_PIN;
        trig_bit = US_IN_TRIG_PIN;
        
        // Wait for echo LOW
        uint16_t wait = 0;
        while ((PINC & (1 << echo_bit)) && wait++ < 10000) _delay_us(1);
        if (wait >= 10000) return -4;
        
        // Send trigger pulse
        PORTD &= ~(1 << trig_bit);
        _delay_us(5);
        PORTD |= (1 << trig_bit);
        _delay_us(10);
        PORTD &= ~(1 << trig_bit);
        
        // Wait for echo HIGH
        wait = 0;
        while (!(PINC & (1 << echo_bit)) && wait++ < 5000) _delay_us(1);
        if (wait >= 5000) return -1;
        
        // Measure pulse duration
        wait = 0;
        while ((PINC & (1 << echo_bit)) && wait++ < 25000) {
            _delay_us(1);
            duration++;
        }
        if (wait >= 25000) return -2;
        
    } else {
        // Exit gate: Trig=PB1, Echo=PC0
        echo_bit = US_OUT_ECHO_PIN;
        trig_bit = US_OUT_TRIG_PIN;
        
        // Wait for echo LOW
        uint16_t wait = 0;
        while ((PINC & (1 << echo_bit)) && wait++ < 10000) _delay_us(1);
        if (wait >= 10000) return -4;
        
        // Send trigger pulse
        PORTB &= ~(1 << trig_bit);
        _delay_us(5);
        PORTB |= (1 << trig_bit);
        _delay_us(10);
        PORTB &= ~(1 << trig_bit);
        
        // Wait for echo HIGH
        wait = 0;
        while (!(PINC & (1 << echo_bit)) && wait++ < 5000) _delay_us(1);
        if (wait >= 5000) return -1;
        
        // Measure pulse duration
        wait = 0;
        while ((PINC & (1 << echo_bit)) && wait++ < 25000) {
            _delay_us(1);
            duration++;
        }
        if (wait >= 25000) return -2;
    }
    
    float distance = (float)duration / 58.0;
    
    // Validate range
    if (distance < DETECT_MIN_CM || distance > DETECT_MAX_CM) return -3;
    
    return distance;
}
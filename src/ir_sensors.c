#include "ir_sensors.h"
#include <avr/io.h>

void ir_init(void) {
    // Set as input with pull-up
    DDRD &= ~(1 << IR_SLOT_1_PIN);  // PD3
    PORTD |= (1 << IR_SLOT_1_PIN);
    
    DDRB &= ~((1 << IR_SLOT_2_PIN) | (1 << IR_SLOT_5_PIN) | (1 << IR_SLOT_6_PIN));
    PORTB |= (1 << IR_SLOT_2_PIN) | (1 << IR_SLOT_5_PIN) | (1 << IR_SLOT_6_PIN);
    
    DDRC &= ~((1 << IR_SLOT_3_PIN) | (1 << IR_SLOT_4_PIN));
    PORTC |= (1 << IR_SLOT_3_PIN) | (1 << IR_SLOT_4_PIN);
}

uint8_t ir_read_slot(uint8_t slot) {
    uint8_t state;
    
    switch(slot) {
        case 1: state = (PIND & (1 << IR_SLOT_1_PIN)) ? 1 : 0; break;
        case 2: state = (PINB & (1 << IR_SLOT_2_PIN)) ? 1 : 0; break;
        case 3: state = (PINC & (1 << IR_SLOT_3_PIN)) ? 1 : 0; break;
        case 4: state = (PINC & (1 << IR_SLOT_4_PIN)) ? 1 : 0; break;
        case 5: state = (PINB & (1 << IR_SLOT_5_PIN)) ? 1 : 0; break;
        case 6: state = (PINB & (1 << IR_SLOT_6_PIN)) ? 1 : 0; break;
        default: return 1;
    }
    
    return state; // Active LOW: 0=terisi, 1=kosong
}

void ir_update_all(uint8_t *slot_status) {
    for (uint8_t i = 0; i < NUM_SLOTS; i++) {
        slot_status[i] = ir_read_slot(i + 1);
    }
}
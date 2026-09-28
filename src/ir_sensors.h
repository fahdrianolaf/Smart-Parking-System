#ifndef IR_SENSORS_H
#define IR_SENSORS_H

#include <stdint.h>

#define IR_SLOT_1_PIN   3    // PD3
#define IR_SLOT_2_PIN   4    // PB4 (bit position in PORTB)
#define IR_SLOT_3_PIN   2    // PC2 (bit position in PORTC)
#define IR_SLOT_4_PIN   3    // PC3 (bit position in PORTC)
#define IR_SLOT_5_PIN   2    // PB2 (bit position in PORTB)
#define IR_SLOT_6_PIN   3    // PB3 (bit position in PORTB)

#define NUM_SLOTS       6

void ir_init(void);
uint8_t ir_read_slot(uint8_t slot); // Returns 1=kosong, 0=terisi
void ir_update_all(uint8_t *slot_status);

#endif
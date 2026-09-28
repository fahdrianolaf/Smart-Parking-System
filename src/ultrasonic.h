#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdint.h>

#define US_IN_TRIG_PIN  2    // PD2
#define US_IN_ECHO_PIN  1    // PC1 (bit position in PORTC)
#define US_OUT_TRIG_PIN 1    // PB1 (bit position in PORTB)
#define US_OUT_ECHO_PIN 0    // PC0 (bit position in PORTC)

#define DETECT_MIN_CM   2.0
#define DETECT_MAX_CM   4.0

void ultrasonic_init(void);
float ultrasonic_read(uint8_t is_entry); // 1=entry, 0=exit

#endif
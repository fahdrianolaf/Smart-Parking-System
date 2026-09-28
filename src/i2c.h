#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <util/delay.h>

// Konfigurasi I2C untuk Arduino Uno (100kHz)
#define F_I2C 100000L
#define F_CPU 16000000L
#define TWBR_VALUE (((F_CPU / F_I2C) - 16) / 2)

// Prototipe fungsi
void I2C_init(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_write(uint8_t data);
uint8_t I2C_read(uint8_t ack);
uint8_t I2C_read_ack(void);
uint8_t I2C_read_nack(void);

#endif
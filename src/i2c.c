#include "i2c.h"

// Inisialisasi I2C
void I2C_init(void) {
    // Set clock frequency (100kHz untuk F_CPU=16MHz)
    TWSR = 0x00;  // Prescaler = 1
    TWBR = TWBR_VALUE;
    
    // Enable I2C
    TWCR = (1 << TWEN);
}

// Start condition
void I2C_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    
    // Check status
    uint8_t status = TWSR & 0xF8;
    if (status != 0x08) {  // START condition transmitted
        // Error handling bisa ditambahkan di sini
    }
}

// Stop condition
void I2C_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    _delay_us(10);
}

// Write byte
void I2C_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    
    // Check status
    uint8_t status = TWSR & 0xF8;
    if (status != 0x18 && status != 0x28 && status != 0x40) {
        // SLA+W transmitted & ACK received (0x18)
        // Data transmitted & ACK received (0x28)
        // SLA+R transmitted & ACK received (0x40)
        // Error handling bisa ditambahkan di sini
    }
}

// Read byte dengan ACK/NACK
uint8_t I2C_read(uint8_t ack) {
    if (ack) {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    } else {
        TWCR = (1 << TWINT) | (1 << TWEN);
    }
    
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

// Read byte dengan ACK
uint8_t I2C_read_ack(void) {
    return I2C_read(1);
}

// Read byte dengan NACK
uint8_t I2C_read_nack(void) {
    return I2C_read(0);
}
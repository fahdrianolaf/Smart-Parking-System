#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "ultrasonic.h"
#include "ir_sensors.h"
#include "servo_gate.h"
#include "lcd_i2c.h"
#include "lcd_helper.h"
#include "uart.h"

// Global variables
uint8_t slot_status[NUM_SLOTS] = {1,1,1,1,1,1};
uint8_t gate_active = 0;

// Helper functions
uint8_t get_available_slots(void) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < NUM_SLOTS; i++) {
        if (slot_status[i]) count++;
    }
    return count;
}

void display_slots(void) {
    lcd_clear_refresh();
    
    // Row 1: S1-S3
    LCD_goto(0, 0);  LCD_print("S1:"); LCD_print(slot_status[0] ? "K " : "I ");
    LCD_goto(0, 7);  LCD_print("S2:"); LCD_print(slot_status[1] ? "K " : "I ");
    LCD_goto(0, 14); LCD_print("S3:"); LCD_print(slot_status[2] ? "K" : "I");
    
    // Row 2: S4-S6
    LCD_goto(1, 0);  LCD_print("S4:"); LCD_print(slot_status[3] ? "K " : "I ");
    LCD_goto(1, 7);  LCD_print("S5:"); LCD_print(slot_status[4] ? "K " : "I ");
    LCD_goto(1, 14); LCD_print("S6:"); LCD_print(slot_status[5] ? "K" : "I");
    
    // Row 3: Available count
    LCD_goto(2, 0);
    LCD_print("Kosong: ");
    lcd_print_int(get_available_slots());
    LCD_print("/6");
    
    // Row 4: Status
    LCD_goto(3, 0);
    LCD_print(get_available_slots() == 0 ? "SLOT PARKIR PENUH" : "                 ");
}

void handle_entry_gate(float distance) {
    uint8_t available = get_available_slots();
    
    UART_print("\r\n>>> [IN] Deteksi: ");
    char buf[10];
    itoa((int)distance, buf, 10);
    UART_print(buf);
    UART_print("cm | Slot: ");
    itoa(available, buf, 10);
    UART_print(buf);
    UART_print("\r\n");
    
    if (available > 0) {
        LCD_goto(3, 0);
        LCD_print("SILAHKAN MASUK  ");
        UART_print(">>> Buka gate IN\r\n");
        
        servo_open();
        gate_active = 1;
        
        _delay_ms(3000); // Tunggu mobil lewat
        
        UART_print(">>> Tutup gate\r\n");
        servo_close();
        gate_active = 0;
        
        display_slots();
    } else {
        LCD_goto(3, 0);
        LCD_print("SLOT PARKIR PENUH");
        UART_print(">>> DITOLAK (penuh)\r\n");
        
        _delay_ms(2000);
        display_slots();
    }
}

void handle_exit_gate(float distance) {
    // PERUBAHAN: Hapus pengecekan apakah ada mobil di dalam
    // Gate exit akan selalu bisa dibuka jika ada deteksi
    
    UART_print("\r\n>>> [OUT] Deteksi: ");
    char buf[10];
    itoa((int)distance, buf, 10);
    UART_print(buf);
    UART_print("cm\r\n");
    
    LCD_goto(3, 0);
    LCD_print("SILAHKAN KELUAR ");
    UART_print(">>> Buka gate OUT\r\n");
    
    servo_open();
    gate_active = 1;
    
    _delay_ms(3000); // Tunggu mobil lewat
    
    UART_print(">>> Tutup gate\r\n");
    servo_close();
    gate_active = 0;
    
    display_slots();
}

int main(void) {
    uint32_t tick = 0;
    uint32_t last_ir_check = 0;
    
    // Initialize all systems
    ultrasonic_init();
    ir_init();
    servo_init();
    I2C_init();
    LCD_init();
    UART_init(9600);
    
    // Welcome screen
    lcd_clear_refresh();
    LCD_goto(0, 0); LCD_print("SISTEM PARKIR 6");
    LCD_goto(1, 0); LCD_print("SLOT - 1 SERVO");
    LCD_goto(2, 0); LCD_print("Range: 2-6 cm");
    
    UART_print("\r\n=== SISTEM PARKIR 6 SLOT ===\r\n");
    UART_print("Servo: Pin 0 (PD0)\r\n");
    UART_print("US-IN : Trig=2, Echo=A1\r\n");
    UART_print("US-OUT: Trig=9, Echo=A0\r\n");
    UART_print("IR: 3,12,A2,A3,10,11\r\n");
    UART_print("Exit gate: SELALU AKTIF\r\n");
    UART_print("============================\r\n\n");
    
    _delay_ms(2000);
    display_slots();
    
    UART_print(">>> Sistem aktif\r\n\n");
    
    while (1) {
        // Check IR sensors every 200ms
        if (tick - last_ir_check >= 200) {
            last_ir_check = tick;
            
            uint8_t old_status[NUM_SLOTS];
            for (uint8_t i = 0; i < NUM_SLOTS; i++) old_status[i] = slot_status[i];
            
            ir_update_all(slot_status);
            
            // Check if any slot changed
            uint8_t changed = 0;
            for (uint8_t i = 0; i < NUM_SLOTS; i++) {
                if (slot_status[i] != old_status[i]) {
                    changed = 1;
                    UART_print("Slot ");
                    char buf[5];
                    itoa(i+1, buf, 10);
                    UART_print(buf);
                    UART_print(": ");
                    UART_print(slot_status[i] ? "KOSONG\r\n" : "TERISI\r\n");
                }
            }
            
            if (changed) display_slots();
        }
        
        // Check entry ultrasonic every 100ms
        if (!gate_active && tick % 100 == 0) {
            float dist = ultrasonic_read(1); // Entry
            if (dist > 0) handle_entry_gate(dist);
        }
        
        // Check exit ultrasonic every 100ms (offset 50ms)
        // PERUBAHAN: Gate exit selalu aktif, tidak peduli status parkir
        if (!gate_active && (tick + 50) % 100 == 0) {
            float dist = ultrasonic_read(0); // Exit
            if (dist > 0) handle_exit_gate(dist);
        }
        
        tick++;
        _delay_ms(1);
    }
    
    return 0;
}
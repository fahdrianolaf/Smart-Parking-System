#include <avr/io.h>
#include "uart.h"

void UART_init(uint32_t baud){ uint16_t ubrr = (16000000UL/(16UL*baud))-1; UBRR0H = (ubrr>>8); UBRR0L = ubrr; UCSR0B = (1<<TXEN0); UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);} 
void UART_tx(char c){ while(!(UCSR0A & (1<<UDRE0))); UDR0 = c;} 
void UART_print(const char *s){ while(*s) UART_tx(*s++);} 
void UART_printNum(int num){ char buf[10]; itoa(num, buf,10); UART_print(buf);}

#ifndef UART_H
#define UART_H
#include <stdint.h>

void UART_init(uint32_t baud);
void UART_tx(char c);
void UART_print(const char *s);
void UART_printNum(int num);

#endif

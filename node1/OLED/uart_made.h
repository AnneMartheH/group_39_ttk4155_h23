#pragma once

#include <avr/io.h>
#include <stdio.h>

void uart_init_made(unsigned int ubrr);
int uart_transmit(char data, FILE * file);
int uart_receive(FILE * file);
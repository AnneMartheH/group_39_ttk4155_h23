#include "uart_made.h"

// code gotten from https://github.com/erlendb/TTK4155-Byggern-labhjelp/tree/master

void uart_init_made(unsigned int ubrr) {
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);

	// Ingenting over denne linjen er forandret

	fdevopen(uart_transmit, uart_receive);
}

int uart_transmit(char data, FILE * file) {
	/* Wait for empty transmit buffer */
	while (! (UCSR0A & (1 << UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;

	return 0;
}

int uart_receive(FILE * file){
	/* Wait for data to be received */
	while ( !(UCSR0A & (1 << RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}
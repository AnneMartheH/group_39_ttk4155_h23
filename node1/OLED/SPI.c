#include "SPI.h"
#include <avr/io.h>

// some code gotten from https://github.com/erlendb/TTK4155-Byggern-labhjelp/tree/master
void spi_ss_high(){
	PORTB |= (1 << DD_SS);
}

void spi_ss_low(){
	PORTB &= ~(1 << DD_SS);
}

void SPI_MasterInit(void)
{
	//set SCK. MOSI as output
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);
	//Enable master and SPI, clock rate = f/16
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
	spi_ss_high();
}
void SPI_MasterTransmit(char cData)
{
	SPDR = cData; //transmit
	while(!(SPSR & (1<<SPIF))){//wait
	} 
}
void spi_write(char cData) {

	SPDR = cData; //start
	while(!(SPSR & (1<<SPIF))) { //wait for copleteded
	}
}

uint8_t spi_read() {
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF))) { //same as above, a wait
	}
	return SPDR; //return register that is read into
}
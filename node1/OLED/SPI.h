#include <stdio.h>

//ex. 5 pins and ports from Atmega to SPI interface
#define DDR_SPI DDRB
#define DD_SS PB4
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7

//sets SS high. Otherwise portB unchanged
void spi_ss_high();
void spi_ss_low();

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);
void spi_write(char cData);
uint8_t spi_read();
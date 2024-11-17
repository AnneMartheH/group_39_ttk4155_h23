#define F_CPU 4915200

#include "mcp2515.h"
#include "SPI.h"

#include "util/delay.h"

// these standard functions are gotten from:
//https://github.com/erlendb/TTK4155-Byggern-labhjelp/blob/master/lab5.md 

void mcp_init() {
	SPI_MasterInit();
	mcp_reset(); 
	_delay_ms(10); 
}

void mcp_reset() {
	spi_ss_low(); //device select
	spi_write(MCP_RESET); 
	spi_ss_high();
}

uint8_t mcp_read(uint8_t adr) {
	spi_ss_low();
	spi_write(MCP_READ);
	spi_write(adr);
	uint8_t data = spi_read();
	spi_ss_high();

	return data;
}

void mcp_write(uint8_t adr, uint8_t data) {
	spi_ss_low();
	spi_write(MCP_WRITE);
	spi_write(adr);
	spi_write(data);
	spi_ss_high();
}


char mcp_read_status() {
	spi_ss_low();
	spi_write(MCP_READ_STATUS);
	char read = spi_read();
	spi_ss_high();

	return read;
}

void mcp_bit_modify(uint8_t adr, uint8_t command, uint8_t data) {
	spi_ss_low();
	spi_write(MCP_BITMOD);
	spi_write(adr); 
	spi_write(command); 
	spi_write(data); 
	spi_ss_high();
	//_delay_ms(10);
}

void mcp_request_to_send(int buffer_number) {
	spi_ss_low();
	buffer_number = buffer_number % 3; // Mapper buffernummer til 0, 1, 2
	char data = MCP_RTS_TX0;
	if (buffer_number == 0) {
		data = MCP_RTS_TX0;
		} else if (buffer_number == 1) {
		data = MCP_RTS_TX1;
		} else if (buffer_number == 2) {
		data = MCP_RTS_TX2;
	}
	spi_write(data);
	spi_ss_high();
}

void mcp_set_mode(uint8_t mode) {
	mcp_bit_modify(MCP_CANCTRL, 0b11100000, mode);
}



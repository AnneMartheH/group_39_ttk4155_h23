#include "CAN_comm.h"
#include <avr/io.h>


#define MCP_TXB0SIDH 0x31 
#define MCP_TXB0SIDL 0x32 
#define MCP_TXB0EID8 0x33
#define MCP_TXB0EID0 0x34
#define MCP_TXB0DLC 0x35 
#define MCP_TXB0D0 0x36 
#define MCP_TXB0D1 0x37
#define MCP_TXB0D2 0x38
#define MCP_TXB0D3 0x39
#define MCP_TXB0D4 0x3A
#define MCP_TXB0D5 0x3B
#define MCP_TXB0D6 0x3C
#define MCP_TXB0D7 0x3D

#define MCP_RXB0SIDH 0x61 
#define MCP_RXB0SIDL 0x62 
#define MCP_RXB0DLC 0x65
#define MCP_RXB0D0 0x36 
#define MCP_RXB0D1 0x37
#define MCP_RXB0D2 0x38
#define MCP_RXB0D3 0x39
#define MCP_RXB0D4 0x3A
#define MCP_RXB0D5 0x3B
#define MCP_RXB0D6 0x3C
#define MCP_RXB0D7 0x3D

void CAN_send_message(uint8_t address, CanMessage message) {
	
	mcp_write(MCP_TXB0DLC, message.length); 
	mcp_write(MCP_TXB0SIDL, message.id);
	mcp_write(MCP_TXB0SIDH, 0b00000000);
	
	spi_write(address); 
	for (int i = 0; i < message.length; i++) 
	{
		mcp_write(MCP_TXB0D0 + i, message.data[i]); 
	}

	mcp_request_to_send(0);
}

CanMessage CAN_recive_message(){
	CanMessage rec_message;
	 rec_message.length = mcp_read(MCP_RXB0DLC); 
	 rec_message.id = mcp_read(MCP_RXB0SIDL);
	 
	 for (int i = 0; i < rec_message.length ; i++)
	 {
		 rec_message.data[i] = mcp_read(MCP_RXB0D0 + i);
	 }
	 return rec_message;
}

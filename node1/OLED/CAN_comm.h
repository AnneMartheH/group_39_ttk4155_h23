#pragma once

#include <avr/io.h>
#include <stdio.h>
#include "mcp2515.h"


//typedef struct{
	//uint8_t id;
	//uint8_t length;
	//uint8_t data[8];
//}CanMessage;
//
//void CAN_send_message(uint8_t address, CanMessage message);

// Send-buffer 0
//#define MCP_TXB0SIDH 0x31 //0b0011 0001
//#define MCP_TXB0SIDL 0x32 //0b0011 0010
//#define MCP_TXB0EID8 0x33
//#define MCP_TXB0EID0 0x34
//#define MCP_TXB0DLC 0x35 //0b0011 0101
//#define MCP_TXB0D0 0x36 //0b0011 0110
//#define MCP_TXB0D1 0x37
//#define MCP_TXB0D2 0x38
//#define MCP_TXB0D3 0x39
//#define MCP_TXB0D4 0x3A
//#define MCP_TXB0D5 0x3B
//#define MCP_TXB0D6 0x3C
//#define MCP_TXB0D7 0x3D

typedef struct{
	uint16_t id;
	//uint8_t id_l;
	uint8_t length;
	uint8_t data[8];
}CanMessage;

typedef struct Byte8 Byte8;
struct Byte8 {
	uint8_t bytes[8];
};

struct CanMsg {
	uint8_t id;
	uint8_t length;
	union {
		uint8_t     byte[8];
		uint32_t    dword[2];
		Byte8       byte8;
	};
};

void CAN_send_message(uint8_t address, CanMessage message);
CanMessage CAN_recive_message();
//void print_can_message(const CANMessage* msg);

	//For CNF1
	#define SHIFT_SJW 7
	#define SHIFT_BRP 5
	//For CNF2
	#define SHIFT_BTL 7
	#define SHIFT_SAM 6
	#define SHIFT_PS1 5
	#define SHIFT_PSEG 3
	//For CNF3
	#define SHIFT_PS2 2
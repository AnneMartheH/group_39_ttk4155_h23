#include "gameboard.h"

#include <stdio.h>
#include <stdint.h>
#include "sam.h"

//uint8_t prev_x_joystick = 0;

uint16_t joystckPosToTicks(uint8_t x_pos_joystick){
	//printf(x_pos_joystick);
	if (x_pos_joystick <= 165 && x_pos_joystick >= 155)// neutral
	{
		//printf("neutral");
		return 1968; // 1968 ticks == neutral pos = 1,5 ms 
	}
	else if( x_pos_joystick < 155){ // left
		int diff = x_pos_joystick - 155;
		float prosentage = ((float)diff / 1.55)/100;
		int ticks_subtracted = 786 * prosentage;
		int ticks = 1968 + ticks_subtracted;
		//printf("low");
		return ticks;
		
	}
	else { // right
	//else if( x_pos_joystick > 155){ // right	
		int diff = x_pos_joystick - 165; // pos - neutral
		float prosentage = ((float)diff / 0.9)/100; // precentage aboove neuytral
		int ticks_added = 786 * prosentage; //786 is max ticks you can add 
		int ticks = 1968 + ticks_added; // 1968 = neutral nubmer of ticks 
		//printf("high");
		return ticks;
		//convert to binary
		
	}
}

uint16_t joystckPosToTicks_speed(uint8_t y_pos_joystick){
		if (y_pos_joystick <= 170 && y_pos_joystick >= 160)// neutral
		{
			//printf("neutral");
			return 0; // 0 = no speed
		}
		else if( y_pos_joystick < 160){ // left
			//printf("low");
			PIOC->PIO_SODR |= PIO_SODR_P23;
			return 0b10000;
			
		}
		else {//right
			PIOC->PIO_CODR |= PIO_CODR_P23;
			return 0b10000;
		}
	
}

void switch_solinod_controller(uint8_t button_joystick){
	if (button_joystick == 255){// Default
		PIOB->PIO_CODR |= PIO_CODR_P16;
	}
	else{
		PIOB->PIO_SODR |= PIO_SODR_P16;
	}
}


// converter made by chat cpt
uint16_t decimalToBinary(uint16_t n) { 
	uint16_t binary = 0;  
	uint16_t place = 1;    

	for (int i = 0; i < 16; i++){
		if (n & 1) {
			binary |= place;  
		}
		n >>= 1;  
		place <<= 1;
	}
	
	return binary;
}


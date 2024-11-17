#define FOSC 4915200
#include <avr/io.h>
#include <util/delay.h>

#include "oled.h"
#include "joystick.h"
#include "uart_made.h"
#include "mcp2515.h"
#include "CAN_comm.h"

#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1 // UBRR value calculation (Page 169 of ATmega162 datasheet)

// OLED works in such a way that you write one byte (8 bits) to it at a time.
//volatile char *ext_oled_command = (char *) 0x1000; // OLED Command starting Address
//volatile char *ext_oled_data = (char *) 0x1200; // OLED Data starting Address
//volatile char *adc = (char *) 0x1400; // Start address of ADC

void adc_init() {
	MCUCR = (1 << SRE);  // Enable external memory interface
	SFIOR = (1 << XMM2); // Mask PC4-PC7 to avoid JTAG conflict
}
void timer1_init() {
	TCCR1B |= (1 << WGM12);   
	OCR1A = 0;                
	TCCR1B |= (1 << CS10);    
	TCCR1A |= (1 << COM1A0);  
	DDRD |= (1 << PD5);      
}

int menu_position = 0;
int skip_menu = 0;

int main(void) {
	enable_external_memory();
	init_oled(); // OLED initialization
	timer1_init(); //change this name, gjør masse setter outouts og ++
	uart_init_made(MYUBRR);
	adc_init();	
	mcp_init();
	
	mcp_set_mode(MODE_CONFIG);
	
	CanMessage  message;
	message.id = 0b11100000; // Only the three MSB that matter, for more explanation translate this: kun de tre første bitsene (MSB / til venstre) som styrer id, noe rart med koden her // endret utdelt kode til å motta unit16.
	message.length = 4;
	message.data[0] = get_joystick_pos().x_value;
	message.data[1] = get_joystick_pos().y_value;
	message.data[2] = get_joystick_pos().joy_buttom;
	message.data[3] = 0;
	
	// MCP2515 Controller setting for CAN Bus bit timings, check page 43 of MCP Controller data sheet
	// TQ = (2(3+1))/16Mz = 500 ns
	// Assuming CAN Baud Rate = 125 kHz (Check page 43 on MCP2515 datasheet) - To obtain this we need to set Bit Time to 16 TQs
	mcp_write(MCP_CNF1, 0b00000011); // SJW=0 (1) BRP=3
	mcp_write(MCP_CNF2, 0b10110000); // BTLMODE=1, SAM=0, PS1=6 (7), PRSEG=0 (1)
	mcp_write(MCP_CNF3, 0b00000101); // PS2=5 (6)	
	mcp_set_mode(MODE_NORMAL);
	print_menu_w_pos(menu_position);
	
	while (1)
	{
		//skip menu = 0 do menu 
		//else send can message 
		 if(skip_menu == 0){
			 if(joystick_moved_up(get_joystick_pos().y_value)){
				 if(menu_position >= 1){
					menu_position --;
					print_menu_w_pos(menu_position); 
				 }
			 }
			 if (joystick_moved_down(get_joystick_pos().y_value)){
				 if(menu_position <= 1){
					 menu_position++;
					 print_menu_w_pos(menu_position);
				 }
			 }
			 if(get_joystick_pos().joy_buttom <= 10 && menu_position == 0){ //start game
				 skip_menu = 1;
				 game_screen();
			 }
			// _delay_ms(1000);
		 }
		 else if (skip_menu == 1){
			JoystickPos joystick =  get_joystick_pos();
			message.data[0] = joystick.x_value;
			message.data[1] = joystick.y_value;
			message.data[2] = joystick.joy_buttom;
			message.data[3] = 1; //Variable to check when you have to play game
			CAN_send_message(MCP_TXB0SIDH, message);
			printf("sending can: \r\n"); 
		 }
		//printf("menu position: %d\r\n", menu_position);
		//printf("skip menu: %d\r\n", skip_menu);
		//printf("joy_buttom: %d\r\n \n", message.data[2]);
		_delay_ms(20);	
	}	
} 
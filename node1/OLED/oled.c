#include "oled.h"
#include "fonts.h"
// some code gotten from https://github.com/erlendb/TTK4155-Byggern-labhjelp/tree/master

volatile char *ext_oled_command = (char *) 0x1000; // OLED Command starting Address
volatile char *ext_oled_data = (char *) 0x1200; // OLED Data starting Address

void write_c(uint8_t command) {
	ext_oled_command[0] = command;
}

// Function for writing data on the Oled
void oled_write_data(uint8_t data) {

	ext_oled_data[0] = data;
}

// Selecting line bet 0 - 7
void oled_goto_line(int line) {
	write_c(0xB0 + line); // enter the number of the line you want to go to in the variable line
}

// Selecting column bet 0 - 127, e.g 97 = 0b 0110 0001
void oled_goto_column(int column) {
	write_c(0x00 + (column % 16)); // Lower nibble (4 LSBs = 0110)
	write_c(0x10 + (column / 16)); // Higher nibble (4 MSBs = 0001)
}

void oled_goto_pos(int line,int column){
	oled_goto_line(line);
	oled_goto_column(column);
}

void oled_print_char(char c) {
	c = c-32;
	for (int i = 0; i < 8; i++) {
		int byte = pgm_read_byte(&font8[c][i]);
		oled_write_data(byte);
	}
}

void oled_print(char c[]) {
	for (int i = 0; i < strlen(c); i++) {
		oled_print_char(c[i]);
	}
}


void clear_screen(){
	// set all pixels on the screen low
	for (int line = 0; line < 8; line++) {
		oled_goto_line(line);
		oled_goto_column(0);
		for (int i = 0; i < 128; i++) {
			oled_write_data(0x00);
		}
	}
}

void enable_external_memory(){
	MCUCR = (1 << SRE); // ATmega162 P 30 Datasheet
	SFIOR &= ~(111<XMM0);
	SFIOR |= (1 << XMM2); // Masking PC4-PC7 (JTAG pins). P 32 Datasheet
}


void init_oled()
{
	write_c(0xae); // display off
	write_c(0xa1); //segment remap
	write_c(0xda); //common pads hardware: alternative
	write_c(0x12);
	write_c(0xc8); //common output scan direction:com63~com0
	write_c(0xa8); //multiplex ration mode:63
	write_c(0x3f);
	write_c(0xd5); //display divide ratio/osc. freq. mode
	write_c(0x80);
	write_c(0x81); //contrast control
	write_c(0x50);
	write_c(0xd9); //set pre-charge period
	write_c(0x21);
	write_c(0x20); //Set Memory Addressing Mode
	write_c(0x02);
	write_c(0xdb); //VCOM deselect level mode
	write_c(0x30);
	write_c(0xad); //master configuration
	write_c(0x00);
	write_c(0xa4); //out follows RAM content
	write_c(0xa6); //set normal display
	write_c(0xaf); // display on
}

void print_menu_w_pos(int pos){
	if (pos == 0){
		clear_screen();
		oled_goto_line(1);
		oled_goto_column(0);
		//oled_print_char('G');  //use '' for printing char
		oled_print("MENU"); // use "" for printing strings
		oled_goto_pos(2,0);
		oled_print(" Start game *");
		oled_goto_pos(3,0);
		oled_print(" Highscores");
		oled_goto_pos(4,0);
		oled_print(" Settings  ");
	}
	if (pos == 1){
		clear_screen();
		oled_goto_line(1);
		oled_goto_column(0);			//oled_print_char('G');  //use '' for printing char
		oled_print("MENU"); // use "" for printing strings
		oled_goto_pos(2,0);
		oled_print(" Start game ");
		oled_goto_pos(3,0);
		oled_print(" Highscores *");
		oled_goto_pos(4,0);
		oled_print(" Settings  ");
	}
	if (pos == 2){
		clear_screen();
		oled_goto_line(1);
		oled_goto_column(0);
		//oled_print_char('G');  //use '' for printing char
		oled_print("MENU"); // use "" for printing strings
		oled_goto_pos(2,0);
		oled_print(" Start game ");
		oled_goto_pos(3,0);
		oled_print(" Highscores ");
		oled_goto_pos(4,0);
		oled_print(" Settings   *");
	}
	
}

void game_screen(){
	clear_screen();
	oled_goto_line(1);
	oled_goto_column(0);
	oled_print("PLAYING PING");
}

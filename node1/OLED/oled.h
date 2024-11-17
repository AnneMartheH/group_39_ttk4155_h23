#pragma once

#include <avr/io.h>
#include <stdio.h>


 
void clear_screen();
void enable_external_memory();

void write_c(uint8_t command);
void oled_write_data(uint8_t data);

void init_oled();
void print_menu_w_pos(int pos);
void game_screen();



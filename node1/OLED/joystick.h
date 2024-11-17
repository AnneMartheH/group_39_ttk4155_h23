#pragma once

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include <stdbool.h>

bool joystick_moved_right(uint8_t x_value);
bool joystick_moved_left(uint8_t x_value);
bool joystick_moved_up(uint8_t y_value);
bool joystick_moved_down(uint8_t y_value);

typedef struct{
	uint8_t x_value;
	uint8_t y_value;
	uint8_t left_slider;
	uint8_t joy_buttom;
	}JoystickPos;

JoystickPos get_joystick_pos();

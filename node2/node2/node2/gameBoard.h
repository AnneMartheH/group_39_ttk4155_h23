#pragma once
#include <stdio.h>

uint16_t joystckPosToTicks(uint8_t x_pos_joystick);
uint16_t joystckPosToTicks_speed(uint8_t y_pos_joystick);
void switch_solinod_controller(uint8_t button_joystick);
uint16_t decimalToBinary(uint16_t n);

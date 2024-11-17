#pragma once
#include <stdio.h>
#include "sam.h"

void IR_init(); // move to different module
void init_ir_goal_counter();
void IR_goal_counter(uint32_t ir_val);

void PWM_init();

void encoder_init();
void solinoid_init();
float get_scaled_encoder_pos();
int get_position_error(int encoder_pos, int joystick_pos);
int check_windup(int historical_error);



int position_controller(int scaled_encoder, int joystick_pos);
float  scale_joystick_y(int joystick_y);

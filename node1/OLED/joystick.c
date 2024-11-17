#include "joystick.h"

bool joystick_moved_right(uint8_t x_value){
	return (x_value >= 210);
}
bool joystick_moved_left(uint8_t x_value){
	return (x_value <= 80);
}
bool joystick_moved_up(uint8_t y_value){
	return (y_value >= 210);
}
bool joystick_moved_down(uint8_t y_value){
	return (y_value <= 80);
}

JoystickPos get_joystick_pos(){
	volatile char *adc = (char *) 0x1400; // Start address of ADC
	adc[0] = 0x04;  // Write 0x04 to select CH0 (AN0)
	JoystickPos joystick;
	
	_delay_us(200);  // Small delay to allow the ADC to process
	joystick.x_value = adc[0];  // Read the ADC value from CH0
	joystick.y_value = adc[1]; // We are reading in a sequential way so its fine to read from the same value
	//joystick.left_slider = adc[2];  // Left Slider
	joystick.joy_buttom = adc[2];
	
	return joystick;
}

	
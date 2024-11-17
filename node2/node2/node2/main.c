
#include "sam.h"
#include "can.h"
#include "time.h"
#include "uart.h"
#include "gameBoard.h"
#include "motors.h"

int main(void)
{
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer in Atsem which is by default enabled that periodically reset
	SystemInit();
	uart_init(84000000,9600); // Cpu frequency and baudrate
	
	CanMsg recMssage;
	CanInit initValues;
	
	initValues.brp = 41; //CAN_BR_BRP(41); TQ = (41+1)/(84Mhz) = 500 ns
	initValues.phase1 = 6;
	initValues.phase2 = 5;
	initValues.propag = 0;
	initValues.sjw = 0;
	initValues.smp = 0;
	
	can_init(initValues,0);
	IR_init(); // Configuring Pin 8 on Ardunio to work as Input pin for IR sensor
	init_ir_goal_counter();
	PWM_init();
	encoder_init();
	solinoid_init();
	
	while (1)
	{
		can_rx(&recMssage);
		//can_printmsg(recMssage);
		
		//Storing Can Values from Node 1
		int joystick_x_Servo = recMssage.byte[0];
		int joystick_y_Motor = recMssage.byte[1];
		int switch_Solenoid = recMssage.byte[2];
		int play_game = recMssage.byte[3];
		
		if (play_game == 1){
			//Servo
			uint16_t servo_duty_cycle_dec =  joystckPosToTicks(joystick_x_Servo);
			uint16_t servo_duty_cycle_bin = decimalToBinary(servo_duty_cycle_dec);
			PWM->PWM_CH_NUM[1].PWM_CDTY=servo_duty_cycle_bin;  // 2 756 ticks setting duty cycle to control servo position		
			//printf("\n \r Joystick Ticks from Node 1: %d \r\n", ticks);
		
			//DC Motor
			int scaled_encoder = get_scaled_encoder_pos();
			int scaled_joystick_y = scale_joystick_y(joystick_y_Motor);
			int decimal_controller_output = position_controller(scaled_encoder, scaled_joystick_y);
			uint16_t binary_controll_output = decimalToBinary(decimal_controller_output);
			//printf("\r  output of controller in decimal (speed): %d \r \n",  decimal_controller_output);
			//printf("\r  error sum from function: %d \r \n",  error_sum);
		
			PWM->PWM_CH_NUM[0].PWM_CDTY=binary_controll_output; //to control the motor speed
		
			//printf("\r  Scaled encoder position = current position: %d \r \n",  scaled_encoder);
			//printf("\r  Scaled joystick position : %d \r \n",  scaled_joystick_y);
			//printf("\r joystick pos not scaled: %d \r \n",  joystick_y_Motor);
			//printf(" \n");
		
			// Control solenoid based on joystick button value
			switch_solinod_controller(switch_Solenoid);
		
		
			//****
			// Read the converted data from the ADC's Last Converted Data Register (ADC_LCDR)
			// The result is available even if the conversion is still running
			uint32_t ir_value = ADC->ADC_CDR[10];  // Read the last conversion result
			//printf("IR Sensor Value on Node 2: %d\r\n", ir_value); // Output the ADC result via UART
			IR_goal_counter(ir_value);
		
		
			///*** Encoder
			//int Encoder_Value = TC2->TC_CHANNEL[0].TC_CV;  // Read the last conversion result
			//printf("\r Encoder Position: %d \r \n",  Encoder_Value);
		}
		
		time_spinFor(msecs(200));
	}
}



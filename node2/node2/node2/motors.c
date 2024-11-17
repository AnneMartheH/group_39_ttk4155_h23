#include "motors.h"
#include "sam.h"

float hist_error = 0;
uint32_t prev_ir_value; 
uint32_t ir_treshold; 
int goals;

void IR_init(){
	// Enable the clock for the ADC (in the PMC)
	PMC->PMC_PCER1 |= (1 << (ID_ADC-32));  // Enable the clock for the ADC peripheral
	
	// Configure the PIO to use the ADC input pin - A8 on the Sheild!! REMEMBER
	// Ensure the pin is mapped to the ADC peripheral (not GPIO)
	PIOA->PIO_ABSR |= (PIO_ABSR_P10);  
	PIOA->PIO_PDR |= (PIO_PDR_P10);    
	
	// ADC Reset
	ADC->ADC_CR = ADC_CR_SWRST;      
	
	// Configure ADC Mode Register
	ADC->ADC_MR = ADC_MR_FREERUN_ON     
	| ADC_MR_PRESCAL(64);    
	
	 // Enable the ADC channel 10
	ADC->ADC_CHER = (1 << 10); 
	
	ADC->ADC_CR |= ADC_CR_START;
}

void init_ir_goal_counter(){
	prev_ir_value = 3000; //over treshold of goal
	ir_treshold = 1200; // should be a define so you cant change it
	goals = 0;
	
}

void IR_goal_counter(uint32_t ir_val){
	//when blocked ir = close to 0
	//when unblocked = bigger than 100, gratest value reived was over 3000 
	
	if ( ir_val < ir_treshold && prev_ir_value > ir_treshold) //100 = ir_treshold 
	{
		goals ++;
		printf("goal !!, numer of goals is now: %d \r \n", goals); //should idealy be sent to node 1, and showed on OLED
	}
	prev_ir_value = ir_val;
	printf("goals is: %d \r \n", goals);
	
}

void PWM_init(){
		//frist program the PIO controller to assigne the PWM pins to perhipial func	*****************************************************************************************************
		//pin pb13  = servo_pmw
		//pin pb12 =  enable pmw
		PMC->PMC_PCER1 |= (1 << (ID_PWM-32)); //power - register 0
		
		PIOB->PIO_ABSR |= (PIO_ABSR_P12); //choosing B
		PIOB->PIO_ABSR |= (PIO_ABSR_P13); // OUTPUTING PWM SIGNAL
		
		PIOB->PIO_OER |= ( PIO_OER_P12); //output enable
		PIOB->PIO_OER |= ( PIO_OER_P13);
		PIOC->PIO_OER |= ( PIO_OER_P23);
		PIOC->PIO_OER |= ( PIO_OER_P23);


		PIOB->PIO_PDR |= ( PIO_PDR_P12); //sending through peripherals B //saying that were not using it as a standard but for a different purpose
		PIOB->PIO_PDR |= ( PIO_PDR_P13);
		
		//setting PMY registers
		//PWM_ENA
		//CPRD /number of ticks
		//duty cycle
		//enable, number of ticks not percentage's
		PWM->PWM_ENA |= PWM_ENA_CHID0;
		PWM->PWM_ENA |= PWM_ENA_CHID1;
		
		PWM->PWM_CH_NUM[0].PWM_CMR = 0b0110; //MSC // 64ms per tick
		PWM->PWM_CH_NUM[1].PWM_CMR = 0b0110; // scales the clock
		
		//Period Setto
		PWM->PWM_CH_NUM[0].PWM_CPRD= 0b100000; // setting period to 25 micro sec = frequemcy at 40 khz, taht comes by 32 ticks = 0b100000
		PWM->PWM_CH_NUM[1].PWM_CPRD= 0b110011010001010; // setting period 20ms; 26 250 desimal = 101001000001 binary ticks
		
		//high = ich 0.9 ms = MIN
		//PWM->PWM_CH_NUM[0].PWM_CDTY=0b10010011110; // 1 182 ticks setting duty cycle
		//PWM->PWM_CH_NUM[1].PWM_CDTY=0b10010011110; //need to put this into a function
		
		//high = ich 2.1 ms = MAX
		PWM->PWM_CH_NUM[0].PWM_CDTY=0; // 16 ticks =  0b10000 half speed ,
		PWM->PWM_CH_NUM[1].PWM_CDTY=0b101011000100; // 2 756 ticks setting duty cycle
		
		PWM->PWM_CH_NUM[0].PWM_CMR |= PWM_CMR_CPOL; //turning the clock polarity
		PWM->PWM_CH_NUM[1].PWM_CMR |= PWM_CMR_CPOL;
		//************************************************************************************************************************************************************************************************************

}


void encoder_init(){
			//Enable inputs todo Enabeling reading from the encoder
			PIOC->PIO_ABSR |= PIO_ABSR_P25; 
			PIOC->PIO_ABSR |= PIO_ABSR_P26;

			PIOC->PIO_OER |= PIO_OER_P25;  
			PIOC->PIO_OER |= PIO_OER_P26;
			
			PIOC->PIO_PER |= PIO_PER_P25;  
			PIOC->PIO_PER |=PIO_PER_P26;

			PIOC->PIO_PDR |= PIO_PDR_P25;  
			PIOC->PIO_PDR |= PIO_PDR_P26; 

			PMC->PMC_PCER1 |= (1 << (ID_TC6-32)); // Enabling clock for TC2, channel 27
			PMC->PMC_PCER0 |= (1 << (ID_TC2));
			
			PMC->PMC_PCR |= PMC_PCR_EN | 0b11101;
			
			TC2->TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_XC0;
			TC2->TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKEN | TC_CCR_SWTRG;
			TC2->TC_BMR |= TC_BMR_QDEN | TC_BMR_POSEN; 
}

void solinoid_init(){
	// Configure PB16 as output
	PMC->PMC_PCER0 |= (1 << ID_PIOB); //Enable Clock for PIOB
	PIOB->PIO_OER |=  PIO_OER_P16; // Set PB16 as output
	PIOB->PIO_PER |=  PIO_PER_P16; // Enable PIO control on PB16
	PIOB->PIO_CODR |= PIO_CODR_P16; //Initializaing to 0 Value	
	PWM->PWM_CH_NUM[0].PWM_CDTY=0b10000; //start on left side 
}

float get_scaled_encoder_pos(){
	int Encoder_Value = TC2->TC_CHANNEL[0].TC_CV;
	//printf("\r  Encoder Value: %d \r \n",  Encoder_Value);
	float scaled_encoder_value = ((float)Encoder_Value / 28.50); // 28 is scalar to get the encoder between 0 and 100
	return scaled_encoder_value; 
}

/*
int get_position_error(int encoder_pos, int joystick_pos){
	int error = encoder_pos - joystick_pos; //encoder is actual posotion and joystick is target
	return error;
}
*/

int position_controller_2(int scaled_encoder, int joystick_pos){
	float kp = 2; //between 0.1 and 1
	float T =  0.02 ; //200ms
	float ki = 0.5; //kp/100 ?
	float current_error = joystick_pos - scaled_encoder;
	float windup_treshold = 200;
	
	hist_error = current_error + hist_error;
	
	if (hist_error > windup_treshold){
		hist_error = windup_treshold;
	}
	else if (hist_error < -windup_treshold){
		hist_error =-windup_treshold;
	}

	float output = (kp * current_error) + (T * ki * hist_error); //Max can be 200 + 2 (When kp 2, T 0,02, ki 0.5)

	return output;
}

void speed_setting(float output){
	int output_scaled = output /6; // 110 / 4 is less than 32
	//printf("\r   scaled_controller_output: %d \r \n",  output_scaled);
	if (output_scaled > 32)
	{
		output_scaled = 32;
	}
}



int position_controller(int scaled_encoder, int joystick_pos){
	
	float kp = 2; //between 0.1 and 1
	float T =  0.02 ; //200ms
	float ki = 0.5; //kp/100 ?
	float current_error = 0;
	float windup_treshold = 200;

	//int windup_treshold = 200; // a bit less then max

	hist_error += current_error;

	current_error = joystick_pos - scaled_encoder;
	
	if (current_error < 0)
	{PIOC->PIO_SODR |= PIO_SODR_P23; //go left
	current_error = current_error*(-1); //make it pos
	}else{
		PIOC->PIO_CODR |= PIO_CODR_P23; //go right
	}
	
	if (hist_error >= windup_treshold) 
	{
		hist_error = windup_treshold;
	}
	
	float output = (kp * (float)current_error) + (T * ki * (float)hist_error); //max = 100 + 10 = 110, the max speed = 32 and min =  0
	//printf("\r   controller_output: %d \r \n",  output);
	
	
	int output_scaled = output /6; // 110 / 4 is less than 32
	//printf("\r   scaled_controller_output: %d \r \n",  output_scaled);
	
	if (output_scaled > 32)
	{
		output_scaled = 32;
	}
	
	//printf("historical error: %d\r\n", hist_error);
	//printf("current error: %f\r\n", current_error);

	//return output_scaled, hist_error; // has to be converted to bit
	return output_scaled; // has to be converted to bit
}

int check_windup(int historical_error){
	int windup_treshold = 254; // a bit less than max error
}

float  scale_joystick_y(int joystick_y){
	float prosent;
	if (joystick_y < 175 && joystick_y > 160){ //default position of joystick
		 prosent =  50.0;
	 }
		
	else if(joystick_y <= 160){ //left position
		 prosent = (float)joystick_y /(1.6*2.0);
	}
	else{
		int diff = joystick_y -175; //right position
		 prosent = 50.0 + (float)diff /(0.8*2.0);
	}
	//printf("\r scaled joystick pos: %f \r \n",  prosent);
	return prosent;
}

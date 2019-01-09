/*
 * Joshua Riley
 * j.riley.zero00@gmail.com
 * UCR - CS 122A
 * 
 * RC Car
 * Master Module
 * Controller
 *
 * Using Atmega1284
 */


#ifndef CONTROLLER_H_
#define CONTROLLER_H_

int throttle_input = 0x00;
int turn_input = 0x00;
unsigned char adc_translation = 0x00;
unsigned char is_automatic = 0x00;
extern unsigned char bt_data;

enum Joysticks_SM {Joysticks_Init, Joysticks_Get_Turn, Joysticks_Get_Throttle} joysticks_state;
enum Button_SM {Press1, Wait1, Press2, Wait2} button_state;

unsigned char Translate(int x){
	
	if (x < 500){ // Left or Down
		return 1;
	}
	else if (x < 600){ // Middle
		return 2;
	}
	else { // Right or Up
		return 3;
	}
	
}

void Joysticks_Tick(){
	if (is_automatic){return;}
	
	switch(joysticks_state){ // Transitions
		case Joysticks_Init:
			joysticks_state = Joysticks_Get_Turn;
			break;
		case Joysticks_Get_Turn:
			joysticks_state = Joysticks_Get_Throttle;
			break;
		case Joysticks_Get_Throttle:
			joysticks_state = Joysticks_Get_Turn;
			break;
		default:
			joysticks_state = Joysticks_Init;
			break;
	}
	switch(joysticks_state){ // Actions
		case Joysticks_Init:
			ADC_init();
			for(unsigned i = 0; i < 25; ++i){asm("nop");}
			bt_data = 0x00;
			break;
		case Joysticks_Get_Turn:
			ADMUX = 0x00;
			for(unsigned i = 0; i < 15; ++i){asm("nop");}
			turn_input = (int)ADC;
			adc_translation = Translate(turn_input);
			if (adc_translation == 1){
				bt_data = (bt_data & 0xFC) | 0x01;
			}
			else if (adc_translation == 2){
				bt_data = (bt_data & 0xFC) | 0x02;
			}
			else {
				bt_data = (bt_data & 0xFC) | 0x03;
			}
			break;
		case Joysticks_Get_Throttle:
			ADMUX = 0x01;
			for(unsigned i = 0; i < 15; ++i){asm("nop");}
			throttle_input = (int)ADC;
			adc_translation = Translate(throttle_input);
			if (adc_translation == 1){
				bt_data = (bt_data & 0xCF) | 0x10;
			}
			else if (adc_translation == 2){
				bt_data = (bt_data & 0xCF) | 0x20;
			}
			else {
				bt_data = (bt_data & 0xCF) | 0x30;
			}
			break;
		default:
			break;
	}
}

void Button_Tick(){
	unsigned char button_press = ~PINC & 0x01;
	switch(button_state){ // Transitions
		case Press1:
			if (button_press){
				button_state = Wait1;
			}
			else {
				button_state = Press1;
			}
			break;
		case Wait1:
			if (button_press){
				button_state = Wait1;
			}
			else {
				button_state = Press2;
			}
			break;
		case Press2:
			if (button_press){
				button_state = Wait2;
			}
			else {
				button_state = Press2;
			}
			break;
		case Wait2:
			if (button_press){
				button_state = Wait2;
			}
			else {
				button_state = Press1;
			}
			break;
		default:
			button_state = Wait2;
			break;
		
	} // Transitions
	
	switch(button_state){ // Actions
		case Press1:
			break;
		case Wait1:
			PORTB = 0x01;
			is_automatic = 1;
			break;
		case Press2:
			break;
		case Wait2:
			PORTB = 0x00;
			is_automatic = 0;
			break;
		default:
			break;
	} // Actions
}

#endif /* CONTROLLER_H_ */
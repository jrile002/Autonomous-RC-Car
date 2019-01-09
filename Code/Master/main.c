/*
 * Joshua Riley
 * j.riley.zero00@gmail.com
 * UCR - CS 122A
 * 
 * RC Car
 * Master Module
 *
 * Using Atmega1284
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "controller.h"
#include "bluetooth_master.h"
#include "timer.h"

// Master Module Pin-out
// * Rx = PD0
// * Tx = PD1

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00; // Arduino testing as well; just remove led wire
	DDRC = 0x00; PORTC = 0xFF;
	bt_data = 0x00;
	bt_slave_state = -1;
	joysticks_state = -1;
	button_state = -1;
	TimerSet(10);
	TimerOn();
	unsigned char counter = 0x00;
    while (1) 
    {
		BT_Master_Tick();
		Joysticks_Tick();
		if (counter >= 10){
			Button_Tick();
		}
		else{
			counter++;
		}
		while(!TimerFlag){}
		TimerFlag = 0;
    }
}



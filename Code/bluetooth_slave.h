/*
 * Joshua Riley
 * j.riley.zero00@gmail.com
 * UCR - CS 122A
 * 
 * RC Car
 * Servant Module
 * Bluetooth
 *
 * Using Atmega1284
 */
#ifndef BLUETOOTH_SLAVE_H_
#define BLUETOOTH_SLAVE_H_

#include "bit.h"
//#include "usart.h"
void initUSART(unsigned char usartNum)
{
	if (usartNum != 1) {
		UCSR0B |= (1 << RXEN0)  | (1 << TXEN0);
		UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
		UBRR0L = BAUD_PRESCALE;
		UBRR0H = (BAUD_PRESCALE >> 8);
	}
	else {
		UCSR1B |= (1 << RXEN1)  | (1 << TXEN1);
		UCSR1C |= (1 << UCSZ10) | (1 << UCSZ11);
		UBRR1L = BAUD_PRESCALE;
		UBRR1H = (BAUD_PRESCALE >> 8);
	}
}

unsigned char USART_HasReceived(unsigned char usartNum)
{
	return (usartNum != 1) ? (UCSR0A & (1 << RXC0)) : (UCSR1A & (1 << RXC1));
}

unsigned char bt_data = 0x00;
unsigned char manual_movement = 0x00;

enum BT_Slave_SM {BT_Slave_Init, BT_Slave_Check_Ready, BT_Slave_Receive} bt_slave_state;

void BT_Slave_Tick(){
	switch (bt_slave_state){ // Transitions
		case BT_Slave_Init:
		bt_slave_state = BT_Slave_Check_Ready;
		break;
		case BT_Slave_Check_Ready:
		if (USART_HasReceived(0)){
			bt_slave_state = BT_Slave_Receive;
		}
		else{
			bt_slave_state = BT_Slave_Check_Ready;
		}
		break;
		case BT_Slave_Receive:
		bt_slave_state = BT_Slave_Check_Ready;
		break;
		default:
		bt_slave_state = BT_Slave_Init;
		break;
	}
	
	switch (bt_slave_state){ // Actions
		case BT_Slave_Init:
		initUSART(0);
		break;
		case BT_Slave_Check_Ready:
		break;
		case BT_Slave_Receive:
		bt_data = UDR0;
		break;
	}
}

void BT_Slave_Tick_Task()
{
	bt_slave_state = -1;
	while(1)
	{
		BT_Slave_Tick();
		vTaskDelay(1);
	}
}



#endif /* BLUETOOTH_SLAVE_H_ */
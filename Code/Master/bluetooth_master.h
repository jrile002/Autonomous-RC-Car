/*
 * Joshua Riley
 * j.riley.zero00@gmail.com
 * UCR - CS 122A
 * 
 * RC Car
 * Master Module
 * Bluetooth
 *
 * Using Atmega1284
 */


#ifndef BLUETOOTH_MASTER_H_
#define BLUETOOTH_MASTER_H_

// USART Setup Values
#define F_CPU 8000000UL // Assume uC operates at 8MHz
#define BAUD_RATE 38400
#define BAUD_PRESCALE (((F_CPU / (BAUD_RATE * 16UL))) - 1)

unsigned char bt_data = 0x00;
extern unsigned char is_automatic;
enum BT_Master_SM {BT_Master_Init, BT_Master_Check_Ready, BT_Master_Send} bt_slave_state;

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

unsigned char USART_IsSendReady(unsigned char usartNum)
{
	return (usartNum != 1) ? (UCSR0A & (1 << UDRE0)) : (UCSR1A & (1 << UDRE1));
}

unsigned char USART_HasTransmitted(unsigned char usartNum)
{
	return (usartNum != 1) ? (UCSR0A & (1 << TXC0)) : (UCSR1A & (1 << TXC1));
}

unsigned char USART_HasReceived(unsigned char usartNum)
{
	return (usartNum != 1) ? (UCSR0A & (1 << RXC0)) : (UCSR1A & (1 << RXC1));
}

void USART_Flush(unsigned char usartNum)
{
	static unsigned char dummy;
	if (usartNum != 1) {
		while ( UCSR0A & (1 << RXC0) ) { dummy = UDR0; }
	}
	else {
		while ( UCSR1A & (1 << RXC1) ) { dummy = UDR1; }
	}
}

void USART_Send(unsigned char sendMe, unsigned char usartNum)
{
	if (usartNum != 1) {
		while( !(UCSR0A & (1 << UDRE0)) );
		UDR0 = sendMe;
	}
	else {
		while( !(UCSR1A & (1 << UDRE1)) );
		UDR1 = sendMe;
	}
}

unsigned char USART_Receive(unsigned char usartNum)
{
	if (usartNum != 1) {
		while ( !(UCSR0A & (1 << RXC0)) ); // Wait for data to be received
		return UDR0; // Get and return received data from buffer
	}
	else {
		while ( !(UCSR1A & (1 << RXC1)) );
		return UDR1;
	}
}


void BT_Master_Tick(){
	switch (bt_slave_state){ // Transitions
		case BT_Master_Init:
			bt_slave_state = BT_Master_Check_Ready;
			break;
		case BT_Master_Check_Ready:
			if (USART_IsSendReady(0)){
				bt_slave_state = BT_Master_Send;
			}
			else{
				bt_slave_state = BT_Master_Check_Ready;
			}
			break;
		case BT_Master_Send:
			bt_slave_state = BT_Master_Check_Ready;
			break;
		default:
			bt_slave_state = BT_Master_Init;
			break;
	}
	
	switch (bt_slave_state){ // Actions
		case BT_Master_Init:
			initUSART(0);
			break;
		case BT_Master_Check_Ready:
			break;
		case BT_Master_Send:
			if (is_automatic){
				bt_data = 0x80;
			}
			else {
				bt_data &= 0x7F;
			}
			USART_Send(bt_data, 0);
			break;
	}
}


#endif /* BLUETOOTH_MASTER_H_ */
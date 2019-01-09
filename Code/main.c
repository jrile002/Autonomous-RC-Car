/*
 * Joshua Riley
 * j.riley.zero00@gmail.com
 * UCR - CS 122A
 * 
 * RC Car
 * Servant Module
 *
 * Using Atmega1284
 */ 
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

// USART Setup Values
#define F_CPU 8000000UL // Assume uC operates at 8MHz
#define BAUD_RATE 38400
#define BAUD_PRESCALE (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#include <util/delay.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"

//Other include files
#include "bluetooth_slave.h"
#include "motor_driver.h"
#include "ultrasonic_sensor.h"

void CreateTasks(unsigned portBASE_TYPE Priority_BT, unsigned portBASE_TYPE Priority_USS, unsigned portBASE_TYPE Priority_Auto, unsigned portBASE_TYPE Priority_Man )
{
	xTaskCreate(BT_Slave_Tick_Task, (signed portCHAR *)"BT_Slave_Tick_Task", configMINIMAL_STACK_SIZE, NULL, Priority_BT, NULL);
	xTaskCreate(USS_Tick_Task, (signed portCHAR *)"USS_Tick_Task", configMINIMAL_STACK_SIZE, NULL, Priority_USS, NULL);
	xTaskCreate(Auto_Drive_Tick_Task, (signed portCHAR *)"Auto_Drive_Tick_Task", configMINIMAL_STACK_SIZE, NULL, Priority_Auto, NULL);
	xTaskCreate(Manual_Drive_Tick_Task, (signed portCHAR *)"Manual_Drive_Tick_Task", configMINIMAL_STACK_SIZE, NULL, Priority_Man, NULL);
}

int main(void)
{
    DDRA = 0xFF; PORTA = 0x00; // MTD-01
	DDRB = 0xFF; PORTB = 0x00; // DRV8333 + USS Trig
	DDRC = 0x00; PORTC = 0xFF; // USS Echo
	// DDRD = ; PORTD = ; // Bluetooth Module
	
	CreateTasks(2, 1, 1, 1);
	vTaskStartScheduler();
	return 0;
}


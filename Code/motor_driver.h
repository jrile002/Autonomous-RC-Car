/*
 * Joshua Riley
 * j.riley.zero00@gmail.com
 * UCR - CS 122A
 * 
 * RC Car
 * Servant Module
 * Motor Drivers
 * 
 * Using Atmega1284
 */
#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

#include "bit.h"

// Bluetooth data
extern unsigned char bt_data;
extern unsigned char manual_movement;

// Automatic
enum Auto_D_Signal {AUTO_STOP, AUTO_FORWARD, AUTO_BACKWARD, AUTO_LEFT, AUTO_RIGHT} auto_drive_signal;

// Manual
#define FORWARD_LEFT 0x31
#define FORWARD 0x32
#define FORWARD_RIGHT 0x33
#define BACKWARD_LEFT 0x11
#define BACKWARD 0x12
#define BACKWARD_RIGHT 0x13

// MTD-01 Motor Driver Port
#define MDA_PORT PORTA
#define MDB_PORT PORTA
// DRV8833 Motor Driver Port
#define MDC_PORT PORTB
#define MDD_PORT PORTB

// MTD-01 Pin-out for DC Motor 1
#define MDA_en 0 // PA0
#define MDA_in1 1 // PA1
#define MDA_in2 2 // PA2
// MTD-01 Pin-out for DC Motor 2
#define MDB_en 3 // PA3
#define MDB_in1 4 // PA4 (in3)
#define MDB_in2 5 // PA5 (in4)

// DRV8833 Pin-out for Enable Signal
#define MDcd_en 0 // PB0 (Slp)
// DRV8833 Pin-out for DC Motor 3
#define MDC_in1 1 // PB1 (Ain1)
#define MDC_in2 2 // PB2 (Ain2)
// DRV8833 Pin-out for DC Motor 4
#define MDD_in1 3 // PB3 (Bin1)
#define MDD_in2 4 // PB4 (Bin2)

enum Manual_Drive_SM {D_Stop, D_Forward_Left, D_Forward, D_Forward_Right, D_Backward_Left, D_Backward, D_Backward_Right} manual_drive_state;
enum Auto_Drive_SM {Auto_D_Stop, Auto_D_Forward, Auto_D_Backward, Auto_D_Left, Auto_D_Right} auto_drive_state;
	
void Manual_Drive_Tick(){
	manual_movement = bt_data & 0x7F;
	switch (manual_movement){ // Optimized Transitions
		case FORWARD_LEFT:
		manual_drive_state = D_Forward_Left;
		break;
		case FORWARD:
		manual_drive_state = D_Forward;
		break;
		case FORWARD_RIGHT:
		manual_drive_state = D_Forward_Right;
		break;
		case BACKWARD_LEFT:
		manual_drive_state = D_Backward_Left;
		break;
		case BACKWARD:
		manual_drive_state = D_Backward;
		break;
		case BACKWARD_RIGHT:
		manual_drive_state = D_Backward_Right;
		break;
		default:
		manual_drive_state = D_Stop;
		break;
	} // Optimized Transitions

	switch (manual_drive_state){ // Actions
		case D_Stop: // No movement
		MDA_PORT = SetBit(MDA_PORT, MDA_en, 0); // ---Motor A---
		MDB_PORT = SetBit(MDB_PORT, MDB_en, 0); // ---Motor B---
		MDC_PORT = SetBit(MDC_PORT, MDcd_en, 0); // ---Motor C and D ---
		break;
		case D_Forward_Left:
		// ---Motor A---
		MDA_PORT = SetBit(MDA_PORT, MDA_en, 0);
		// ---Motor B---
		MDB_PORT = SetBit(MDB_PORT, MDB_en, 1);
		MDB_PORT = SetBit(MDB_PORT, MDB_in1, 1);
		MDB_PORT = SetBit(MDB_PORT, MDB_in2, 0);
		// Enables Motors C and D
		MDC_PORT = SetBit(MDC_PORT, MDcd_en, 1);
		// ---Motor C---
		MDC_PORT = SetBit(MDC_PORT, MDC_in1, 0);
		MDC_PORT = SetBit(MDC_PORT, MDC_in2, 0);
		// ---Motor D---
		MDD_PORT = SetBit(MDD_PORT, MDD_in1, 1);
		MDD_PORT = SetBit(MDD_PORT, MDD_in2, 0);
		break;
		case D_Forward:
		// ---Motor A---
		MDA_PORT = SetBit(MDA_PORT, MDA_en, 1);
		MDA_PORT = SetBit(MDA_PORT, MDA_in1, 1);
		MDA_PORT = SetBit(MDA_PORT, MDA_in2, 0);
		// ---Motor B---
		MDB_PORT = SetBit(MDB_PORT, MDB_en, 1);
		MDB_PORT = SetBit(MDB_PORT, MDB_in1, 1);
		MDB_PORT = SetBit(MDB_PORT, MDB_in2, 0);
		// Enables Motors C and D
		MDC_PORT = SetBit(MDC_PORT, MDcd_en, 1);
		// ---Motor C---
		MDC_PORT = SetBit(MDC_PORT, MDC_in1, 1);
		MDC_PORT = SetBit(MDC_PORT, MDC_in2, 0);
		// ---Motor D---
		MDD_PORT = SetBit(MDD_PORT, MDD_in1, 1);
		MDD_PORT = SetBit(MDD_PORT, MDD_in2, 0);
		break;
		case D_Forward_Right:
		// ---Motor A---
		MDA_PORT = SetBit(MDA_PORT, MDA_en, 1);
		MDA_PORT = SetBit(MDA_PORT, MDA_in1, 1);
		MDA_PORT = SetBit(MDA_PORT, MDA_in2, 0);
		// ---Motor B---
		MDB_PORT = SetBit(MDB_PORT, MDB_en, 0);
		// Enables Motors C and D
		MDC_PORT = SetBit(MDC_PORT, MDcd_en, 1);
		// ---Motor C---
		MDC_PORT = SetBit(MDC_PORT, MDC_in1, 1);
		MDC_PORT = SetBit(MDC_PORT, MDC_in2, 0);
		// ---Motor D---
		MDD_PORT = SetBit(MDD_PORT, MDD_in1, 0);
		MDD_PORT = SetBit(MDD_PORT, MDD_in2, 0);
		break;
		case D_Backward_Left:
		// ---Motor A---
		MDA_PORT = SetBit(MDA_PORT, MDA_en, 0);
		// ---Motor B---
		MDB_PORT = SetBit(MDB_PORT, MDB_en, 1);
		MDB_PORT = SetBit(MDB_PORT, MDB_in1, 0);
		MDB_PORT = SetBit(MDB_PORT, MDB_in2, 1);
		// Enables Motors C and D
		MDC_PORT = SetBit(MDC_PORT, MDcd_en, 1);
		// ---Motor C---
		MDC_PORT = SetBit(MDC_PORT, MDC_in1, 0);
		MDC_PORT = SetBit(MDC_PORT, MDC_in2, 0);
		// ---Motor D---
		MDD_PORT = SetBit(MDD_PORT, MDD_in1, 0);
		MDD_PORT = SetBit(MDD_PORT, MDD_in2, 1);
		break;
		case D_Backward:
		// ---Motor A---
		MDA_PORT = SetBit(MDA_PORT, MDA_en, 1);
		MDA_PORT = SetBit(MDA_PORT, MDA_in1, 0);
		MDA_PORT = SetBit(MDA_PORT, MDA_in2, 1);
		// ---Motor B---
		MDB_PORT = SetBit(MDB_PORT, MDB_en, 1);
		MDB_PORT = SetBit(MDB_PORT, MDB_in1, 0);
		MDB_PORT = SetBit(MDB_PORT, MDB_in2, 1);
		// Enables Motors C and D
		MDC_PORT = SetBit(MDC_PORT, MDcd_en, 1);
		// ---Motor C---
		MDC_PORT = SetBit(MDC_PORT, MDC_in1, 0);
		MDC_PORT = SetBit(MDC_PORT, MDC_in2, 1);
		// ---Motor D---
		MDD_PORT = SetBit(MDD_PORT, MDD_in1, 0);
		MDD_PORT = SetBit(MDD_PORT, MDD_in2, 1);
		break;
		case D_Backward_Right:
		// ---Motor A---
		MDA_PORT = SetBit(MDA_PORT, MDA_en, 1);
		MDA_PORT = SetBit(MDA_PORT, MDA_in1, 0);
		MDA_PORT = SetBit(MDA_PORT, MDA_in2, 1);
		// ---Motor B---
		MDB_PORT = SetBit(MDB_PORT, MDB_en, 0);
		// Enables Motors C and D
		MDC_PORT = SetBit(MDC_PORT, MDcd_en, 1);
		// ---Motor C---
		MDC_PORT = SetBit(MDC_PORT, MDC_in1, 0);
		MDC_PORT = SetBit(MDC_PORT, MDC_in2, 1);
		// ---Motor D---
		MDD_PORT = SetBit(MDD_PORT, MDD_in1, 0);
		MDD_PORT = SetBit(MDD_PORT, MDD_in2, 0);
		break;
		default:
		break;
	} // Actions
	
}

void Auto_Drive_Tick(){
	switch (auto_drive_signal){ // Optimized Transitions
		case AUTO_FORWARD:
		auto_drive_state = Auto_D_Forward;
		break;
		case AUTO_BACKWARD:
		auto_drive_state = Auto_D_Backward;
		break;
		case AUTO_LEFT:
		auto_drive_state = Auto_D_Left;
		break;
		case AUTO_RIGHT:
		auto_drive_state = Auto_D_Right;
		break;
		default:
		auto_drive_state = Auto_D_Stop;
		break;

	} // Optimized Transitions
	
	switch(auto_drive_state){ // Actions
		case Auto_D_Stop:	// Turn off all motors
		MDA_PORT = SetBit(MDA_PORT, MDA_en, 0); // ---Motor A---
		MDB_PORT = SetBit(MDB_PORT, MDB_en, 0); // ---Motor B---
		MDC_PORT = SetBit(MDC_PORT, MDcd_en, 0); // ---Motor C and D ---
		break;
		case Auto_D_Forward:	// Power all 4 motors
		// ---Motor A---
		MDA_PORT = SetBit(MDA_PORT, MDA_en, 1);
		MDA_PORT = SetBit(MDA_PORT, MDA_in1, 1);
		MDA_PORT = SetBit(MDA_PORT, MDA_in2, 0);
		// ---Motor B---
		MDB_PORT = SetBit(MDB_PORT, MDB_en, 1);
		MDB_PORT = SetBit(MDB_PORT, MDB_in1, 1);
		MDB_PORT = SetBit(MDB_PORT, MDB_in2, 0);
		// Enables Motors C and D
		MDC_PORT = SetBit(MDC_PORT, MDcd_en, 1);
		// ---Motor C---
		MDC_PORT = SetBit(MDC_PORT, MDC_in1, 1);
		MDC_PORT = SetBit(MDC_PORT, MDC_in2, 0);
		// ---Motor D---
		MDD_PORT = SetBit(MDD_PORT, MDD_in1, 1);
		MDD_PORT = SetBit(MDD_PORT, MDD_in2, 0);
		break;
		case Auto_D_Backward: // Power all 4 motors in REVERSE
		// ---Motor A---
		MDA_PORT = SetBit(MDA_PORT, MDA_en, 1);
		MDA_PORT = SetBit(MDA_PORT, MDA_in1, 0);
		MDA_PORT = SetBit(MDA_PORT, MDA_in2, 1);
		// ---Motor B---
		MDB_PORT = SetBit(MDB_PORT, MDB_en, 1);
		MDB_PORT = SetBit(MDB_PORT, MDB_in1, 0);
		MDB_PORT = SetBit(MDB_PORT, MDB_in2, 1);
		
		// Enables Motors C and D
		MDC_PORT = SetBit(MDC_PORT, MDcd_en, 1);
		// ---Motor C---
		MDC_PORT = SetBit(MDC_PORT, MDC_in1, 0);
		MDC_PORT = SetBit(MDC_PORT, MDC_in2, 1);
		// ---Motor D---
		MDD_PORT = SetBit(MDD_PORT, MDD_in1, 0);
		MDD_PORT = SetBit(MDD_PORT, MDD_in2, 1);
		break;
		case Auto_D_Left: // Power ONLY rightmost motors forward
		// ---Motor A---
		MDA_PORT = SetBit(MDA_PORT, MDA_en, 0);
		// ---Motor B---
		MDB_PORT = SetBit(MDB_PORT, MDB_en, 1);
		MDB_PORT = SetBit(MDB_PORT, MDB_in1, 1);
		MDB_PORT = SetBit(MDB_PORT, MDB_in2, 0);
		// Enables Motors C and D
		MDC_PORT = SetBit(MDC_PORT, MDcd_en, 1);
		// ---Motor C---
		MDC_PORT = SetBit(MDC_PORT, MDC_in1, 0);
		MDC_PORT = SetBit(MDC_PORT, MDC_in2, 0);
		// ---Motor D---
		MDD_PORT = SetBit(MDD_PORT, MDD_in1, 1);
		MDD_PORT = SetBit(MDD_PORT, MDD_in2, 0);

		break;
		case Auto_D_Right: // Power ONLY leftmost motors
		// ---Motor A---
		MDA_PORT = SetBit(MDA_PORT, MDA_en, 1);
		MDA_PORT = SetBit(MDA_PORT, MDA_in1, 1);
		MDA_PORT = SetBit(MDA_PORT, MDA_in2, 0);
		// ---Motor B---
		MDB_PORT = SetBit(MDB_PORT, MDB_en, 0);
		// Enables Motors C and D
		MDC_PORT = SetBit(MDC_PORT, MDcd_en, 1);
		// ---Motor C---
		MDC_PORT = SetBit(MDC_PORT, MDC_in1, 1);
		MDC_PORT = SetBit(MDC_PORT, MDC_in2, 0);
		// ---Motor D---
		MDD_PORT = SetBit(MDD_PORT, MDD_in1, 0);
		MDD_PORT = SetBit(MDD_PORT, MDD_in2, 0);
		break;
		default:
		break;
	} // Actions
}

void Manual_Drive_Tick_Task(){
	manual_drive_state = -1;
	while(1)
	{
		if ((bt_data & 0x80) == 0){
			Manual_Drive_Tick();
		}
		vTaskDelay(500);
	}
}

void Auto_Drive_Tick_Task()
{
	auto_drive_state = -1;
	while(1)
	{
		if ((bt_data & 0x80) != 0){
			Auto_Drive_Tick();
		}
		vTaskDelay(500);
	}
}

#endif
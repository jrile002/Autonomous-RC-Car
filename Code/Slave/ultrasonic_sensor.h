/*
 * Joshua Riley
 * j.riley.zero00@gmail.com
 * UCR - CS 122A
 * 
 * RC Car Project
 * Servant Module
 * Ultrasonic Sensor
 *
 * Using Atmega1284
 */


#ifndef ULTRASONIC_SENSOR_H_
#define ULTRASONIC_SENSOR_H_

#define USS_TRIG_PORT PORTB
#define USS_ECHO_PORT PINC
#define TRIG_PIN 5 // Trig is PB5 => OUTPUT
#define ECHO_PIN 0x01 // Echo is PC0 => INPUT

const unsigned long USS_TIMEOUT = 3000; // 3 second timeout
unsigned long duration = 0; // Pulse duration in ms
unsigned int distance = 0; // Object distance in cm

enum USS_SM {USS_TriggerPulse, USS_WaitForHigh, USS_WaitForLow} USS_state;

unsigned int millisecondsToCentimeters(unsigned long milliseconds){ // For USS distance calculation
	return (unsigned int)(milliseconds / 2.0 / 29.0 * 1000 );
}

void USS_Tick(){
	
	switch (USS_state){ // Transitions
		case USS_TriggerPulse:
		USS_state = USS_WaitForHigh;
		break;
		case USS_WaitForHigh:
		if ( (USS_ECHO_PORT & ECHO_PIN) != 0){
			// Echo Pin is High
			duration = 0;
			USS_state = USS_WaitForLow;
		}
		else {
			//Retry pulse
			USS_state = USS_TriggerPulse;
		}
		break;
		case USS_WaitForLow:
		if ( (USS_ECHO_PORT & ECHO_PIN) != 0 && duration < USS_TIMEOUT){
			// Echo Pin is high and the pulse hasn't timed out
			USS_state = USS_WaitForLow;
		}
		else {
			if (duration >= USS_TIMEOUT){ // Timed out
				auto_drive_signal = AUTO_FORWARD;
			}
			else{ // Calculate distance and display on LED
				distance = millisecondsToCentimeters(duration);
				if (distance > 100){
					auto_drive_signal = AUTO_FORWARD;
				}
				else if (distance > 50){
					auto_drive_signal = AUTO_RIGHT;
				}
				else{
					auto_drive_signal = AUTO_BACKWARD;
				}
			}
			duration = 0;
			USS_state = USS_TriggerPulse;
		}
		break;
		default:
		USS_state = USS_TriggerPulse;
		auto_drive_signal = AUTO_STOP;
		break;
		
	} // Transitions
	
	switch (USS_state){ // Actions
		case USS_TriggerPulse:
		USS_TRIG_PORT = SetBit(USS_TRIG_PORT, TRIG_PIN, 1);
		_delay_us(10);
		USS_TRIG_PORT = SetBit(USS_TRIG_PORT, TRIG_PIN, 0);
		break;
		case USS_WaitForHigh:
		break;
		case USS_WaitForLow:
		++duration;
		break;
		default:
		break;
		
	} // Actions
}

void USS_Tick_Task()
{
	USS_state = -1;
	while(1)
	{
		if ((bt_data & 0x80) != 0){
			USS_Tick();
		}
		vTaskDelay(1);
	}
}




#endif /* ULTRASONIC_SENSOR_H_ */
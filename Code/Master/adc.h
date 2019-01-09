////////////////////////////////////////////////////////////////////////////////

// Permission to copy is granted provided that this header remains intact.
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

#ifndef __ADC_H__
#define __ADC_H__

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}
#endif
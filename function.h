/*
 * function.h
 *
 *  Created on: 11 juin 2020
 *      Author: Kaze
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include "hal_port.h"

inline void function_init(void){

	pin_direction(&DDRB,pin0,output);
	pin_direction(&DDRB,pin1,output);
	pin_direction(&DDRB,pin2,output);
	pin_direction(&DDRB,pin3,output);
	pin_direction(&DDRD,pin6,output); /// gsm request pin

	pin_write(&PORTB,pin0,low);
	pin_write(&PORTB,pin1,low);
	pin_write(&PORTB,pin2,low);
	pin_write(&PORTB,pin3,low);
	pin_write(&PORTD,pin6,high);/// gsm request pin



}


inline void function_write(unsigned char _data){

	PORTB &= 0xF0;

	PORTB |= (0x0F & _data);

}


void gsm_request(void);


#endif /* FUNCTION_H_ */

/*
 * function.c
 *
 *  Created on: 11 juin 2020
 *      Author: Kaze
 */
#include "function.h"

void gsm_request(void){

	pin_write(&PORTD,pin6,low);
	_delay_ms(500);

	pin_write(&PORTD,pin6,high);
	_delay_ms(500);
}


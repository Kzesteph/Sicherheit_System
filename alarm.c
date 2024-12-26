/*
 * alarm.c
 *
 *  Created on: 25 mai 2020
 *      Author: michel_granda
 */

#include "alarm.h"

void buzzer_bipper(unsigned char number){

	unsigned char i=0;
	
	
	for(i=0; i<number; i++){
		
		alarm_set();
		_delay_ms(500);
		
		alarm_reset();
		_delay_ms(100);
		
	}
	
	
	
	
}



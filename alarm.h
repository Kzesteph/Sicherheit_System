/*
 * alarm.h
 *
 *  Created on: 25 mai 2020
 *      Author: michel_granda
 */

#ifndef ALARM_H_
#define ALARM_H_

#include "hal_port.h"


/**
 *
 */
inline void alarm_init(void){

	pin_direction(&DDRC,pin3,output);
	pin_direction(&DDRC,pin2,input);

	pin_write(&PORTC,pin3,low);
	pin_write(&PORTC,pin2,high);///active pull up
}

/**
 *
 */
inline unsigned char alarm_set(void){

	pin_write(&PORTC,pin3,high);
	//_delay_ms(5);

	return 1;
}

/**
 *
 */
inline unsigned char alarm_reset(void){

	pin_write(&PORTC,pin3,low);
	//_delay_ms(5);

	return 1;
}


/**
 *
 */
inline unsigned char alarm_toggle(void){

	pin_toggle(&PORTC,pin3);
		//_delay_ms(5);

	return 1;
}

/**
 *
 * @return
 */
inline unsigned char alarm_get_state(void){

	

	if(PINC & (1 << pin2)){

		return 0;
	}
	else{

			return 1;
	}

}


inline void alarm_restaure_state(void){

uint8_t alarm_adresse = 0x00;

	switch( eeprom_read_byte( (const uint8_t*)alarm_adresse) ){

		case 0:
		alarm_reset();
		break;

		case 1:
		alarm_set();

		break;
	}

}

inline void alarm_store_state(void){

uint8_t alarm_adresse = 0x00;

	eeprom_write_byte((uint8_t *)alarm_adresse, alarm_get_state()  );

}



void buzzer_bipper(unsigned char number);

#endif /* ALARM_H_ */

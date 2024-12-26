/*
 * bougie.h
 *
 *  Created on: 25 mai 2020
 *      Author: michel_granda
 */

#ifndef BOUGIE_H_
#define BOUGIE_H_

#include "hal_port.h"



/**
 * ce fonction initialise les broches qui servent pour la fonction bougie
 */
inline void bougie_init(void){

	pin_direction(&DDRC,pin1,output);
	pin_direction(&DDRC,pin0,input);

	pin_write(&PORTC,pin1,low);
	pin_write(&PORTC,pin0,high);///active pull up
}

/**
 * cette fonction permet d'ouvrir le contact de la bougie
 *
 */
inline unsigned char bougie_open(void){

	pin_write(&PORTC,pin1,low);
	//_delay_ms(5);
	return 1;
}


/**
 *
 */
inline unsigned char bougie_close(void){

	pin_write(&PORTC,pin1,high);
	//_delay_ms(5);
	return 1;
}

/**
 *
 * @return
 */
inline unsigned char bougie_get_state(void){

	

	if(PINC & (1 << pin0)){

		return 0;
	}
	else{

		return 1;
	}
}

inline void bougie_restaure_state(void){

uint8_t bougie_adresse = 0x01;

		switch( eeprom_read_byte((const uint8_t*)bougie_adresse) ){

			case 0:
			bougie_open();
			break;

			case 1:
			bougie_close();
			break;
		}
}


inline void bougie_store_state(void){

uint8_t bougie_adresse = 0x01;

	eeprom_write_byte( (uint8_t *)0x01, bougie_get_state() );

}
#endif /* BOUGIE_H_ */

/*
 * hal_port.h
 *
 *  Created on: 25 mai 2020
 *      Author: Kaze
 */

#ifndef HAL_PORT_H_
#define HAL_PORT_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>


typedef enum{

	pin0=0,
	pin1,
	pin2,
	pin3,
	pin4,
	pin5,
	pin6,
	pin7

}pin;

typedef enum{

	input=0,
	output

} direction;

typedef enum{

	low=0,
	high

} state;

typedef enum{

	disable=0,
	enable

}activation;

typedef enum{
	pull_up_disable = 0,
	pull_up_enable

}pull_up;

//********************************* operation sur les broches **************************************************

inline unsigned char pin_direction(volatile uint8_t *port, pin pin_number, direction pin_direction){

	if (pin_direction == output){

		*port |= (1 << pin_number); // on met la broche correspondante en sortie

	}
		else{

			*port &= ~(1 << pin_number); // au cas contraire la met en entree
		}

	return 1;
}


inline unsigned char pin_write(volatile uint8_t *port, pin pin_number, state pin_state){

	if ( pin_state == high){

		*port |= (1 << pin_number); // on met la broche a l'etat haut
	}
	else{

		*port &= ~(1 << pin_number); // au cas contraire la met a l'etat bas
	}

	return 1;
}


inline unsigned char pin_toggle(volatile uint8_t *port, pin pin_number){

	*port ^= (1 << pin_number);

	return 1;
}


inline unsigned char pin_read(volatile uint8_t *port, pin pin_number){

	if ( *port & (1 << pin_number) ){

		return 0x01;
	}
	else{

		return 0x00;
	}

}


//****************************************operation sur les ports ***********************************************

inline unsigned char port_write(volatile uint8_t *port, unsigned char data){

	*port = (unsigned char) data;

	return 1;
}




inline unsigned char port_read(volatile uint8_t *port){


		return *(port);



}


inline void flush_buffer(unsigned char *_buf, unsigned int len){

	unsigned int i = 0;

	for(i=0; i<len; i++){

		_buf[i] = 0;
	}
}

#endif /* HAL_PORT_H_ */

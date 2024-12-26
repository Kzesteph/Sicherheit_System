/*
 * gsm_module.h
 *
 *  Created on: 27 mai 2020
 *      Author: michel_granda
 */

#ifndef GSM_MODULE_H_
#define GSM_MODULE_H_

#include "hal_usart.h"
#include "hal_port.h"



typedef enum{

	disable_state =0,
	enable_state

}etat;

typedef enum{

	clear = 0,
	set

}action;


typedef enum{

	low_state = 0,
	pin_change_state,
	falling,
	rising

}transition;


typedef enum{

	set_alarme = 1,
	reset_alarme,
	ok,
	read_alarme_status,

	set_bougie,
	reset_bougie,
	pulse_bougie,
	read_bougie_status,

	get_battery_voltage,
	get_position_gps,

	get_all,
	set_all,
	reset_all,

	enable_system,
	disable_system



}code_function;


/**
 *
 * @param _state
 */
inline void external_interrupt_enable(etat _state, transition _transition){

	EICRA &= ~(0x03);
	EICRA |= _transition;

	if(_state == enable_state){

		EIMSK |= (1 << INT0);

	}
	else{

		EIMSK &= ~(1 << INT0);
	}


}

/**
 *
 * @param _action
 */
inline void external_interrupt_flag(action _action){


	if(_action == clear){

		EIFR &= ~(1 << INTF0);
	}
	else{

		EIFR |= (1 << INTF0);

	}

}

/**
 *
 */
inline void function_init(void){

	pin_direction(&DDRD,pin3,input);
	pin_direction(&DDRD,pin4,input);
	pin_direction(&DDRD,pin5,input);
	pin_direction(&DDRD,pin6,input);


	///activation de pull up
	pin_write(&PORTD,pin3,high);
	pin_write(&PORTD,pin4,high);
	pin_write(&PORTD,pin5,high);
	pin_write(&PORTD,pin6,high);

}



/**
 *
 */
inline void gsm_init(void){

	pin_direction(&DDRD,pin2,input); ///on configure PD2 en entree
	pin_write(&PORTD,pin2,high); /// on active le pull up de la broche INT0

	function_init();
	external_interrupt_enable(enable_state, rising);


}

inline unsigned char function_read(void){

unsigned char buf = 0;

	buf = (unsigned char)(PIND >> 0x03);

	buf &= 0x0F;

	return buf;


}


inline void copy_char(unsigned char *_tab_recv, unsigned char *_tab_send, unsigned char len){

unsigned char i =0;

	for(i=0; i<len; i++){
	
		_tab_recv[i] = _tab_send[i];
	}

}


inline unsigned char crc8(unsigned char *_tab, unsigned char len){
	
	unsigned char checksum_cal = 0;
	unsigned char i = 0;

	checksum_cal =0;

	for(i=0; _tab[i] != '*'; i++ ){

		checksum_cal ^= _tab[i+1];
	}


	return (unsigned char)checksum_cal;
	
	
}


#endif /* GSM_MODULE_H_ */

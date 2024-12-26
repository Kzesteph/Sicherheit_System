/*
 * hal_spi.h
 *
 *  Created on: 4 juin 2020
 *      Author: Kaze
 */

#ifndef HAL_SPI_H_
#define HAL_SPI_H_

#include "hal_port.h"

#define DI		5
#define DO		6
#define SCK		7

typedef enum{

	FOSC_DIV_4=0,
	FOSC_DIV_16,
	FOSC_DIV_64,
	FOSC_DIV_128,
	FOSC_DIV_2,
	FOSC_DIV_8,
	FOSC_DIV_32,

}freq_div;



inline void SPI_init(void){

	pin_direction(&DDRB,DO,output);
	pin_direction(&DDRB,DI,input);
	pin_direction(&DDRB,SCK,input);
	
	USICR = 0;
	USICR = (1<<USIOIE) | (1<<USIWM0) | (1<<USICS1);
	USISR = (1<<USIOIF);

}


inline unsigned char SPI_write(unsigned char _data){

unsigned char x =0;

	USIDR = _data;
	USISR = (1<<USIOIF);
	while( !(USISR & (1 << USIOIF) ) );
	
	x = USIDR;

	return x;


}


#endif /* HAL_SPI_H_ */

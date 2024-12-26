/*
 * i2c.c
 *
 *  Created on: 2 juin 2020
 *      Author: michel_granda
 */

#include "spi.h"
#include "hal_usart.h"

void SPI_write_buffer(unsigned char *_tab, unsigned char len){

	unsigned char i = 0;
	unsigned char recv;

	for(i=0; i<len; i++){

		recv = SPI_write(_tab[i]);
		_delay_ms(500);
		
	}

}

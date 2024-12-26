/*
 * spi.h
 *
 *  Created on: 3 juin 2020
 *      Author: michel_granda
 */

#ifndef SPI_H_
#define SPI_H_

#include "hal_port.h"

#define MOSI	3
#define MISO	4
#define SCK		5
#define SS		2

typedef enum{

	FOSC_DIV_4=0,
	FOSC_DIV_16,
	FOSC_DIV_64,
	FOSC_DIV_128,
	FOSC_DIV_2,
	FOSC_DIV_8,
	FOSC_DIV_32,

}freq_div;


typedef enum{

	MSB_FIRST = 0,
	LSB_FIRST,

}order;

typedef enum{

	slave_mode = 0,
	master_mode
}mode;


inline void SPI_init(freq_div _freq, mode _mode, order _order){

	if(_mode == master_mode){

		pin_direction(&DDRB, MOSI, output);
		pin_direction(&DDRB, MISO, input );
		pin_direction(&DDRB, SCK,  output);
		pin_direction(&DDRB, SS,   output);

	}
	else{

		pin_direction(&DDRB, MOSI, input);
		pin_direction(&DDRB, MISO, output );
		pin_direction(&DDRB, SCK,  input);
		pin_direction(&DDRB, SS,   input);
	}


	pin_write(&SPCR,DORD,_order);  ///on choisit l'ordre MSB ou LSB
	SPCR |= (0x03 & _freq);
	pin_write(&SPSR,SPI2X, (_freq >> 2) );

	pin_write(&SPCR,MSTR,_mode);   ///on choisit soit master ou slave
	pin_write(&SPCR,SPE,high); /// on active le spi


/*
DDRB |=  (1 << 2) | (1 << 3) | (1 << 5); //mosi ,sck et ss en sortie
DDRB &= ~(1 << 4); //miso en entree;

SPCR = (1<<SPE)|(1<<MSTR); //spi frequency as 4Mhz
*/

}

inline unsigned char SPI_write(unsigned char _data){


	/*
	port_write(&SPDR,_data);

	
	while( !pin_read(&SPSR, SPIF) );*/

	SPDR = _data;
	while(!(SPSR & (1<<SPIF)));
	
	return SPDR;
}


void SPI_write_buffer(unsigned char *_tab, unsigned char len);

#endif /* SPI_H_ */

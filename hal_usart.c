/*
 * hal_usart.c
 *
 *  Created on: 25 mai 2020
 *      Author: Kaze
 */


#include "hal_usart.h"


/**
 *
 * @param _bauderate
 */
void usart_init(bauderate _bauderate){

	pin_direction(&DDRD,pin0,input); /// on configure la broche 0 en entree -> RX
	pin_direction(&DDRD,pin1,output);/// on configure la broche 1 en sortie -> TX
	/* Set baud rate */
	UBRRH = (unsigned char)(_bauderate>>8);
	UBRRL = (unsigned char)_bauderate;
	UCSRA = (1 << U2X);

	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (3<<UCSZ0);


}


/**
 *
 * @param _data
 */

void usart_write(unsigned char _data){

	/* Wait for empty transmit buffer */
	while ( !pin_read(&UCSRA,UDRE) );

	/* Put data into buffer, sends the data */
	port_write(&UDR,_data);

}

/**
 *
 * @param text
 */
void usart_write_text(unsigned char *text, unsigned int len){

	unsigned int i=0;

	switch(len){

	case 0:

		for(i=0; text[i] != '\0'; i++){

			usart_write( text[i] );
		}

		break;

	default:

		for(i=0; i < len; i++){

			usart_write( text[i] );
		}

		break;
	}

}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void usart_write_text_const(volatile const unsigned char *text, unsigned int len){

	unsigned int i=0;

	switch(len){		//pgm_read_byte( &text[i] )

		case 0:

		for(i=0; pgm_read_byte( &text[i]) != '\0'; i++){

			usart_write( pgm_read_byte( &text[i]) );
			
		}

		break;

		default:

		for(i=0; i < len; i++){

			usart_write( pgm_read_byte( &text[i]) );
		}

		break;
	}

}

/**
 *
 * @return
 */
unsigned char usart_data_ready(void){

	 if( pin_read(&UCSRA,RXC) ){

		 return 1;
	 }
	 else{

		 return 0;
	 }

}

/**
 *
 * @return
 */
unsigned char usart_data_read(void){

	/* Wait for data to be received */
	while ( !pin_read(&UCSRA,RXC) );

	/* Get and return received data from buffer */
	return port_read(&UDR);

}




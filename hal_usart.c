/*
 * hal_usart.c
 *
 *  Created on: 25 mai 2020
 *      Author: michel_granda
 */


#include "hal_usart.h"


/**
 *
 * @param _bauderate
 */
void usart_init(bauderate _bauderate){

	pin_direction(&DDRD,pin0,input); /// on configure la broche 0 en entree -> RX
	pin_direction(&DDRD,pin1,output);/// on configure la broche 1 en sortie -> TX

	//if(F_CPU == 16000000){

		pin_write(&UCSR0A,U2X0,high); //activation pour le double de la frequence du module USART

		port_write(&UBRR0H, (_bauderate >> 8 ) );

		port_write(&UBRR0L, (_bauderate & 0x00FF) );

		pin_write(&UCSR0B,RXEN0,high); /// activation du  buffer de reception de USART
		pin_write(&UCSR0B,TXEN0,high); /// activation du buffer de transmission de l'USART

		pin_write(&UCSR0C,USBS0,low); /// mode 1 bit de stop

		pin_write(&UCSR0C,UCSZ01,high);
		pin_write(&UCSR0C,UCSZ00,high); /// on met le module USART en mode 8 bit




	//}



}


/**
 *
 * @param _data
 */

void usart_write(unsigned char _data){

	/* Wait for empty transmit buffer */
	while ( !pin_read(&UCSR0A,UDRE0) );

	/* Put data into buffer, sends the data */
	port_write(&UDR0,_data);

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

/**
 *
 * @return
 */
unsigned char usart_data_ready(void){

	 if( pin_read(&UCSR0A,RXC0) ){

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
	while ( !pin_read(&UCSR0A,RXC0) );

	/* Get and return received data from buffer */
	return port_read(&UDR0);

}




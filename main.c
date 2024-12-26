/*
 * main.c
 *
 *  Created on: 4 juin 2020
 *      Author: Kaze
 */


#include "hal_port.h"
#include "hal_usart.h"
#include "hal_spi.h"
#include "decoder_engine.h"
#include "function.h"

#define SIZE_TAB 40
//#define CTRL_Z  0x1A


volatile const unsigned char CMGS[] PROGMEM= "AT+CMGS=\"";
volatile const unsigned char ALL[] PROGMEM= "1,4";
extern volatile const unsigned char CMGD[] PROGMEM;
extern volatile const unsigned char CRLF[] PROGMEM;
unsigned char i = 0, x=0;
volatile unsigned char send = 0;
volatile unsigned char number_phone[10]="";
unsigned char spi_buffer[40]="";



ISR(USI_OVERFLOW_vect){
	
 x = USIDR;
 USISR = (1<<USIOIF);	

spi_buffer[i++] = x;

//usart_write(x);

if(i >= 36){
	
	cli();	
	//usart_write_text( spi_buffer,36 );
	send = 1;
	i = 0;

}
USIDR = 0;	
	
}


int main(void){

//*************************************** variable locale***************************
	
	

//*********************************initialisation des fonction *********************
	usart_init(baud_9600);
	function_init();
	SPI_init();
	
	usart_receive_interruption(enable);
	i = 0;
 
	get_number_phone(number_phone); // on recupere le numero stocké en memoire
	_delay_ms(10000);
	
	usart_write_text_const(CMGD, 0);
	usart_write_text_const(ALL, 0);
	usart_write_text_const(CRLF,0);
	_delay_ms(1100);
	
	//usart_write_text("init.full\r\n",0);
	sei();
	
	
	while(1){



	
		if( send == 1 ){
			
			 
			//ici on doit envoyer la reponse de la requete au telephone du proprietaire
            cli();
			usart_write_text_const(CMGS,0);
			usart_write_text(number_phone,9);
			usart_write_text("\"\r",0);
			_delay_ms(1000); //temporisation tres importante

			usart_write_text(spi_buffer, 36);
			_delay_ms(300);
			usart_write(CTRL_Z);  // on tape CTRL+Z pour envoyer le message
			_delay_ms(1000);
			sei();
			send = 0;
		}
		

		
		

		
	}




}

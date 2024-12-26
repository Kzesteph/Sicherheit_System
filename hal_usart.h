/*
 * hal_usart.h
 *
 *  Created on: 25 mai 2020
 *      Author: Kaze
 */

#ifndef HAL_USART_H_
#define HAL_USART_H_

#include "hal_port.h"

typedef enum{

	baud_2400    = 832,
	baud_4800    = 416,
	baud_9600    = 207,
	baud_14400   = 138,
	baud_19200   = 103,
	baud_28800   = 68,
	baud_38400   = 51,
	baud_57600   = 34,
	baud_76800   = 25,
	baud_115200  = 16,
	baud_230400  = 8,
	baud_250000  = 7,
	baud_500000  = 3,
	baud_1000000 = 1

}bauderate;

//**********************initialisation prototype for usart ********************************

void usart_init(bauderate _bauderate);


//********************* write operation for usart *****************************************

void usart_write(unsigned char _data);

void usart_write_text(unsigned char *text, unsigned int len);

void usart_write_text_const(volatile const unsigned char *text, unsigned int len);

//******************* read operation for usart *******************************************

unsigned char usart_data_ready(void);

unsigned char usart_data_read(void);

unsigned char usart_data_read_text(unsigned char *text);


//************************** interruption operation for usart ********************************

inline void usart_receive_interruption(activation  _state){

	if(_state == enable){

		pin_write(&UCSRB,RXCIE,high);
	}
	else{

		pin_write(&UCSRB,RXCIE,low);
	}

}









#endif /* HAL_USART_H_ */

/*
 * decoder_engine.h
 *
 *  Created on: 4 juin 2020
 *      Author: Kaze
 */

#ifndef DECODER_ENGINE_H_
#define DECODER_ENGINE_H_

#include "hal_port.h"
#include "hal_usart.h"
#include "function.h"


#define CTRL_Z  0x1A
#define _BUF_SIZE 50

unsigned char my_buffer[_BUF_SIZE];
unsigned char _index[3];



typedef enum{

	_buffer_full = 1,
	_end_recv,
	_recv_continue

}status_decoder_engine;


typedef enum{

	entete_error=0,
	entete_match
}entete_preamble;


typedef enum{

	checksum_error = 0,
	checksum_ok = 1

}checksum_status;

typedef enum{

	index_error = 0,
	index_valid

}index_status;



///**************************************************************************************************************************
/**
 *
 * @param *recv_reg     ce pointeur recupere l'adresse du registre qui contient la donnee
 * @param *_buffer_tab  ce pointeur recupere l'adresse du buffer de stockage
 * @param _end_text     cette variable recupere la valeur du caratere de fin trame
 * @param len			taille totale du buffer
 * @return
 */
inline static unsigned char reception_trame(volatile uint8_t *recv_reg, unsigned char *_buffer_tab, unsigned char _end_text, unsigned int len){

	static unsigned char recv = 0, index = 0;

	recv = *(recv_reg);


	if( (recv != _end_text) && (index < len) ){

		_buffer_tab[index++] = recv;

		return _recv_continue; /// on renvoie l'etat de la transmission

	}
	else if( index >= len){

		index = 0;

		return _buffer_full;

	}
	else{

			index = 0;
			recv = 0;
			return _end_recv;

	}

}


/**
 *
 * @param _tab
 * @param entete
 * @return
 */
inline unsigned char  get_entete(unsigned char *_tab, volatile const unsigned char *entete, unsigned char len){

unsigned char _size = 0 , i = 0;
unsigned char error = 1;

	_size = len; //strlen(entete);

	for(i=0; i < _size; i++){

		if( _tab[i] != pgm_read_byte( &entete[i] ) ){ //pgm_read_byte( &entete[i] )

			error = 0;

			break;

		}
	}


	return error;

}



/**
 *
 * @param _tab
 * @param buffer
 * @return
 */
inline unsigned char get_index(unsigned char *_tab, unsigned char *buffer){

	unsigned char i = 0;
	unsigned char error_status = 0;

	buffer[0] = 0;
	buffer[1] = 0;
	buffer[2] = 0;

	while( _tab[12 + i++] != '\r' ){

		if(i < 3){

			buffer[i-1] = _tab[12 + i-1];

			error_status = 1;
		}
		else{

			buffer[0] = 0;
			buffer[1] = 0;
			buffer[2] = 0;

			error_status = 0;
			break;
		}

	}

	i = 0;

	return error_status;
}


inline unsigned char string_to_decimal(unsigned char *pt){

unsigned char number=0;

	if( (pt[1] == 0) && (pt[0] >= 0x30) && (pt[0] <= 0x39) ){


		return pt[0] - 48;

	}
	else if( (pt[0] >= 0x30) && (pt[0] <= 0x39) && (pt[1] >= 0x30) && (pt[1] <= 0x39) ){

		number = ( (pt[0] - 48) * 10 ) + (pt[1] - 48);

		return number;

	}
	else{

		return 0;
	}

}

/************************************************************************/
/*                                                                      */
/************************************************************************/

inline void copy_char(unsigned char *_tab_recv, unsigned char *_tab_send, unsigned char len){

	unsigned char i =0;

	for(i=0; i<len; i++){
		
		_tab_recv[i] = _tab_send[i];
	}

}


/************************************************************************/
/*                                                                      */
/************************************************************************/

 
inline void save_phone_number(unsigned char *number){
	
	uint8_t i = 0;
	
	for(i=0; i<9; i++){
		
		eeprom_update_byte( (uint8_t *) i, number[i] );
	}
	
	
	
}
 

/************************************************************************/
/*                                                                      */
/************************************************************************/

 
inline void get_number_phone(volatile unsigned char *number){
	
	uint8_t i = 0;
		
	for(i=0; i<9; i++){
			
		number[i] = eeprom_read_byte( (const uint8_t *) i);
	}
		
	
}
 
#endif /* DECODER_ENGINE_H_ */

/*
 * decoder_engine.h
 *
 *  Created on: 27 mai 2020
 *      Author: michel_granda
 */

#ifndef DECODER_ENGINE_H_
#define DECODER_ENGINE_H_

#include "hal_port.h"
#include "NMEA_sentence.h"

volatile GNRMC_sentence GNRMC_var;



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
 */
inline unsigned char  get_entete(unsigned char *_tab, char *entete){

unsigned int _size = 0 , i = 0;
unsigned char error = 1;

	_size = strlen(entete);

	for(i=0; i < _size; i++){

		if( _tab[i] != entete[i] ){

			error = 0;

			break;

		}
	}


	return error;

}


/**
 *
 * @param _tab
 * @return
 */
inline unsigned char get_checksum(unsigned char *_tab){

unsigned char checksum_in_trame = 0;
unsigned char checksum_cal = 0;
unsigned char i = 0, buf[3];

	checksum_cal =0;

	for(i=0; _tab[i+1] != '*'; i++ ){

			checksum_cal ^= _tab[i+1];
	}

 /// on recupere le checksum a la fin de la trame
 ///
	buf[0] = _tab[i+2];
	buf[1] = _tab[i+3];
	buf[2] = 0;



	checksum_in_trame = (unsigned char)( strtol(buf,0,16) );


	///comparaison du cheksum

	if(checksum_cal == checksum_in_trame){

		return checksum_ok;
	}
	else{

		return checksum_error;
	}


}

/**
 *
 * @param _tab
 * @param _tab_resultat
 * @param _delemiter
 * @return
 */
unsigned char string_recupere(unsigned char *_tab, unsigned char *_tab_resultat,  unsigned char _delemiter, unsigned char _position);

/**
 *
 * @param _tab
 * @param pt
 * @return
 */
inline GNRMC_sentence decode_NMEA_GNRMC_sentence(unsigned char *_tab){

GNRMC_sentence nmea;

	string_recupere(_tab, nmea.entete,        ',', 0);
	string_recupere(_tab, nmea.time_utc,      ',', 1);
	string_recupere(_tab, nmea.status,        ',', 2);
	string_recupere(_tab, nmea.latitude, 	  ',', 3);
	string_recupere(_tab, nmea.hemisphere_NS, ',', 4);
	string_recupere(_tab, nmea.longitude,     ',', 5);
	string_recupere(_tab, nmea.hemisphere_EW, ',', 6);
	string_recupere(_tab, nmea.speed,         ',', 7);
	string_recupere(_tab, nmea.cog,           ',', 8);
	string_recupere(_tab, nmea.date,          ',', 9);


	return nmea;

}

/**
 *
 * @param _buf
 * @param len
 */

void flush_buffer(unsigned char *_buf, unsigned int len);


/*

*/
inline void copy_data(unsigned char *_tab_recv, unsigned char *_tab_send, unsigned char len){

	unsigned char i =0;

	for(i=0; i<len; i++){
		
		_tab_recv[i] = _tab_send[i];
	}

}

#endif /* DECODER_ENGINE_H_ */

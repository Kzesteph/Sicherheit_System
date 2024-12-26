/*
 * decoder_engine.c
 *
 *  Created on: 27 mai 2020
 *      Author: michel_granda
 */

#include "decoder_engine.h"
#include "hal_usart.h"

#define _BUF_SIZE  90

unsigned char buffer_recv[_BUF_SIZE];



ISR(USART_RX_vect){



	switch( reception_trame(&UDR0,buffer_recv,'\n',_BUF_SIZE) ){ /// on enregistre chaque octect


		case _buffer_full:

			flush_buffer(buffer_recv, _BUF_SIZE);

			break;

		case _end_recv:  /// lorsque la trame a ete recue correctement on passe a la verification de l'entete

				switch( get_entete(buffer_recv,"$GPRMC") ){///verification de l'entete

					case entete_match:

						if (get_checksum(buffer_recv) == checksum_ok){

							///lorsqu'on est ici c'est que l,entete correspond et que le message est bien arrivé sans erreur

							GNRMC_var = decode_NMEA_GNRMC_sentence(buffer_recv);

							/*
							usart_write_text(GNRMC_var.entete, 7);
							usart_write_text("-",1);
							usart_write_text(GNRMC_var.time_utc,11);
							usart_write_text("-",1);
							usart_write_text(GNRMC_var.status,2);
							usart_write_text("-",1);
							usart_write_text(GNRMC_var.latitude,9);
							usart_write_text("-",1);
							usart_write_text(GNRMC_var.hemisphere_NS,2);
							usart_write_text("-",1);
							usart_write_text(GNRMC_var.longitude,10);
							usart_write_text("-",1);
							usart_write_text(GNRMC_var.hemisphere_EW,0);
							usart_write_text("-",1);
							usart_write_text(GNRMC_var.speed,0);
							usart_write_text("-",1);
							usart_write_text(GNRMC_var.cog,0);
							usart_write_text("-",1);
							usart_write_text(GNRMC_var.date,0);
							usart_write_text("\n",1);
							usart_write_text(buffer_recv, _BUF_SIZE);*/

							flush_buffer(buffer_recv, _BUF_SIZE);

						}
						else{
							
							flush_buffer(buffer_recv, _BUF_SIZE);
						}

						break;

					case entete_error:

						flush_buffer(buffer_recv, _BUF_SIZE);

						break;

					default:

						break;

				}

			break;


		case _recv_continue:


			break;

		default:

			break;

	}


}


/**
 *
 * @param _buf
 * @param len
 */
void flush_buffer(unsigned char *_buf, unsigned int len){

	unsigned int i = 0;

	for(i=0; i<len; i++){

		_buf[i] = 0;
	}
}


/**
 *
 * @param _tab
 * @param _delemiter
 */
unsigned char string_recupere(unsigned char *_tab, unsigned char *_tab_resultat,  unsigned char _delemiter, unsigned char _position){

unsigned char buf[12] = {'0','0','0','0','0','0','0','0','0','0','0',0};
unsigned char delimiter_pos[20]={0,0,0,0,0,0};
unsigned char tampon[80];
unsigned char *pt;
unsigned char i=0, index=0, max_element=0;

	flush_buffer(tampon,80);

	strcpy(tampon,_tab);

	while(tampon[i++] != '\0'){

			if( (tampon[i] == _delemiter)  || (tampon[i] == '*') ){


				delimiter_pos[1+index++] = i;

			}

	}

	max_element = index + 1;

	i = 0;
	index = 0;


	switch(_position){

	case 0: ///entete

		tampon[ delimiter_pos[_position + 1] ] = 0;
		strcpy(_tab_resultat,tampon);

		break;


	case 1: ///time utc

		tampon[ delimiter_pos[_position + 1] ] = 0;
		strcpy(_tab_resultat, (tampon + delimiter_pos[_position] + 1) );

		break;


	case 2: /// status

		tampon[ delimiter_pos[_position + 1] ] = 0;
		strcpy(_tab_resultat, (tampon + delimiter_pos[_position] + 1) );

		break;

	case 3: //latitude

			pt = (tampon + delimiter_pos[_position] + 1);

		if( *pt == _delemiter){

			strcpy(_tab_resultat, buf ); // on remplie les 0
		}
		else{
			tampon[ delimiter_pos[_position + 1] ] = 0;
			strcpy(_tab_resultat, (tampon + delimiter_pos[_position] + 1) );
		}

		break;


	case 4://hemisphere nord sud

		pt = (tampon + delimiter_pos[_position] + 1);

		if( *pt == _delemiter){

			strcpy(_tab_resultat, "0" );
		}
		else{

			tampon[ delimiter_pos[_position + 1] ] = 0;
			strcpy(_tab_resultat, (tampon + delimiter_pos[_position] + 1) );
		}

		break;

	case 5://longitude

		pt = (tampon + delimiter_pos[_position] + 1);

		if( *pt == _delemiter){

			//strcpy(_tab_resultat, "0" ); // on remplie les 0
			copy_data(_tab_resultat,buf,10);
		}
		else{

			tampon[ delimiter_pos[_position + 1] ] = 0;
			strcpy(_tab_resultat, (tampon + delimiter_pos[_position] + 1) );
		}

		break;


	case 6://hemisphere ouest est

		pt = (tampon + delimiter_pos[_position] + 1);

		if( *pt == _delemiter){

			strcpy(_tab_resultat, "0" );
		}
		else{

			tampon[ delimiter_pos[_position + 1] ] = 0;
			strcpy(_tab_resultat, (tampon + delimiter_pos[_position] + 1) );
		}

		break;


	case 7://speed

		pt = (tampon + delimiter_pos[_position] + 1);

		if( *pt == _delemiter){

			strcpy(_tab_resultat, buf );
		}
		else{

			tampon[ delimiter_pos[_position + 1] ] = 0;
			strcpy(_tab_resultat, (tampon + delimiter_pos[_position] + 1) );
		}

		break;


	case 8://cog


		pt = (tampon + delimiter_pos[_position] + 1);

		if( *pt == _delemiter){

			strcpy(_tab_resultat, "0" );
		}
		else{

			tampon[ delimiter_pos[_position + 1] ] = 0;
			strcpy(_tab_resultat, (tampon + delimiter_pos[_position] + 1) );
		}

		break;


	case 9: //date


		pt = (tampon + delimiter_pos[_position] + 1);

		if( *pt == _delemiter){

			strcpy(_tab_resultat, "000000" );
		}
		else{

			tampon[ delimiter_pos[_position + 1] ] = 0;
			strcpy(_tab_resultat, (tampon + delimiter_pos[_position] + 1) );
		}

		break;

	case 10:

		pt = (tampon + delimiter_pos[_position] + 1);

		if( *pt == _delemiter){

			strcpy(_tab_resultat, "0" );
		}
		else{

			tampon[ delimiter_pos[_position + 1] ] = 0;
			strcpy(_tab_resultat, (tampon + delimiter_pos[_position] + 1) );
		}

		break;

	case 11:

		pt = (tampon + delimiter_pos[_position] + 1);

		if( *pt == _delemiter){

			strcpy(_tab_resultat, "0");
		}
		else{

			tampon[ delimiter_pos[_position + 1] ] = 0;
			strcpy(_tab_resultat, (tampon + delimiter_pos[_position] + 1) );
		}

		break;


	case 12:

		pt = (tampon + delimiter_pos[_position] + 1);

		if( *pt == _delemiter){

			strcpy(_tab_resultat, "0" );
		}
		else{

			tampon[ delimiter_pos[_position + 1] ] = 0;
			strcpy(_tab_resultat, (tampon + delimiter_pos[_position] + 1) );
		}

		break;


	default:

		break;


	}



	return 1;
}

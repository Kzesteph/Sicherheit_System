/*
 * decoder_engine.c
 *
 *  Created on: 4 juin 2020
 *      Author: Kaze
 */

#include "decoder_engine.h"



 volatile const unsigned char CMTI[] PROGMEM= "+CMTI:";
 volatile const unsigned char GPSM[] PROGMEM= "+GPSM:";
 volatile const unsigned char CMGR[] PROGMEM= "AT+CMGR=";
 volatile const unsigned char NUMB[] PROGMEM= "+NUMB="; // exemple +NUMB=697544991
 volatile const unsigned char CRLF[] PROGMEM= "\r\n";
 volatile const unsigned char CMGD[] PROGMEM= "AT+CMGD=";
 volatile const unsigned char CMGS2[] PROGMEM= "AT+CMGS=\"";
 volatile const unsigned char RECU[] PROGMEM= " est enregistré comme numero du proprietaire";

  extern volatile unsigned char number_phone[10];
 
 unsigned char index_lue;
 unsigned char sms_index[3];

ISR(USART_RX_vect){

		switch( reception_trame(&UDR,my_buffer,'\n',_BUF_SIZE) ){

			case _buffer_full:

				flush_buffer(my_buffer, _BUF_SIZE);

				break;

			case _end_recv:

//***************************
				switch( get_entete(my_buffer,CMTI,6) ){///verification de l'entete lors de la reception d'un message

					case entete_match:

						switch( get_index(my_buffer, _index) ){

							case index_valid: //ici c'est quand l'index est valid
								
								copy_char(sms_index, _index,3);
								
								usart_write_text_const(CMGR, 0);
								usart_write_text(_index, 0);
								usart_write_text_const(CRLF,0); /// on lit le message en donnant l'emplacement du message

								

								break;

							case index_error:

								flush_buffer(_index, 3);

								break;


							default:

								break;

						}

						flush_buffer(my_buffer, _BUF_SIZE);

						break;

					case entete_error:



						break;

					default:

						break;

				}/// fin de test de reception de +CMTI


///***********************

				switch( get_entete(my_buffer,GPSM, 6) ){///debut test de reception de +GPSM: "MG",00CRLF


					case entete_match:

						switch( get_index(my_buffer, _index) ){

						case index_valid:

 
							// debut de l'effacement du message
							
							usart_write_text_const(CMGD, 0);
							usart_write_text(sms_index, 0);
							usart_write_text_const(CRLF,0);
							
							//fin de l'effacement du message
				 
							
							/// ici on envoie le code function a realiser
							function_write( string_to_decimal(_index) );
							gsm_request();

							
							flush_buffer(sms_index, 0x03);


							break;

						case index_error:

							flush_buffer(_index, 3);

							break;


						default:

							break;


						}


						flush_buffer(my_buffer, _BUF_SIZE);

						break;

						case entete_error:


							break;


						default:


							break;

				}///fin de reception de +GPSM:


//*****************************************
				switch ( get_entete(my_buffer,NUMB, 6)){//debut de reception de +NUMB= pour la reception du numero de telephone

					case entete_match:

						save_phone_number( my_buffer + 6 );//enregistrement du numero de telephone ou le module enverra le message
						copy_char(number_phone, my_buffer+6, 9); //on sauvegarde le numero
 
						// on envoie le message de reponse
						
						usart_write_text_const(CMGS2,0);
						usart_write_text(number_phone,9);
						usart_write_text("\"\r",0);
						_delay_ms(1000); //temporisation tres importante
					
						usart_write_text(number_phone,9);					
						usart_write_text_const(RECU,0);
						_delay_ms(300);
						usart_write(CTRL_Z);  // on tape CTRL+Z pour envoyer le message	
						_delay_ms(1000);
						// fin d'envoie du sms


						// debut de l'effacement du message
							
						usart_write_text_const(CMGD, 0);
						usart_write_text(sms_index, 0);
						usart_write_text_const(CRLF,0);
							
						//fin de l'effacement du message
							
						flush_buffer(sms_index, 0x03);
						flush_buffer(my_buffer, _BUF_SIZE);
 
						break;

					case entete_error:


						break;

					default:

						break;

				}///fin de reception de +NUMB:

//***********************************************

				flush_buffer(my_buffer, _BUF_SIZE);

				break;


			case _recv_continue:

				

				break;


			default:


				break;

		}








}

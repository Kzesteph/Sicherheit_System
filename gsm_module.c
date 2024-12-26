/**
 * gsm_module.c
 *
 *  Created on: 27 mai 2020
 *      Author: michel_granda
 */

#include "gsm_module.h"
#include "alarm.h"
#include "alarm.h"
#include "bougie.h"
#include "decoder_engine.h"
#include "battery_sensor.h"

extern volatile unsigned char reponse;
extern volatile unsigned char system_state;
extern volatile unsigned char spi_buffer[40];//="+GPSM:";
extern volatile unsigned char oui;
extern volatile unsigned char gps_bit; 
ISR(INT0_vect){


	switch( function_read() ){

	case set_alarme:
	
/*
		spi_buffer[6]  = 48 + set_alarme;//code function
		spi_buffer[7]  = 48 + 1;   //len
		spi_buffer[8]  = 48 + alarm_set();//data
		spi_buffer[9]  = '*';
		spi_buffer[10] = crc8(spi_buffer,9);
*/
		buzzer_bipper(5);
		
 
		reponse = 1;
		break;

	case reset_alarme:

		
		
		spi_buffer[6] = 48 + reset_alarme;//code function
		spi_buffer[7] = 48 + 1;   //len
		spi_buffer[8] = 48 + alarm_reset();//data
		spi_buffer[9] = '*';
		spi_buffer[10] = crc8(spi_buffer,9);

		//on sauvegarde l'etat du system



		reponse = 1;
		break;

	case ok:
	
		

		break;

	case read_alarme_status:

		spi_buffer[6] = 48 + read_alarme_status;//code function
		spi_buffer[7] = 48 + 1;   //len
		spi_buffer[8] = 48 +  alarm_get_state();//data
		spi_buffer[9] = '*';
		spi_buffer[10] = crc8(spi_buffer,9);

		reponse = 1;

		break;


	case set_bougie:

		spi_buffer[6] = 48 + set_bougie;//code function
		spi_buffer[7] = 48 + 1;   //len
		spi_buffer[8] = 48 + bougie_close();//data
		spi_buffer[9] = '*';
		spi_buffer[10] = crc8(spi_buffer,9);

		//on sauvegarde l'etat du system

 
		reponse = 1;
		break;

	case reset_bougie:

		spi_buffer[6] = 48 + reset_bougie;//code function
		spi_buffer[7] = 48 + 1;   //len
		spi_buffer[8] = 48 + bougie_open();//data
		spi_buffer[9] = '*';
		spi_buffer[10] = crc8(spi_buffer,9);

		//on sauvegarde l'etat du system
 
		reponse = 1;
		break;

	case pulse_bougie:

		oui = 2;

		break;


	case read_bougie_status:

		gps_bit = 1;
		
		break;

	case get_battery_voltage:
	oui = 1;
       /*
		spi_buffer[6] = 48 + get_battery_voltage;//code function
		spi_buffer[7] = 48 + 12;   //len
		copy_char(spi_buffer + 8, get_sensor_battery(), 5);//data
		copy_char(spi_buffer + 13, get_sensor_secours(), 5);//data
		spi_buffer[18] = '*';
		spi_buffer[19] = crc8(spi_buffer,20);

		reponse = 1;		
		*/
		break;


	case get_position_gps:

		//GNRMC_var.latitude
		
		spi_buffer[6] = 48 + get_position_gps;//code function
		spi_buffer[7] = 48 + 26;   //len
		copy_char(spi_buffer + 8,   GNRMC_var.latitude,		 9); // de 8 a 16
		copy_char(spi_buffer + 17,  GNRMC_var.hemisphere_NS, 1); // de 17 a 17
		copy_char(spi_buffer + 18,  GNRMC_var.longitude,     10); // de 18 a 27
		copy_char(spi_buffer + 28,  GNRMC_var.hemisphere_EW, 1); // de 28 a 28
		copy_char(spi_buffer + 29,  GNRMC_var.speed, 05); // de 29 a 33				
		spi_buffer[34] = '*';
		spi_buffer[35] = crc8(spi_buffer,34);

		reponse = 1;
		break;

	case get_all:


		spi_buffer[6] = 48 + get_all;//code function
		spi_buffer[7] = 48 + 2;   //len
		spi_buffer[8] = 48 + bougie_get_state();//data1
		spi_buffer[9] = 48 + alarm_get_state();//data2
		spi_buffer[10] = '*';
		spi_buffer[11] = crc8(spi_buffer,10);
		reponse = 1;

		break;

	case set_all:

		spi_buffer[6]  = 48 + set_all;//code function
		spi_buffer[7]  = 48 + 2;   //len
		spi_buffer[8]  = 48 + bougie_close();//data1
		spi_buffer[9]  = 48 + alarm_set(); //data2
		spi_buffer[10] = '*';
		spi_buffer[11] = crc8(spi_buffer,10);

		//on sauvegarde l'etat du system

 
		reponse = 1;		

		break;

	case reset_all:

		spi_buffer[6]  = 48 + reset_all;//code function
		spi_buffer[7]  = 48 + 2;   //len
		spi_buffer[8]  = 48 + bougie_open();//data1
		spi_buffer[9]  = 48 + alarm_reset(); //data2
		spi_buffer[10] = '*';
		spi_buffer[11] = crc8(spi_buffer,10);

		//on sauvegarde l'etat du system

 
		reponse = 1;

		break;

	case enable_system: // activation du systeme de protection

		system_state = 1;
		
		bougie_open(); // on ouvre le contact de la bougie
		alarm_reset(); // on desactive la sirene
		
		spi_buffer[6]  = 48 + enable_system;//code function
		spi_buffer[7]  = 48 + 1;   //len
		spi_buffer[8]  = 48 + system_state;//data1
		spi_buffer[9] = '*';
		spi_buffer[10] = crc8(spi_buffer,9);
		eeprom_update_byte((uint8_t *)0x02, system_state );

		reponse = 1;
		break;

	case disable_system: //desactivation du systeme de protection

		system_state = 0;
		bougie_close(); // on ferme le contact de la bougie
		alarm_reset(); // on arrete l'alarme

		spi_buffer[6]  = 48 + disable_system;//code function
		spi_buffer[7]  = 48 + 1;   //len
		spi_buffer[8]  = 48 + system_state;//data1
		spi_buffer[9] = '*';
		spi_buffer[10] = crc8(spi_buffer,9);

		eeprom_update_byte((uint8_t *)0x02, system_state );

		reponse = 1;
		break;

	default:


		break;

	}








}

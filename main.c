/**
 * main.c
 *
 *  Created on: 25 mai 2020
 *      Author: michel_granda
 */



#include "hal_port.h"
#include "hal_usart.h"
#include "hal_adc.h"
#include "hal_i2c.h"
#include "spi.h"
#include "alarm.h"
#include "bougie.h"
#include "gsm_module.h"
#include "decoder_engine.h"
#include "detecteur_mouvement.h"
#include "battery_sensor.h"

#define SIZE_TAB 40



unsigned char i = 0 ,a;
unsigned char entete[] = "+GPSM:";
//char _buffer[10];

volatile unsigned char reponse = 0;
volatile unsigned char system_state = 0;
volatile unsigned char spi_buffer[SIZE_TAB]="";
volatile unsigned char oui = 0;
volatile unsigned char gps_bit = 0;

unsigned char MPU_BUFFER[20]={0,0,0,0,0};
	
unsigned char infraction[SIZE_TAB]="Tentative de vol actuel sur vehicule";

unsigned char viol_distance[SIZE_TAB]="violation de distance du vehicule.";	

unsigned char nombre_mouvement = 0;
int OLD_GYRO[5]={0,0,0,0};
int NEW_GYRO[5]={0,0,0,0};
float valeur=0;
unsigned char bp=0;
	
uint8_t system_state_adresse = 0x02;

void niveau1(unsigned char *number);
void niveau2(void);
void send_sms(unsigned char *tab);
void delay_seconde(unsigned long temp);
void position_gps(unsigned char *pt);
int main(void){


		copy_char(spi_buffer, entete, 6);
		//strcpy(spi_buffer,entete);


		usart_init(baud_9600);
		usart_receive_interruption(enable);
		SPI_init(FOSC_DIV_4, master_mode,MSB_FIRST);
		I2C_Init(100000);
		ADC_init();

		gsm_init();
		alarm_init();
		bougie_init();

		detecteur_mouvement_init();
		
		system_state = eeprom_read_byte((const uint8_t *)system_state_adresse); // avoir l'etat prec. du systeme
		alarm_restaure_state(); // pareil ici
		bougie_restaure_state();// et ici

		if(1){
			
			get_mouvement(MPU_BUFFER);
			store_gyroscope(MPU_BUFFER,OLD_GYRO); // on sauvegarde la position gyroscopique
		    usart_write_text(MPU_BUFFER,14);
		}
 
		//_delay_ms(10000); /// attente d'initialisation
		
		usart_write_text("initialisé\r\n",0);
		_delay_ms(100);
		usart_write_text("initialisé\r\n",0);
		_delay_ms(100);
		usart_write_text("initialisé\r\n",0);
		
		
		sei(); /// activation des interruption globales
		
		
	for(;;){


		
		
		get_mouvement(MPU_BUFFER); // on lit les positions gyroscopiques
		store_gyroscope(MPU_BUFFER,NEW_GYRO); // on enregistre les position gyroscopiques
		
		// si le systeme d'alarme est activé
		if( (1)  && ( mouvement_changed(OLD_GYRO, NEW_GYRO) == 1 ) ){
			
			cli();
			
			nombre_mouvement++;
			usart_write_text("\r\nmouvement detecte\r\n",0);
			get_mouvement(MPU_BUFFER);
			store_gyroscope(MPU_BUFFER,OLD_GYRO);
			_delay_ms(1000);
			
			niveau1(&nombre_mouvement);
			
			
			sei();
		}
		
// debut niveau 2
		if( 1 ){
			
			valeur = (ADC_read(7) * 5.0)/1024.0;
			
			if(valeur > 2.5){
				cli();
				niveau2();
				sei();
			}
			oui = 0;
		}
// fin niveau 2			
		

		position_gps(&gps_bit);





	}
}



//*************************************************************************************

void send_sms(unsigned char *tab){
	
		
		//if(reponse == 1){
			
			external_interrupt_enable(disable_state, rising);
			
			EIFR = 0x01;
			SPI_write_buffer(tab, 36);
			
			alarm_store_state();
			bougie_store_state();
			
			
			_delay_ms(500);
			
			EIFR = 0x01;
			external_interrupt_enable(enable_state, rising);
			
		
			//reponse = 0;}	
	
	
	
	
}


void niveau1(unsigned char *number){
	
	unsigned char count_niv1=0;
	
	if( *number >= 4){
		
		*number = 0;
		
		send_sms(infraction);
		delay_seconde(10);
		
		usart_write_text("infraction1\n\r",0);
		
		send_sms(infraction);
		delay_seconde(10);
		usart_write_text("infraction2\n\r",0);
		
		send_sms((char*)"Etes vous en accord avec cette alerte?");
		
		sei();
			
		for(count_niv1=0; count_niv1<60; count_niv1++){
			
			_delay_ms(1000);
			usart_write_text("attente\n\r",0);
			
			if(oui == 1){
				
				delay_seconde(10);
				send_sms((char*)"vous etes en accord");
				delay_seconde(10);
				break;
				
			}
			
			if(oui == 2){
				
				delay_seconde(10);
				send_sms((char*)"vous etes en desaccord");
				delay_seconde(10);
				break;
				
			}
			
		}
		
		cli();
		
		usart_write_text("fin attente\n\r",0);		
		
		
		if(oui == 1){ // si on est en accord
			
			oui = 0;
			
		}
		else{ // si on est pas en accord
			
			oui = 0;
			buzzer_bipper(10);
			
			copy_char(spi_buffer,entete,6);
			spi_buffer[6] = 48 + get_position_gps;//code function
			spi_buffer[7] = 48 + 26;   //len
			copy_char(spi_buffer + 8,  GNRMC_var.latitude,		 9); // de 8 a 16
			copy_char(spi_buffer + 17,  GNRMC_var.hemisphere_NS, 1); // de 17 a 17
			copy_char(spi_buffer + 18,  GNRMC_var.longitude,     10); // de 18 a 27
			copy_char(spi_buffer + 28,  GNRMC_var.hemisphere_EW, 1); // de 28 a 28
			copy_char(spi_buffer + 29,  GNRMC_var.speed, 05); // de 29 a 33
			spi_buffer[34] = '*';
			spi_buffer[35] = crc8(spi_buffer,34);
			
			send_sms(spi_buffer);
			delay_seconde(10);
			
		}
	
		
		
	}
	
	
	
}



void niveau2(void){
	
	unsigned char count_niv2=0;

		send_sms(viol_distance);
		delay_seconde(10);
		usart_write_text("violation1\n\r",0);
		
		
		send_sms(viol_distance);
		delay_seconde(10);
		usart_write_text("violation2\r\n",0);
		
		send_sms((char*)"Etes vous en accord avec cette alerte?");
		
		sei();
		
		for(count_niv2=0; count_niv2<60; count_niv2++){
			
			_delay_ms(1000);
			usart_write_text("attente 2\n\r",0);
			
			if (oui == 1){
				
				delay_seconde(10);
				send_sms((char*)"vous etes en accord");
				delay_seconde(10);
				break;
				
			}
			
			if(oui == 2){
				
				delay_seconde(10);
				send_sms((char*)"vous etes en desaccord\r\n");
				delay_seconde(10);
				break;
				
			}
			
		}
		
		cli();
		usart_write_text("fin attente 2\n\r",0);
		
		
		if(oui == 1){ // si on est en accord
			
			oui = 0;
			
		}
		else{ // si on est pas en accord
			
			oui = 0;
			
			bougie_close();
			
			copy_char(spi_buffer,entete,6);
			spi_buffer[6] = 48 + get_position_gps;//code function
			spi_buffer[7] = 48 + 26;   //len
			copy_char(spi_buffer + 8,  GNRMC_var.latitude,		 9); // de 8 a 16
			copy_char(spi_buffer + 17,  GNRMC_var.hemisphere_NS, 1); // de 17 a 17
			copy_char(spi_buffer + 18,  GNRMC_var.longitude,     10); // de 18 a 27
			copy_char(spi_buffer + 28,  GNRMC_var.hemisphere_EW, 1); // de 28 a 28
			copy_char(spi_buffer + 29,  GNRMC_var.speed, 05); // de 29 a 33
			spi_buffer[34] = '*';
			spi_buffer[35] = crc8(spi_buffer,34);
			
			send_sms(spi_buffer);
			delay_seconde(10);
			
		}	
	
	
	
	
}




void position_gps(unsigned char *pt){
	
	unsigned char tab[40]={0,0,0,0};
	
	if(*pt == 1){
		
			cli();
			
			copy_char(tab,entete,6);
			tab[6] = 48 + get_position_gps;//code function
			tab[7] = 48 + 26;   //len
			copy_char(tab + 8,  GNRMC_var.latitude,		 9); // de 8 a 16
			copy_char(tab + 17,  GNRMC_var.hemisphere_NS, 1); // de 17 a 17
			copy_char(tab + 18,  GNRMC_var.longitude,     10); // de 18 a 27
			copy_char(tab + 28,  GNRMC_var.hemisphere_EW, 1); // de 28 a 28
			copy_char(tab + 29,  GNRMC_var.speed, 05); // de 29 a 33
			tab[34] = '*';
			tab[35] = crc8(tab,34);
			usart_write_text("demande de position\n\r",0);
		
				
			send_sms(tab);
			
			
			delay_seconde(10);
		
		
		   *pt = 0;
		   
			usart_write_text(tab,36);
			
		sei();
	}
}


void delay_seconde(unsigned long temp){
	
	unsigned long count =0;
	
	for(count=0; count<temp; count++){
		
		_delay_ms(1000);
	}
	
	
}
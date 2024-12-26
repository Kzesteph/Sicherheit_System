/*
 * hal_i2c.c
 *
 * Created: 06/08/2020 11:12:14
 *  Author: MICHEL GRANDA
 */ 

#include "hal_i2c.h"



void I2C_Init(unsigned long _freq){
	
	pin_write(&TWSR,TWPS0,low);
	pin_write(&TWSR,TWPS1,low);
	
	TWBR = (unsigned char)((( F_CPU / _freq) - 16) / 2);
	
	TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWEA); 
	
}

void I2C_Start(void){
	
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);  //Send START condition
	while ( !(TWCR & (1<<TWINT)) ); // Wait for TWINT flag set. This indicates that the START condition has been transmitted
	
}

void I2C_Repeated_Start(void){
	
	I2C_Start();
	
}

void I2C_Write(unsigned char data){
	
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN); //Load SLA_W into TWDR register. Clear TWINT bit in TWCR to start transmission of address
	while ( !(TWCR & (1<<TWINT)) ); //Wait for TWINT flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received.
}

unsigned char I2C_Read(unsigned char reponse){
	
	
	if(reponse == 0){
		
		// start receiving without acknowledging reception
		TWCR = (1<<TWINT) | (1<<TWEN);
	}
	else{
		
			// start TWI module and acknowledge data after reception
			TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	}
	
		while( !(TWCR & (1<<TWINT)) ); // wait for end of transmission
	
		return TWDR; // return received data from TWDR
		
}

void I2C_Stop(void){
	
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); //Transmit STOP condition
}
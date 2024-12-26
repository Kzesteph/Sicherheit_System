/*
 * detecteur_mouvement.c
 *
 * Created: 06/08/2020 15:00:38
 *  Author: MICHEL GRANDA
 */ 

#include "detecteur_mouvement.h"
#include "hal_usart.h"

#define MPU_ADRESSE_WRITE 0xD0
#define MPU_ADRESSE_READ  0xD1

#define LIMIT 30000

void detecteur_mouvement_init(void){
	
	I2C_Start(); // le maitre initie la communication
	I2C_Write(MPU_ADRESSE_WRITE);// il envoie l'adresse de l'esclave
	I2C_Write(0x6B);  //PWR_MGMT_1 adresse
	I2C_Write(0x80);
	I2C_Stop(); // il arrete la communication
	
	_delay_ms(200);
	
	I2C_Start();
	I2C_Write(MPU_ADRESSE_WRITE);
	I2C_Write(0x6B);  //PWR_MGMT_1 adresse
	I2C_Write(0x00);
	I2C_Stop();
	
	_delay_ms(200);
	
	I2C_Start();
	I2C_Write(MPU_ADRESSE_WRITE);
	I2C_Write(0x6C);  //PWR_MGMT_2 adresse
	I2C_Write(0x00);
	I2C_Stop();
	_delay_ms(200);
	
	// x,y,z
}

void store_gyroscope(unsigned char *mouvement_position, int *gyroscope){
	
	int gyro_x=0, gyro_y=0, gyro_z=0;
	
	
	gyro_x = (int)( (mouvement_position[8]  << 8) | mouvement_position[9]  );
	gyro_y = (int)( (mouvement_position[10] << 8) | mouvement_position[11] );
	gyro_z = (int)( (mouvement_position[12] << 8) | mouvement_position[13] );
	
	gyroscope[0] = gyro_x;
	gyroscope[1] = gyro_y;
	gyroscope[2] = gyro_z;
	
	
}

void get_mouvement(unsigned char *mouvement_position){
	
	unsigned char i=0,x=0;
	
	I2C_Start();
	I2C_Write(MPU_ADRESSE_WRITE);
	I2C_Write(0x3B);
	
	I2C_Start();
	I2C_Write(MPU_ADRESSE_READ);  
	
	
	for(i=0; i<13; i++){
	    
		x =I2C_Read(1);
		mouvement_position[i] = x; 
			
	}
	
	x =I2C_Read(0);
	mouvement_position[13] = x;
	
	I2C_Stop();
	
}

unsigned char mouvement_changed(int *old_gyroscope, int *new_gyroscope){
	
	int difference_x=0, difference_y=0, difference_z=0;
	
	difference_x = abs( old_gyroscope[0] - new_gyroscope[0] );
	difference_y = abs( old_gyroscope[1] - new_gyroscope[1] );
	difference_z = abs( old_gyroscope[2] - new_gyroscope[2] );
	
	if( (difference_x > LIMIT) /*|| (difference_y > LIMIT) || (difference_z > LIMIT) */){
		
		return 1;
	}
	else{
	
		return 0;
		
	}
	
}

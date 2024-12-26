/*
 * hal_i2c.h
 *
 * Created: 06/08/2020 10:46:32
 *  Author: MICHEL GRANDA
 */ 


#ifndef HAL_I2C_H_
#define HAL_I2C_H_

#include "hal_port.h"


void I2C_Init(unsigned long _freq);

void I2C_Start(void);

void I2C_Repeated_Start(void);

void I2C_Write(unsigned char data);

unsigned char I2C_Read(unsigned char reponse);

void I2C_Stop(void);




#endif /* HAL_I2C_H_ */
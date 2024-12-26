/*
 * hal_adc.h
 *
 *  Created on: 11 juin 2020
 *      Author: MICHEL GRANDA
 */

#ifndef HAL_ADC_H_
#define HAL_ADC_H_

#include "hal_port.h"

void ADC_init(void);

int ADC_read(unsigned char _channel);



#endif /* HAL_ADC_H_ */

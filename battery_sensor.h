/*
 * battery_sensor.h
 *
 * Created: 14/06/2020 13:34:51
 *  Author: MICHEL GRANDA
 */ 


#ifndef BATTERY_SENSOR_H_
#define BATTERY_SENSOR_H_

#include "hal_adc.h"

unsigned char* get_sensor_battery(void);

unsigned char* get_sensor_secours(void);




#endif /* BATTERY_SENSOR_H_ */
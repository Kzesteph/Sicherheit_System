/*
 * battery_sensor.c
 *
 * Created: 14/06/2020 14:25:57
 *  Author: MICHEL GRANDA
 */ 

#include "battery_sensor.h"


unsigned char *get_sensor_battery(void){

	static char _buffer[6];
	unsigned char i=0;
	
	float voltage;

	voltage = (ADC_read(6) * 5.0)/1024.0;

	voltage *= 11.0;
	
	dtostrf(voltage,3,2,_buffer);

	for(i=0; i<6; i++){
		
		if(_buffer[i] == 0) {
			
			_buffer[i] = '0';
		}
	}
	
	return _buffer;
}


unsigned char *get_sensor_secours(void){

	static char _buffer_secours[6];
	unsigned char i=0;
	float voltage_secours;

	voltage_secours = (ADC_read(7) * 5.0)/1024.0;

	voltage_secours *= 11.0;
	
	dtostrf(voltage_secours,3,2,_buffer_secours);


	for(i=0; i<6; i++){
		
		if(_buffer_secours[i] == 0){
			
			_buffer_secours[i] = '0';
		}
	}
	
	return _buffer_secours;



}
/*
 * hal_adc.c
 *
 * Created: 14/06/2020 11:10:48
 *  Author: MICHEL GRANDA
 */ 

#include "hal_adc.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ADC_init(void){

		ADMUX  = (1 << REFS0);
		ADCSRB = 0x00;
		ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 

	


}




/************************************************************************/
/*                                                                      */
/************************************************************************/
int ADC_read(unsigned char channel){

	int valeur;

	DIDR0   =  ( 1 << channel);
	ADMUX  &= ~(0x0F);
	ADMUX  |= channel;
	ADCSRA |= (1 << ADEN); // on active le convertisseur
	ADCSRA |= (1 << ADSC); // on demarre la conversion
	while (!(ADCSRA & (1 << ADIF))); // on attend que la conversion termine
	
	valeur  = (int)(ADCL);
	valeur |= (int)(ADCH << 8);
	
	return valeur;

}

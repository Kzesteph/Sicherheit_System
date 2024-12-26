/*
 * detecteur_mouvement.h
 *
 * Created: 06/08/2020 14:46:03
 *  Author: MICHEL GRANDA
 */ 


#ifndef DETECTEUR_MOUVEMENT_H_
#define DETECTEUR_MOUVEMENT_H_


#include "hal_i2c.h"

void detecteur_mouvement_init(void);

void store_gyroscope(unsigned char *mouvement_position, int *gyroscope);

void get_mouvement(unsigned char *mouvement_position);

unsigned char mouvement_changed(int *old_gyroscope, int *new_gyroscope);



#endif /* DETECTEUR_MOUVEMENT_H_ */
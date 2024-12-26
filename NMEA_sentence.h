/*
 * NMEA_sentence.h
 *
 *  Created on: 29 mai 2020
 *      Author: michel_granda
 */

#ifndef NMEA_SENTENCE_H_
#define NMEA_SENTENCE_H_

#include"hal_port.h"

/**
 *
 */
typedef struct{




}GNGLL_sentence;

/**
 *
 */
typedef struct{

	unsigned char entete[7];		/// entete du message
	unsigned char time_utc[11];			/// UTC time, time of position
	unsigned char status[2];		/// status, V= navigationreceiver warning, A = data valid
	unsigned char latitude[11];		/// latitude, degrees+minutes
	unsigned char hemisphere_NS[2]; /// N/S indicator, hemisphere N=nord or S=sud
	unsigned char longitude[11];	/// longitude, degrees + minutes
	unsigned char hemisphere_EW[2]; /// E/W indicator, hemisphere E=est W=ouest
	unsigned char speed[10];		/// speed over ground
	unsigned char cog[10];			/// course over ground
	unsigned char date[7];			/// date in day month year format
	unsigned char mv[5];			/// magnetic variation value
	unsigned char mvE[2];			/// magnetic variation E/W indicator
	unsigned char mode[2];			/// mode indicator

}GNRMC_sentence;

/**
 *
 */
typedef struct{



}GNZDA_sentence;
#endif /* NMEA_SENTENCE_H_ */

/*
 * timeout_exception.h
 *
 *  Created on: 27 mai 2020
 *      Author: michel_granda
 */

#ifndef TIMEOUT_EXCEPTION_H_
#define TIMEOUT_EXCEPTION_H_

#include "hal_port.h"



/**
 *
 */
typedef enum{

	TIMER0=0,//!< TIMER0
	TIMER1,  //!< TIMER1
	TIMER2,  //!< TIMER2
	TIMER3   //!< TIMER3
}timer;


inline void timer_init(timer _timer_number){



}

inline void timer_interruption_enable(timer _timer_number, state _state){



}

inline void timer_interruption_flag_enable(timer _timer, state _state){


}

inline void timeout_enable(unsigned char timeout_number, state _state){


}

inline void timeout_reset(unsigned char timeout_number){


}


#endif /* TIMEOUT_EXCEPTION_H_ */

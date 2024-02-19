/*
 * my_tracker.h
 *
 *  Created on: Feb 19, 2024
 *      Author: mzeml
 */

#ifndef MY_TRACKER_H_
#define MY_TRACKER_H_

#include "my_global.h"

// ASTRO COMMANDS

// 0,X Command to set Ultra-Low power settings

// 1,X Command to set my_rtc_alarmA_time variable

// 9,X Sensitive command regarding contingency situations
#define SYS_RESET_CMD 				"9,9"	//HAL_NVIC_SystemReset



#endif /* MY_TRACKER_H_ */
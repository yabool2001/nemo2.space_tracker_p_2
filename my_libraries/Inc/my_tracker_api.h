/*
 * my_tracker.h
 *
 *  Created on: Feb 19, 2024
 *      Author: mzeml
 */

#ifndef MY_TRACKER_H_
#define MY_TRACKER_H_

#include "my_global.h"
#include "my_conversions.h"

#include "my_astronode.h"
#include "my_rtc.h"
#include "my_gnss.h"
#include "iis2dh_reg.h"

// ASTRO COMMANDS

// 0,X Command to set Ultra-Low power settings

// 1,X Command to set my_rtc_alarmA_time variable

// 9,X Sensitive command regarding contingency situations
#define 	GET_SYS_CFG 					1
#define 	RESET_SYS_CMD_VALUE 			1	//HAL_NVIC_SystemReset

bool		my_tracker_api_is_cmd ( const char* ) ;
uint8_t		my_tracker_api_get_cmd_code ( const char* ) ;
uint32_t	my_tracker_api_get_cmd_value ( const char* ) ;
bool		my_tracker_api_parse_cmd ( cmd_astro* , char* ) ;

#endif /* MY_TRACKER_H_ */

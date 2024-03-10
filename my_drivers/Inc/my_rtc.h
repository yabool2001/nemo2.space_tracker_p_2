/*
 * my_rtc.h
 *
 *  Created on: Oct 27, 2023
 *      Author: mzeml
 */

#ifndef MY_RTC_H_
#define MY_RTC_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "my_global.h"
#include "my_conversions.h"
#include "my_nmea.h"

extern RTC_HandleTypeDef hrtc ;

extern void		my_nmea_get_rmc_date_yy	( const char* , uint8_t* ) ;
extern void		my_nmea_get_rmc_date_mm	( const char* , uint8_t* ) ;
extern void		my_nmea_get_rmc_date_dd	( const char* , uint8_t* ) ;
extern void		my_nmea_get_rmc_utc_hh	( const char* , uint8_t* ) ;
extern void		my_nmea_get_rmc_utc_mm	( const char* , uint8_t* ) ;
extern void		my_nmea_get_rmc_utc_ss	( const char* , uint8_t* ) ;
extern void		my_nmea_get_rmc_utc_sss	( const char* , uint32_t* ) ;

void		my_rtc_get_dt ( RTC_DateTypeDef* d , RTC_TimeTypeDef* t ) ;
uint16_t	my_rtc_get_dt_s ( char* ) ;
bool		my_rtc_set_alarm ( uint32_t ) ;
void		my_rtc_set_dt_from_nmea_rmc ( const char* ) ;
uint32_t	my_rtc_get_ts ( void ) ;
void		send_debug_logs ( const char* ) ;

#endif /* MY_RTC_H_ */

/*
 * my_lx6_gnss.h
 *
 *  Created on: Oct 27, 2023
 *      Author: mzeml
 */

#ifndef MY_LX6_GNSS_H_
#define MY_LX6_GNSS_H_

#include <stdbool.h>
#include "my_global.h"
#include "my_nmea.h"
#include "my_rtc.h"

#define NMEA_3D_FIX						'3'
#define MY_GNSS_COORDINATE_MAX_SIZE		12 // 10 + ew. znak minus + '\0'
#define NMEA_FIX_PDOP_STRING_BUFF_SIZE	5

// Local functions
bool my_gnss_acq_coordinates ( fix_astro* ) ;
bool my_lx6_get_coordinates ( uint16_t , uint16_t , double , double* , int32_t* , int32_t* ) ;
extern void my_rtc_set_dt_from_nmea_gll ( const char* ) ;
extern void my_rtc_set_dt_from_nmea_rmc ( const char* ) ;
bool my_gnss_get_utc () ;
void my_gnss_log ( uint16_t ) ;

#endif /* MY_LX6_GNSS_H_ */

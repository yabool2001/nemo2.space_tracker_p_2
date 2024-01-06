/*
 * my_lx6_gnss.h
 *
 *  Created on: Oct 27, 2023
 *      Author: mzeml
 */

#ifndef MY_LX6_GNSS_H_
#define MY_LX6_GNSS_H_

#include <stdbool.h>
#include "my_nmea.h"

#define NMEA_3D_FIX						'3'
#define NMEA_MESSAGE_SIZE				250
#define MY_GNSS_NMEA_MAX_SIZE			12 // 10 + ew. znak minus + '\0'
#define MY_GNSS_MIN_TNS					3 // Minimalna ilość satelitów
#define MY_GNSS_MIN_TNS_TIME_THS		10 // Czas w jakim powinno być co najmniej MY_GNSS_NMEA_GSV_MIN_TNS satelitów
#define NMEA_FIX_PDOP_STRING_BUFF_SIZE	5

void my_gnss_receive_byte ( uint8_t* , bool ) ;
void send_debug_logs ( char* ) ;
bool my_lx6_get_coordinates ( uint16_t , uint16_t , double , double* , int32_t* , int32_t* ) ;
void my_rtc_set_dt_from_nmea_rmc ( const char* ) ;

#endif /* MY_LX6_GNSS_H_ */

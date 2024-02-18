/*
 * my_global.c
 *
 *  Created on: Jan 7, 2024
 *      Author: mzeml
 */

#include "my_global.h"

// GNSS
uint16_t	utc_acq_ths = FIX_ACQ_THS ;
uint16_t	fix_acq_ths = TIME_THS_2_MIN ;
uint16_t	min_tns_time_ths = TIME_THS_30_SEC ; // Czas w jakim powinno być co najmniej MY_GNSS_NMEA_GSV_MIN_TNS satelitów
double		pdop_ths = PDOP_THS ;
uint8_t		fix3d_flag = false ;
bool		rcv_cmd_flag = false ;

//ASTRO
char my_astro_rcv_cmd[40] = {0} ;

// TIM
uint16_t	tim_seconds = 0 ;

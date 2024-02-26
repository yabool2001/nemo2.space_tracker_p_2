/*
 * my_global.c
 *
 *  Created on: Jan 7, 2024
 *      Author: mzeml
 */

#include "my_global.h"

//SYS
char		dbg_payload[UART_TX_MAX_BUFF_SIZE] = {0} ;

// GNSS
uint16_t	fix_acq_ths = TIME_THS_2_MIN ;
uint16_t	min_tns_time_ths = TIME_THS_30_SEC ; // Czas w jakim powinno być co najmniej MY_GNSS_NMEA_GSV_MIN_TNS satelitów
double		pdop_ths = PDOP_THS ;
bool		astro_rcv_cmd_flag = false ;

//ASTRO
char 		my_astro_rcv_cmd[40] = {0} ;

// TIM
uint16_t	tim_seconds = 0 ;

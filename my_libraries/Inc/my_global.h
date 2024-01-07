/*
 * my_global.h
 *
 *  Created on: Jan 7, 2024
 *      Author: mzeml
 */

#ifndef MY_GLOBAL_H_
#define MY_GLOBAL_H_

#include <stdbool.h>

#include "stm32g0xx_hal.h"


#define UART_TX_MAX_BUFF_SIZE		250
#define PDOP_THS					5.1
#define UTC_ACQ_THS					30
#define FIX_ACQ_THS					240 // produkcyjnie ustawić na 120
#define MIN_TNS						3 // Minimalna ilość satelitów
#define MIN_TNS_TIME_THS			10 // Czas w jakim powinno być co najmniej MY_GNSS_NMEA_GSV_MIN_TNS satelitów

// GNSS
extern uint16_t		utc_acq_ths ;
extern uint16_t		fix_acq_ths ;
extern uint16_t		min_tns_time_ths ;
extern double		pdop_ths ;
extern uint8_t		fix3d_flag ;
typedef struct
{
	int32_t	latitude_astro_geo_wr ;
	int32_t	longitude_astro_geo_wr ;
	double 	pdop ;
} fix_astro ;

// ASTRO


// TIM
extern uint16_t		tim_seconds ;

void send_debug_logs ( char* ) ;
void my_gnss_receive_byte ( uint8_t* , bool ) ;
bool is_fix3d () ;

#endif /* MY_GLOBAL_H_ */

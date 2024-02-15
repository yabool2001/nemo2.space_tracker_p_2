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
#define FIX_ACQ_THS					360 // produkcyjnie ustawić na 120
#define MIN_TNS						3 // Minimalna ilość satelitów
#define MIN_TNS_TIME_THS			120 // Czas w jakim powinno być co najmniej MY_GNSS_NMEA_GSV_MIN_TNS satelitów

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
	char	fix_mode ;
} fix_astro ;

// ASTRO


// TIM
void my_tim_start ( void ) ;
void my_tim_stop ( void ) ;
extern uint16_t		tim_seconds ;


void my_gnss_receive_byte ( uint8_t* , bool ) ;
bool is_fix3d () ;

// Astronode
void send_debug_logs ( char* ) ;
void my_astronode_reset ( void ) ;
void send_astronode_request ( uint8_t* , uint32_t ) ;
uint32_t get_systick ( void ) ;
bool is_systick_timeout_over ( uint32_t , uint16_t ) ;
bool is_astronode_character_received ( uint8_t* ) ;


#endif /* MY_GLOBAL_H_ */

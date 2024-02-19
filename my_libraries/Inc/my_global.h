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

#define TIME_THS_30_SEC				30
#define TIME_THS_1_MIN				60
#define TIME_THS_2_MIN				120
#define TIME_THS_5_MIN				300
#define TIME_THS_10_MIN				600
#define TIME_THS_15_MIN				900 // Nic nie powinno tego przekraczać, bo to jest okres watchdog
#define TIME_THS_1_H				3600

#define UART_TX_MAX_BUFF_SIZE		250
#define PDOP_THS					2
#define FIX_ACQ_THS					600 // produkcyjnie ustawić na 120
#define MIN_TNS						3 // Minimalna ilość satelitów

// GNSS
extern uint16_t		utc_acq_ths ;
extern uint16_t		fix_acq_ths ;
extern uint16_t		min_tns_time_ths ;
extern double		pdop_ths ;
extern uint8_t		fix3d_flag ;
extern bool			astro_rcv_cmd_flag ;
typedef struct
{
	int32_t		latitude_astro_geo_wr ;
	int32_t		longitude_astro_geo_wr ;
	double 		pdop ;
	char		fix_mode ;
	uint16_t	acq_time ;
	uint32_t	acq_total_time ;
} fix_astro ;

// ASTRO
extern char my_astro_rcv_cmd[40] ;

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

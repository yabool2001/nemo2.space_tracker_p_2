/*
 * my_global.c
 *
 *  Created on: Jan 7, 2024
 *      Author: mzeml
 */

#include "my_global.h"

// GNSS
uint16_t	utc_acq_ths = FIX_ACQ_THS ;
uint16_t	fix_acq_ths = FIX_ACQ_THS ;
uint16_t	min_tns_time_ths = MIN_TNS_TIME_THS ;
double		pdop_ths = PDOP_THS ;
uint8_t		fix3d_flag = false ;


// TIM
uint16_t	tim_seconds = 0 ;

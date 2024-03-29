/*
 * my_astronode.h
 *
 *  Created on: Oct 23, 2023
 *      Author: mzeml
 */

#ifndef ASTROCAST_INC_MY_ASTRONODE_H_
#define ASTROCAST_INC_MY_ASTRONODE_H_

#include <string.h>
#include "my_global.h"
#include "astronode_application.h"
#include "astronode_definitions.h"
#include "astronode_transport.h"


bool my_astro_init ( void ) ;
void my_astro_write_coordinates ( int32_t , int32_t ) ;
bool my_astro_add_payload_2_queue ( uint16_t , const char* ) ;
bool my_astro_handle_evt ( void ) ;
bool my_astro_log ( void ) ;
void send_debug_logs ( const char* ) ;

#endif /* ASTROCAST_INC_MY_ASTRONODE_H_ */

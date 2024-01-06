/*
 * my_astronode.c
 *
 *  Created on: Oct 23, 2023
 *      Author: mzeml
 */
#include "my_astronode.h"


bool my_astro_add_payload_2_queue ( uint16_t id , char* payload )
{
	size_t l = strlen ( payload ) ;
	if ( l <= ASTRONODE_APP_PAYLOAD_MAX_LEN_BYTES )
	{
		if ( astronode_send_pld_er ( id , payload , l ) )
		{
			return true ;
		}
	}
	else
	{
		send_debug_logs ( "my_astronode.c,my_astro_add_payload_2_queue,Payload exceeded ASTRONODE_APP_PAYLOAD_MAX_LEN_BYTES value." ) ;
	}
	return false ;
}
bool my_astro_read_evt_reg ( void )
{
	send_debug_logs ( "my_astronode.c,my_astro_read_evt_reg,evt pin is high." ) ;
	astronode_send_evt_rr () ;
	if (is_sak_available () )
	{
	  astronode_send_sak_rr () ;
	  astronode_send_sak_cr () ;
	  send_debug_logs ( "my_astronode.c,my_astro_read_evt_reg,message has been acknowledged." ) ;
	  //astronode_send_per_rr () ;
	}
	if ( is_astronode_reset () )
	{
	  send_debug_logs ( "my_astronode.c,my_astro_read_evt_reg,terminal has been reset." ) ;
	  astronode_send_res_cr () ;
	}
	if ( is_command_available () )
	{
	  send_debug_logs ( "my_astronode.c,my_astro_read_evt_reg,unicast command is available" ) ;
	  astronode_send_cmd_rr () ;
	  astronode_send_cmd_cr () ;
	}
	return true ;
}
bool my_astro_log ( void )
{
	astronode_send_rtc_rr ();
	astronode_send_nco_rr () ;
	//astronode_send_lcd_rr () ;
	//astronode_send_end_rr () ;
	//astronode_send_per_rr () ;
	return true ;
}

void my_astro_write_coordinates ( int32_t astro_geo_wr_latitude , int32_t astro_geo_wr_longitude )
{
	astronode_send_geo_wr ( astro_geo_wr_latitude , astro_geo_wr_longitude ) ;
}



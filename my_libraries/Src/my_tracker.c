/*
 * my_tracker.c
 *
 *  Created on: Feb 19, 2024
 *      Author: mzeml
 */


my_tracker_get_cmd ( c* )
{
	if ( strstr ( my_astro_rcv_cmd , (char*) SYS_RESET_CMD ) )
	{
	  my_rtc_get_dt_s ( rtc_dt_s ) ;
	  sprintf ( dbg_payload , "%s,%d,%s,HAL_NVIC_SystemReset" , __FILE__ , __LINE__ , rtc_dt_s ) ;
	  send_debug_logs ( dbg_payload ) ;
	  HAL_NVIC_SystemReset () ;
	}
}

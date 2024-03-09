/*
 * my_st_acc.c
 *
 *  Created on: Mar 9, 2024
 *      Author: mzeml
 */
#include "my_st_acc.h"

stmdev_ctx_t 	my_acc_ctx ;

uint16_t my_st_acc_health_flag = 1 ;

bool my_st_iis2dh_init ( void )
{
	uint8_t id = 0 ;

	iis2dh_device_id_get ( &my_acc_ctx , &id ) ;
	if ( id != IIS2DH_ID )
		return false ;

	iis2dh_full_scale_set ( &my_acc_ctx , IIS2DH_2g ) ;
	iis2dh_operating_mode_set ( &my_acc_ctx , IIS2DH_LP_8bit ) ;
	iis2dh_data_rate_set ( &my_acc_ctx , IIS2DH_ODR_10Hz ) ;

	return true ;
}

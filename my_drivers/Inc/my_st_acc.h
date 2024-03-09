/*
 * my_st_acc.h
 *
 *  Created on: Mar 9, 2024
 *      Author: mzeml
 */

#ifndef MY_ST_ACC_H_
#define MY_ST_ACC_H_

#include <stdbool.h>

#include "iis2dh_reg.h"


extern stmdev_ctx_t my_acc_ctx ;
extern uint16_t 	my_st_acc_health_flag ;

bool my_st_iis2dh_init ( void ) ;

#endif /* MY_ST_ACC_H_ */

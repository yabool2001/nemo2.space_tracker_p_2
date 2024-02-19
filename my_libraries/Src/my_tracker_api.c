/*
 * my_tracker.c
 *
 *  Created on: Feb 19, 2024
 *      Author: mzeml
 */

#include <my_tracker_api.h>

bool my_tracker_api_is_cmd ( char* c )
{
	size_t l = strlen ( c ) ;
	uint8_t i = 0 ; // Znaków nie może być więcej niż max. cmd payload czyli 80
	uint8_t liczba_przecinkow = 0 ;

	if ( l < 3 || l > 8 )
		return false ;



	for ( i = 0 ; i < l ; i++ )
	{
		if ( c[i] == ',' )
		{
			liczba_przecinkow++ ;
		}
		else if ( c[i] < '0' || c[i] > '9' )
		{
			return false;
	    }
	}

	if ( c[0] == ',' || c[l-1] == ',' || liczba_przecinkow > 1 )
	{
		return false ;
	}

	return true ;
}

uint8_t my_tracker_api_get_cmd_code ( char* c )
{
	char* tmp ;
	uint8_t cmd = 0 ;

	tmp = strtok_r ( c , "," , &c ) ;

	return cmd ;
}

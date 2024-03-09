/*
 * my_tracker.c
 *
 *  Created on: Feb 19, 2024
 *      Author: mzeml
 */

#include <my_tracker_api.h>



bool my_tracker_api_is_cmd ( const char* c )
{
	size_t l = strlen ( c ) ;
	uint8_t i = 0 ; // Znaków nie może być więcej niż max. cmd payload czyli 80
	uint8_t liczba_przecinkow = 0 ;

	if ( l < 3 || l > 9 )
		return false ;

	for ( i = 0 ; i < l ; i++ )
	{
		if ( c[i] == ',' )
			liczba_przecinkow++ ;
		else if ( c[i] < '0' || c[i] > '9' )
			return false;
	}

	if ( /*c[0] == ',' || c[l-1] == ',' ||*/ liczba_przecinkow > 1 )
		return false ;

	const char* comma_p = strchr ( c , ',' ) ;
	size_t cmd_code_length = comma_p - c ;
	if ( cmd_code_length > 2 || cmd_code_length < 1 )
		return false ;

	return true ;
}

uint8_t my_tracker_api_get_cmd_code ( const char* c )
{
	const char* comma_p = strchr ( c , ',' ) ;
	size_t cmd_code_length = comma_p - c ;

	char* cmd_code_s = (char*) malloc ( ( cmd_code_length + 1 ) * sizeof ( char ) ) ;
	strncpy ( cmd_code_s , c , cmd_code_length ) ; // Kopiowanie fragmentu łańcucha
	cmd_code_s[cmd_code_length] = '\0';
	uint8_t cmd_code = (uint8_t) my_conv_string_2_uint32_t ( cmd_code_s ) ;
	free ( cmd_code_s ) ;

	return cmd_code ;
}

uint32_t my_tracker_api_get_cmd_value ( const char* c )
{
	const char* comma_p = strchr ( c , ',' ) ;
	size_t cmd_value_length = strlen ( comma_p + 1 ) ;

	char* cmd_value_s = (char*) malloc ( ( cmd_value_length + 1 ) * sizeof ( char ) ) ;
	strncpy ( cmd_value_s , comma_p + 1 , cmd_value_length ) ; // Kopiowanie fragmentu łańcucha
	cmd_value_s[cmd_value_length] = '\0';
	uint32_t cmd_value = my_conv_string_2_uint32_t ( cmd_value_s ) ;
	free ( cmd_value_s ) ;

	return cmd_value ;
}

bool my_tracker_api_parse_cmd ( cmd_astro* cmd , char* s )
{
	cmd->code = 0 ;
	cmd->value = 0 ;
	cmd->is_executed = false ;

	char* tok = strtok ( s , "," ) ;
	cmd->code = (uint8_t) my_conv_string_2_uint32_t ( tok ) ;
	tok = strtok ( NULL , "," ) ;
	cmd->value = my_conv_string_2_uint32_t ( tok ) ;
	tok = strtok ( NULL , "," ) ;

	if ( cmd->code == 0 || tok != NULL )
		return false ;
	else
		return true ;
}

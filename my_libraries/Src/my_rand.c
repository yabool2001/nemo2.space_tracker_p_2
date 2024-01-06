/*
 * my_rand.c
 *
 *  Created on: Dec 3, 2023
 *      Author: mzeml
 */


#include "stm32g0xx_hal.h"
#include <stdlib.h>

void my_rand_get_coordinates ( double* last_latitude_astro_geo_wr , double* last_longitude_astro_geo_wr , double* latitude_astro_geo_wr , double* longitude_astro_geo_wr )
{

    // Wartości współrzędnych
    double last_lat = *last_latitude_astro_geo_wr ;
    double last_lon = *last_longitude_astro_geo_wr ;

    // Ziarno dla generatora liczb losowych
    srand ( HAL_GetTick () ) ;

    // Losowa odległość w zakresie 0-100 km (jako różnica współrzędnych)
    double delta_lat = ( ( double ) rand () / RAND_MAX - 0.5 ) * 1 ;  // zakres: -0.1 do 0.1
    double delta_lon = ( ( double ) rand () / RAND_MAX - 0.5 ) * 1 ;  // zakres: -0.1 do 0.1

    // Nowe współrzędne
    *latitude_astro_geo_wr = last_lat + delta_lat;
    *longitude_astro_geo_wr = last_lon + delta_lon;

    // Sprawdź, czy współrzędne są w zakresie poprawnym dla półkul
    if ( *latitude_astro_geo_wr > 90.0 || *latitude_astro_geo_wr < -90.0 )
    {
        // Poza zakresem dla półkuli północnej i południowej, więc odwróć znak
        *latitude_astro_geo_wr = -*latitude_astro_geo_wr;
    }

    // Zakres dla współrzędnych geograficznych to -180 do 180
    if ( *longitude_astro_geo_wr > 180.0 )
    {
        *longitude_astro_geo_wr = *longitude_astro_geo_wr - 360.0 ;
    }
    else if ( *longitude_astro_geo_wr < -180.0 )
    {
        *longitude_astro_geo_wr = *longitude_astro_geo_wr + 360.0 ;
    }
}

void my_rand_get_coordinates_old ( int32_t* last_latitude_astro_geo_wr , int32_t* last_longitude_astro_geo_wr , int32_t* latitude_astro_geo_wr , int32_t* longitude_astro_geo_wr )
{

    // Wartości współrzędnych przemnożone przez 10000000
    int32_t last_lat = *last_latitude_astro_geo_wr ;
    int32_t last_lon = *last_longitude_astro_geo_wr ;

    // Losowa odległość w zakresie 0-100 km (jako różnica współrzędnych)
    int32_t delta_lat = ( rand() % 100000 - 50000 ) * 10 ;  // zakres: -50000 do 49999
    int32_t delta_lon = ( rand() % 100000 - 50000 ) * 10 ;  // zakres: -50000 do 49999

    // Nowe współrzędne
    *latitude_astro_geo_wr = last_lat + delta_lat;
    *longitude_astro_geo_wr = last_lon + delta_lon;

    // Sprawdź, czy współrzędne są w zakresie poprawnym dla półkul
    if ( *latitude_astro_geo_wr > 900000000 || *latitude_astro_geo_wr < -900000000 )
    {
        // Poza zakresem dla półkuli północnej i południowej, więc odwróć znak
        *latitude_astro_geo_wr = -*latitude_astro_geo_wr;
    }

    // Zakres dla współrzędnych geograficznych to -1800000000 do 1800000000
    // Zakres dla współrzędnych geograficznych w kodzie zakodowanym * 10000000
    if ( *longitude_astro_geo_wr > 1800000000 )
    {
        *longitude_astro_geo_wr = *longitude_astro_geo_wr - 3600000000 ;
    }
    else if ( *longitude_astro_geo_wr < -1800000000 )
    {
        *longitude_astro_geo_wr = *longitude_astro_geo_wr + 3600000000 ;
    }
    // Ogranicz do 8 cyfr
	*latitude_astro_geo_wr %= 100000000 ;
	*longitude_astro_geo_wr %= 100000000 ;
    *last_latitude_astro_geo_wr = *latitude_astro_geo_wr ; *last_longitude_astro_geo_wr = *longitude_astro_geo_wr ;
}

/* Inna wersja
void generate_random_coordinates(int32_t* last_latitude_astro_geo_wr, int32_t* last_longitude_astro_geo_wr,
                                 int32_t* latitude_astro_geo_wr, int32_t* longitude_astro_geo_wr) {
    // Deklaracja zmiennych pomocniczych
    int32_t latitude_offset, longitude_offset;

    // Generowanie losowych offsetów w zakresie -1000000 do 1000000 (od -10 km do 10 km)
    latitude_offset = (rand() % 2000001) - 1000000;
    longitude_offset = (rand() % 2000001) - 1000000;

    // Dodawanie offsetów do ostatnich współrzędnych
    *latitude_astro_geo_wr = *last_latitude_astro_geo_wr + latitude_offset;
    *longitude_astro_geo_wr = *last_longitude_astro_geo_wr + longitude_offset;

    // Aktualizacja ostatnich współrzędnych
    *last_latitude_astro_geo_wr = *latitude_astro_geo_wr;
    *last_longitude_astro_geo_wr = *longitude_astro_geo_wr;

    // Uwzględnianie półkuli północnej, południowej, wschodniej i zachodniej
    if (*latitude_astro_geo_wr > 900000000 || *latitude_astro_geo_wr < -900000000) {
        // Jeśli przekroczyło półkule północną lub południową, odwróć znak
        *latitude_astro_geo_wr = -*latitude_astro_geo_wr;
    }

    if (*longitude_astro_geo_wr > 1800000000 || *longitude_astro_geo_wr < -1800000000) {
        // Jeśli przekroczyło półkule wschodnią lub zachodnią, odwróć znak
        *longitude_astro_geo_wr = -*longitude_astro_geo_wr;
    }
}
*/

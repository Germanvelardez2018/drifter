
/**
 * @file SIM7000G.h
 * @author German Velardez (gvelardez@inti.gob.ar)
 * @brief  Modulo para el manejo del modulo de comunicacion SIM7000G
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SIM7000G_H
#define __SIM7000G_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "core.h"
/* Definiciones --------------------------------------------------------------*/


/*-----------------------------------------------------------------------------*/



// Nota POWERKEY MODIFICADO PB13 A PA12...CAMBIARLO EN SOFTWARE







status_t sim7000g_check();


status_t sim7000g_init();


status_t sim7000g_set_mqtt_config();



status_t sim7000g_mqtt_publish();

status_t sim7000g_set_mqtt_subscribe();

status_t sim7000g_set_mqtt_dessubscribe();



status_t sim7000g_resume();


status_t sim7000g_sleep();




status_t sim7000g_get_NMEA( uint8_t* buffer, uint8_t len);



status_t sim7000g_test();



#ifdef __cplusplus
}
#endif

#endif /* __SIM7000G_H */


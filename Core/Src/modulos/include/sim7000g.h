
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

uint8_t sim7000g_get_parse(char* string);



void sim_copy_buffer_cmd(uint8_t* buffer);

void sim_get_values(char* buffer,int8_t* interval, uint8_t* max);

uint8_t* sim_get_buffer_cmd();

uint8_t sim_get_update_params(void);


void sim_set_update_params(uint8_t value);




void check_buffer_cmd();

uint8_t* sim_get_id(void );

 uint8_t* sim7000g_get_interval();
 uint8_t* sim7000g_get_max();



uint8_t sim7000g_get_flag();




status_t sim7000g_init();




status_t sim7000g_resume();


status_t sim7000g_sleep();


status_t sim7000g_get_operator();


status_t sim7000g_set_gps(uint8_t value);


status_t sim7000g_get_NMEA( uint8_t* buffer, uint8_t len);

status_t sim7000g_set_mqtt_config(uint8_t* url, uint8_t* user, uint8_t* password, uint8_t* qos);



status_t sim7000g_mqtt_publish(uint8_t* topic, uint8_t* payload, uint8_t len_payload);

status_t sim7000g_mqtt_subscription(uint8_t* topic);

status_t sim7000g_mqtt_unsubscription(uint8_t* topic);



status_t sim7000g_mqtt_check();

status_t sim7000g_test();

status_t sim7000g_get_signal();


status_t sim7000g_open_apn();



status_t sim7000g_read_input(uint8_t len, uint32_t timeout);

uint8_t* sim7000g_get_buffer();




#ifdef __cplusplus
}
#endif

#endif /* __SIM7000G_H */


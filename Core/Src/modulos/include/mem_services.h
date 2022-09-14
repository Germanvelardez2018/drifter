/**
 * @file mem_services.h
 * @author German Velardez (gvelardez@inti.gob.ar)
 * @brief  Brinda los servicios de alto nivel en cuanto a manejo de memoria externa
 * @version 0.1
 * @date 2022-09-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MEM_SERVICES_H
#define __MEM_SERVICES_H



#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "core.h"

/* Definiciones --------------------------------------------------------------*/


/*-----------------------------------------------------------------------------*/

/**
 * @brief S
 * 
 * @return ** void 
 */
status_t mem_s_init();


// Interval
status_t mem_s_get_interval(uint32_t* interval);
status_t mem_s_set_interval(uint32_t* interval);


status_t mem_s_get_max_amount_data(uint32_t* max_amount_data);
status_t mem_s_set_max_amount_data(uint32_t* max_amount_data);


status_t mem_s_get_battery_state(battery_state_t* battery_state);
status_t mem_s_set_battery_state(battery_state_t* battery_state);


status_t mem_s_get_max_accelerometer_offset(int16_t* x,int16_t* y, int16_t* z );
status_t mem_s_set_max_accelerometer_offset(int16_t* x, int16_t* y,int16_t* z);


status_t mem_s_get_max_data(uint8_t* data_buffer);
status_t mem_s_set_max_data(uint8_t* data_buffer);




#ifdef __cplusplus
}
#endif

#endif /* __MEM_SERVICES_H */

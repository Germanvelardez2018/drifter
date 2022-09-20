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




status_t mem_s_get_counter(uint8_t* counter);
status_t mem_s_set_counter(uint8_t* counter);



// Interval
status_t mem_s_get_interval(uint8_t* interval);
status_t mem_s_set_interval(uint8_t* interval);


status_t mem_s_get_max_amount_data(uint8_t* max_amount_data);
status_t mem_s_set_max_amount_data(uint8_t* max_amount_data);


status_t mem_s_get_battery_state(battery_state_t* battery_state);
status_t mem_s_set_battery_state(battery_state_t* battery_state);


status_t mem_s_get_max_accelerometer_offset(int16_t* x,int16_t* y, int16_t* z );
status_t mem_s_set_max_accelerometer_offset(int16_t* x, int16_t* y,int16_t* z);



// Los datos se guardan como lifo
status_t mem_s_load_data(uint8_t* string );


// Para leer todos los datos almacenados recomienda usar dentro 
// de un bucle while con condicion (ret = mem_s_download_data(buffer) != STATUS__ERROR)
status_t mem_s_download_data(uint8_t* string);






status_t write_data(uint8_t* buffer, uint32_t page);






status_t read_data(uint8_t* buffer, uint32_t page);



#ifdef __cplusplus
}
#endif

#endif /* __MEM_SERVICES_H */

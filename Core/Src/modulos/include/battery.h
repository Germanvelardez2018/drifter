/**
 * @file BATTERY.h
 * @author German Velardez(gvelardez@inti.gob.ar)
 * @brief  Archivo de configuracion del periferico BATTERY
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */




/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BATTERY_H
#define __BATTERY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "core.h"
#include "adc.h"


/*-----------------------------------------------------------------------------*/


 void battery_init(void);


/**
 * 
 * **/
status_t battery_check_status(uint8_t* buffer,uint8_t max_len);








#ifdef __cplusplus
}
#endif

#endif /* __BATTERY_H */


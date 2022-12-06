/**
 * @file power_save.h
 * @author German Velardez (gvelardez@inti.gob.ar)
 * @brief  Manejo de modos de bajo consumo en micros
 * @version 0.1
 * @date 2022-09-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */







/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __POWER_SAVE_H
#define __POWER_SAVE_H



#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "core.h"

/* Definiciones --------------------------------------------------------------*/

typedef enum{
    SLEEP = 0,
    RUN   = 1
} pwr_mode_t;

/*-----------------------------------------------------------------------------*/



pwr_mode_t pwr_get_mode();


// Inicio el modulo, ponggo el rtc en 0:0:0
void pwr_init();


// mando a dormir y me levanto un intervalo despues
void pwr_sleep(uint8_t option);

#ifdef __cplusplus
}
#endif

#endif /* __POWER_SAVE_H */





















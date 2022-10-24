/**
 * @file watchdog.h
 * @author German Velardez  (gvelardez@inti.gob.ar)
 * @brief  Archivo de configuracion del Watch dog timer
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WDT_H
#define __WDT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "core.h"


/*-----------------------------------------------------------------------------*/

/**
 * @brief Configura el hardware WDT. Tiene reloj aparte y esta configurado para maximo tiempo de espera disponible.
 * 
 * @return ** void 
 */
void MX_IWDG_Init(void);



void wdt_reset();



#ifdef __cplusplus
}
#endif

#endif /* __WDT_H */






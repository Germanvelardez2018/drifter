/**
 * @file rtc.h
 * @author German Velardez (gvelardez@ginti.gob.ar)
 * @brief  Archivos de configuracion del reloj de tiempo real
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */





/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "core.h"


/*-----------------------------------------------------------------------------*/



/**
 * @brief Configura el hardware de reloj de tiempo real
 * 
 * @return ** void 
 */
void MX_RTC_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H */




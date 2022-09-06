/**
 * @file system_clock.h
 * @author German Velardez(gvelardez@inti.gob.ar)
 * @brief  Archivo de configuracion del clocks
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */




/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTEM_CLOCK_H
#define __SYSTEM_CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "core.h"


/*-----------------------------------------------------------------------------*/



/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void);
#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_CLOCK_H */

/**
 * @file dma.h
 * @author German Velardez(gvelardez@inti.gob.ar)
 * @brief  Archivo de configuracion del periferico DMA. Usado en SPI y en UART DEBUG
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */




/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DMA_H
#define __DMA_H

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

#endif /* __DMA_H */





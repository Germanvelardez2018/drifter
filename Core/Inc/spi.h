/**
 * @file spi.h
 * @author German Velardez (gvelardez@inti.gob.ar)
 * @brief  Archivo de configuracion de uso de periferico SPI
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */







/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H
#define __SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "core.h"

/*-----------------------------------------------------------------------------*/



/**
 * @brief Configura el hardware del SPI1 . DMA normal mode e interrupciones (agregado manualmente, no MXCube)
 * 
 * @return ** void 
 */
void SPI_init(void);


void SPI_deinit(void);









#ifdef __cplusplus
}
#endif

#endif /* __SPI_H */

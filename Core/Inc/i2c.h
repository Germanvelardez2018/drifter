/**
 * @file i2c.h
 * @author German Velardez(gvelardez@inti.gob.ar)
 * @brief  Archivo de configuracion del periferico I2C
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */









/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "core.h"


/*-----------------------------------------------------------------------------*/

/**
 * @brief Configura el hardware del I2C2. Modo simple sin DMA y sin interrupciones
 * 
 * @return ** void 
 */
void I2C_init(void);

/**
 * @brief Desconfigura el hardware del I2C2. 
 * 
 * @return ** void 
 */
void I2C_deinit(void);


#ifdef __cplusplus
}
#endif

#endif /* __I2C_H */



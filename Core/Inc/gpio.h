/**
 * @file gpio.h
 * @author German Velardez (gvelardez@inti.gob.ar)
 * @brief  Archivo de configuracion del GPIO
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */






/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "core.h"


/*-----------------------------------------------------------------------------*/

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void GPIO_init(void);

void GPIO_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H */





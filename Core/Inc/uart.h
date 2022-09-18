/**
 * @file usart.h
 * @author German Velardez (gvelardez@inti.gob.ar)
 * @brief  Archivo de configuracion de Perifericos UART.
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */


 void MX_USART1_UART_Init(void);
 void MX_USART2_UART_Init(void);




 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER_H
#define __TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "core.h"


/*-----------------------------------------------------------------------------*/




/**
 * @brief  Configura el hardware del uart 1. Simple , sin dma ni interrupciones.
 * 
 * @return ** void 
 */
 void UART_SIMCOM_init(void);


/**
 * @brief Desconfigura el hardware
 * 
 * @return ** void 
 */
  void UART_SIMCOM_deinit(void);

 
 /**
  * @brief Configura el hardware del UART 2. Activando DMA e interrupciones. Leer NOTAS EN uart.c
  * 
  * @return ** void 
  */
 void UART_DEBUG_init(void);


/**
 * @brief  Desconfiguro el hardware
 * 
 * @return ** void 
 */
  void UART_DEBUG_deinit(void);





#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H */
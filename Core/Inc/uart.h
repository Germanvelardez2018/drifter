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



/*-----------------------------------------------------------------------------*/




/**
 * @brief  Configura el hardware del uart 1. Simple , sin dma ni interrupciones.
 * 
 * @return ** void 
 */
 void MX_USART1_UART_Init(void);
 
 /**
  * @brief Configura el hardware del UART 2. Activando DMA e interrupciones. Leer NOTAS EN uart.c
  * 
  * @return ** void 
  */
 void MX_USART2_UART_Init(void);




#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H */
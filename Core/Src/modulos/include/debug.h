/**
 * @file debug.h
 * @author German Velardez (gvelardez@inti.gob.ar)
 * @brief  Modulo para debug mediante impresion de mensaje por uart. Usa DMA
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEBUG_H
#define __DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Definiciones --------------------------------------------------------------*/


/*-----------------------------------------------------------------------------*/


void  modulo_debug_init(void);

void modulo_debug_print(char* string);


#ifdef __cplusplus
}
#endif

#endif /* __DEBUG_H */



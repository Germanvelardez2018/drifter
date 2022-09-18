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

/**
 * @brief  Inicio el modulo para imprimir mensaje de debug
 * 
 * @return ** void 
 */
void  modulo_debug_init(void);


/**
 * @brief Imprimo un mensaje de debug
 * 
 * @param string array de char, tiene que ser un string con /0 al final.
 * @return ** void 
 */
void modulo_debug_print(char* string);


#ifdef __cplusplus
}
#endif

#endif /* __DEBUG_H */



/**
 * @file fsm.h
 * @author German Velardez (gvelardez@inti.gob.ar)
 * @brief  Maquina de estado para el dispositivo
 * @version 0.1
 * @date 2022-09-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FSM_H
#define __FSM_H



#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "core.h"

/* Definiciones --------------------------------------------------------------*/

typedef enum{
   FSM_ERROR            = -1
 , FSM_UNDEFINED        = 0
 , FSM_ON_FIELD         = 1     //! Transmicion desde campo
 , FSM_MEMORY_DOWNLOAD  = 2     //! Transmicion desde memoria

} fsm_state_t;


typedef void (*callback_function)(void);

/*-----------------------------------------------------------------------------*/


/**
 * @brief 
 *  Agrego las funciones que necesito 
 *  que se ejecuten segun estado de maquina de estado.
 *   
 * @param on_field         funcion que se ejecuta si estamos en campo tomando mediciones
 * @param memory_download  funcion que se ejecuta si estamos online enviando datos
 * @return ** status_t 
 */
status_t fsm_set_callbacks( callback_function on_field,
                            callback_function memory_download);


/**
 * @brief  Entramos en loop infinito
 * 
 * @return ** void 
 */
void fsm_loop();


/**
 * @brief SIEMPRE INICIAMOS EN MODO SIN_CONFIGURACION,
 *  LOS DEMAS MODOS SE ACCEDEN DESDE LA MEMORIA FLASH
 * 
 * @return ** void 
 */
status_t fsm_init();


/**
 * @brief 
 * Cambio el estado en flash 
 * Si el proceso de guardado en flash fue correcto,
 * actualizo estado en sram
 * 
 * @param new_state 
 * @return ** status_t 
 */
status_t fsm_set_state( fsm_state_t new_state);



/**
 * @brief 
 *  Devuelvo estado en SRAM. 
 *  Debe coincidir con estado en flash.
 * 
 * @return **  fsm_state_t 
 */
 fsm_state_t fsm_get_state(void);




#ifdef __cplusplus
}
#endif

#endif /* __FSM_H */








































/**
 * @file time.h
 * @author German Velardez (gvelardez@inti.gob.ar)
 * @brief  Timer para interrupciones periodica.s
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */





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
 * @brief Configura el haardware asociado a el timer TIM4 de 16 bits. Simple
 * 
 * @return ** void 
 */
 void MX_TIM1_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H */






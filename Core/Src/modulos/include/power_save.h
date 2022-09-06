/**
 * @file power_save.h
 * @author German Velardez (gvelardez@inti.gob.ar)
 * @brief  Manejo de modos de bajo consumo en micros
 * @version 0.1
 * @date 2022-09-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */







/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __POWER_SAVE_H
#define __POWER_SAVE_H



#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "core.h"

/* Definiciones --------------------------------------------------------------*/



typedef enum{
    RUN_MODE = 0,
    SLEEP_MODE = 1
} pws_mode_t;




/*-----------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* __POWER_SAVE_H */





















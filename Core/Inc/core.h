/**
 * @file core.h
 * @author German Velardez (gvelardez@inti.gob.ar)
 * @brief  Archivo con definiciones utiles
 * @version 0.1
 * @date 2022-09-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CORE_H
#define __CORE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/*-----------------------------------------------------------------------------*/


/* Defines ------------------------------------------------------------------*/



// Private functions 


#define PRIVATE    static



typedef enum {
    STATUS_ERROR = 0,
    STATUS_OK
} status_t;

typedef enum {
    LEVEL_LOW =0,
    LEVEL_HIGH
} level_t;


/*-----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __CORE_H */

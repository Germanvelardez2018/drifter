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
#include "debug.h"

/*-----------------------------------------------------------------------------*/


/* Defines ------------------------------------------------------------------*/


//Macros

// BIT(x) regresa el bit x puesto a uno y los demas bits en cero, ej. BIT(3) regresa 00001000
#define BIT(x)               (1<<(x))
// BIT_GET(x,b) regresa el bit b-esimo de x ej. BIT_GET(PINC,3)
#define BIT_GET(x,b)        ((x) & BIT(b))
// BIT_SET(x,b) establece en '1' el bit b de x ej. BIT_SET(PORTD,4)
#define BIT_SET(x,b)        ((x) |= BIT(b))
// BIT_CLEAR(x,b) establece a '0' el bit b de x
#define BIT_CLEAR(x,b)      ((x) &= ~BIT(b))
// BIT_TOGGLE(x,b) invierte el valor del bit b de x a su complemento,
#define BIT_TOGGLE(x,b)     ((x) ^= BIT(b))
// BIT_WRITE(x,b,v) establece el valor 'v' de
#define BIT_WRITE(x,b,v)    ((v)? BIT_SET(x,b) : BIT_CLEAR(x,b))
 
// ES_PAR(x) regresa 0 cuando x no es par y algo diferente de 0 cuando es par
#define ES_PAR(x) (!BIT_GET(x,0))
// Private functions 
#define PRIVATE    static




typedef enum {
    DRIVER_STARTED      = 0,
    DRIVER_NO_STARTED   = 1
} driver_status_t;

typedef enum {
    STATUS_ERROR = 0,
    STATUS_OK
} status_t;

typedef enum {
    LEVEL_LOW =0,
    LEVEL_HIGH
} level_t;


typedef enum {
    LOW_LEVEL    = 0,
   NORMAL_LEVEL  = 1
} battery_state_t;








/// @brief  Valores predefinidos de intervalos validos
typedef enum {

                SLEEP_1MINUTE     = (uint8_t)  1,        //        1 minuto
                SLEEP_5MINUTES    = (uint8_t)  5,        //        5 minutos
                SLEEP_10MINUTES   = (uint8_t)  10,        //        10 minutos
                SLEEP_30MINUTES   = (uint8_t)  30,        //        30 minutos
                SLEEP_1HOUR       = (uint8_t)  60         //        60 minutos

} sleep_interval_t;





#define delay(t)                     HAL_Delay(t)


/*-----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __CORE_H */

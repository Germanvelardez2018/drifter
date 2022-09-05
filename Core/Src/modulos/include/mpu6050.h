/**
 * @file mpu6050.h
 * @author German Velardez (you@domain.com)
 * @brief  Archivo de configuracion del modulo MPU6050
 * @version 0.1
 * @date 2022-09-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */





/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MPU6050_H
#define __MPU6050_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "core.h"
/* Definiciones --------------------------------------------------------------*/


typedef enum {
    SCALA_2G = 0,
    SCALA_4G,
    SCALA_8G,
    SCALA_16G
}mpu6050_scala_t;

/*-----------------------------------------------------------------------------*/


/**
 * @brief Configuramos el periferico y el modulo a utilizar
 * 
 * @return ** status_t 
 */
status_t mpu6050_init();


/**
 * @brief Configuramos la escala del acelerometro
 * 
 * @param scala 
 * @return ** status_t 
 */
status_t mpu6050_set_scala(mpu6050_scala_t scala);



/**
 * @brief Salimos del modo de bajo consumo
 * 
 * @return ** status_t 
 */
status_t mpu6050_resume();


/**
 * @brief Entramos en el modo de bajo consumo
 * 
 * @return ** status_t 
 */
status_t mpu6050_sleep();



/**
 * @brief Obtener la medicion del acelerometro
 * 
 * @param x variable de 16b donde se almacena el valor de medicion del eje x
 * @param y variable de 16b donde se almacena el valor de medicion del eje y
 * @param z variable de 16b donde se almacena el valor de medicion del eje z
 * @return ** status_t 
 */
status_t mpu6050_get_acceleration( int16_t* x, int16_t* y, int16_t* z);



/**
 * @brief Obtener la medicion de temperatura
 * 
 * @param temp 
 * @return ** status_t 
 */
status_t mpu6050_get_temperature( int16_t* temp);



/**
 * @brief 
 * 
 * @return ** status_t 
 */
status_t mpu6050_test();


#ifdef __cplusplus
}
#endif

#endif /* __MPU6050_H */
/**
 * @file adc.h
 * @author German Velardez(gvelardez@inti.gob.ar)
 * @brief  Archivo de configuracion del periferico ADC
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */




/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC1_H
#define __ADC1_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------*/



/**
 * @brief Configuracion de hardware ADC1. Modo simple sin DMA e interrupciones.
 * 
 * @return ** void 
 */
void MX_ADC1_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADC1_H */





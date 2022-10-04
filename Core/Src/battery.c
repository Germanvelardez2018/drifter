#include "battery.h"

// en adc.c
extern  ADC_HandleTypeDef hadc1;

#define B_LOW                                       ("Low")
#define B_NORMAL                                    ("Normal")
#define RESOLUCION                                   (4096) 
#define BB_LOW                                       (3500)


#define BATTERY_FORMAT                              ("Bateria:%s\n")
#define LEN_FORMAT                                    20
uint8_t adc_buffer[100];

PRIVATE uint32_t get_adc(){
  uint32_t adc_value = 0;
  HAL_ADC_Start(&hadc1);
  HAL_Delay(50);
  adc_value = HAL_ADC_GetValue(&hadc1);
 
  return adc_value;
}


PRIVATE battery_state_t get_status(){
    battery_state_t ret = LEVEL_LOW;
    uint32_t adc_value = get_adc();
    sprintf(adc_buffer,"adc:%d\r\n",adc_value);
    modulo_debug_print(adc_buffer);
    if( adc_value < BB_LOW ) return ret;
    return NORMAL_LEVEL;
    }
   



 void battery_init(void){
    MX_ADC1_Init();
}




status_t battery_check_status(uint8_t* buffer,uint8_t max_len){

    status_t ret = STATUS_ERROR;

    uint8_t len = 0;
    len = strlen(buffer);
    uint8_t* p=NULL;
    p = (&(buffer[0])+len);

  //  if((len+ LEN_FORMAT) <max_len)return ret;

    sprintf(p,BATTERY_FORMAT,(get_status() == NORMAL_LEVEL)?B_NORMAL:B_LOW);
    modulo_debug_print(p);
    
    return STATUS_OK;



}

#include "power_save.h"
#include "rtc.h"
#include "debug.h"

extern  RTC_HandleTypeDef hrtc;
PRIVATE  pwr_mode_t __PWR_FLAG__= RUN; 


PRIVATE status_t set_time(uint8_t hours, uint8_t minutes, uint8_t seconds){  
    status_t ret = STATUS_ERROR;
    ret = STATUS_OK;
    RTC_TimeTypeDef sTime = {0};
    sTime.Hours   = hours;
    sTime.Minutes = minutes;
    sTime.Seconds = seconds;
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) == HAL_OK){
        ret = STATUS_OK;
    }
    return ret;
}


PRIVATE status_t get_time(uint8_t* hours, uint8_t* minutes, uint8_t* seconds){  
    status_t ret = STATUS_ERROR;
    RTC_TimeTypeDef sTime = {0};
    if (HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) == HAL_OK){
        ret = STATUS_OK;
        (*hours) =sTime.Hours ;
        (* minutes) =sTime.Minutes ;
        (*seconds) = sTime.Seconds ;
    }        
    return ret;
}



PRIVATE status_t set_alarm(uint8_t hours, uint8_t minutes, uint8_t seconds){  
    status_t ret = STATUS_ERROR;
    ret = STATUS_OK;
    RTC_AlarmTypeDef sAlarm ={0};
    sAlarm.AlarmTime.Hours = hours;
    sAlarm.AlarmTime.Minutes = minutes;
    sAlarm.AlarmTime.Seconds = seconds;
    sAlarm.Alarm = 1;
    if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) == HAL_OK){
        ret = STATUS_OK;
    }
    return ret;
}


pwr_mode_t pwr_get_mode(){
    return __PWR_FLAG__;
}


PRIVATE uint8_t interval_from_flash(){
    // Funcion dummy por el momento
    // Por default dejamos el intervalo en 5
    return 8;
}


void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){
    HAL_ResumeTick();
    modulo_debug_print("rtc iqr\n");
    __PWR_FLAG__= RUN;
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,1);




}




void pwr_init(){
  //  HAL_PWR_EnableSleepOnExit();
        set_time(0,0,0);
}



static void _set_alarm(){

      // leo intervalor from flash (dummy por el momento)
 uint8_t interval = interval_from_flash();
 uint8_t* buffer[100];
 //leo tiempo    
 uint8_t h,m,s ;
 // leo intervalor from flash (dummy por el momento)
 interval = interval_from_flash();
 get_time(&h,&m,&s);
   
 //configuro alarma
 set_alarm(h,m,s+interval);
 sprintf(buffer,"%d:%d:%d=>%d:%d:%d\n",h,m,s,h,m,s+interval);
 modulo_debug_print(buffer);



}


void pwr_sleep(){    
    if(__PWR_FLAG__ == RUN){
        _set_alarm();
        __PWR_FLAG__ = SLEEP;
    }
       
    HAL_SuspendTick();
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,0);

    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    // A dormir o un intervalo en minutos

}
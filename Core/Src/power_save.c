#include "power_save.h"
#include "rtc.h"
#include "debug.h"
#include "mem_services.h"

//define aqui el intervalo
#define INTERVAL(h,m,s,i)                 h,m+i,s // h,m,s+i


extern  RTC_HandleTypeDef hrtc;

PRIVATE  pwr_mode_t __PWR_FLAG__= RUN; 

//para prints de debug
PRIVATE  uint8_t buffer[100];


//  intervalo en sleep mode en SRAM, 
//  se recarga desde memoria flash en momentos especificos
PRIVATE  uint8_t  __sleep_interval__ ;





PRIVATE uint8_t get_sleep_interval(){
    return __sleep_interval__;
}

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



void set_sleep_interval(uint8_t interval){
    __sleep_interval__ = interval;
}



pwr_mode_t pwr_get_mode(){
    return __PWR_FLAG__;
}




void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){
        HAL_ResumeTick();
        modulo_debug_print("rtc iqr\n");
       __PWR_FLAG__= RUN;
}




void pwr_init(){
    set_time(0,0,0);
    // obtengo intervalo desde flash
    modulo_debug_print("pwr init\n");

    mem_s_get_interval(&__sleep_interval__);


}



void pwr_sleep(){    
    
    //configuro alarma si flag no es SLEEP,
    if(__PWR_FLAG__ == RUN){
         // leo intervalor from flash (dummy por el momento)
    uint8_t interval = get_sleep_interval();
    //leo tiempo    
    uint8_t h,m,s ;
    get_time(&h,&m,&s);  
    __PWR_FLAG__ = SLEEP;
    set_alarm(INTERVAL(h,m,s,interval));
    sprintf(buffer,"%d:%d:%d=>%d:%d:%d\n",h,m,s,INTERVAL(h,m,s,interval));
    modulo_debug_print(buffer);
    }
    HAL_SuspendTick();
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

    
    // A dormir o un intervalo en minutos

}
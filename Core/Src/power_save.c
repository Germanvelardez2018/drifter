#include "power_save.h"
#include "rtc.h"
#include "debug.h"


extern  RTC_HandleTypeDef hrtc;


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





PRIVATE status_t set_alarm(uint8_t hours, uint8_t minutes, uint8_t seconds){  
    status_t ret = STATUS_ERROR;
    ret = STATUS_OK;
    RTC_AlarmTypeDef sAlarm ={0};
    sAlarm.AlarmTime.Hours = hours;
    sAlarm.AlarmTime.Minutes = minutes;
    sAlarm.AlarmTime.Seconds = seconds;
    sAlarm.Alarm = 1;
    if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK){
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











PRIVATE uint8_t interval_from_flash(){
    // Funcion dummy por el momento
    // Por default dejamos el intervalo en 5
    return 1;
}






void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){
        HAL_ResumeTick();
        modulo_debug_print("desperte\n");

}




void pwr_init(){
        set_time(0,0,0);
}



void pwr_sleep(){    
    modulo_debug_print("a dormir\n");
    uint8_t interval =0;
    //leo tiempo
    
    uint8_t h,m,s ;
    get_time(&h,&m,&s);
    // leo intervalor from flash (dummy por el momento)

    interval = interval_from_flash();

    m = (m+ interval) % 60;
    //configuro alarma
    set_alarm(h,m,s);





    HAL_SuspendTick();
    HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    // A dormir un tiempo un intervalo en minutos

}
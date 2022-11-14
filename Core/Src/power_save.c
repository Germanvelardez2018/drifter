#include "power_save.h"
#include "rtc.h"
#include "debug.h"
#include "mem_services.h"

//define aqui el intervalo
#define INTERVAL(h,m,s,i)                 h,m+i,s // h,m+i,s


extern  RTC_HandleTypeDef hrtc;

PRIVATE  pwr_mode_t __PWR_FLAG__= RUN; 

//para prints de debug
PRIVATE  uint8_t buffer[100];


//Valor en sram del perido


PRIVATE sleep_interval_t __INTERVAL__ = 1;



//  intervalo en sleep mode en SRAM, 
//  se recarga desde memoria flash en momentos especificos



PRIVATE uint8_t pwr_get_interval(){
    return __INTERVAL__;
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
}



void pwr_sleep(){    
    
    //configuro alarma si flag no es SLEEP,
    if(__PWR_FLAG__ == RUN){
    uint8_t h,m,s ;
    get_time(&h,&m,&s);  
    __PWR_FLAG__ = SLEEP;
    set_alarm(INTERVAL(h,m,s,__INTERVAL__));
    sprintf(buffer,"\r\n%d:%d:%d=>%d:%d:%d\n",h,m,s,INTERVAL(h,m,s,__INTERVAL__));
    modulo_debug_print(buffer);
    }
    HAL_SuspendTick();
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    // A dormir o un intervalo en minutos

}



/***
 *  NUEVAS CARACTERISTICAS EN EL DISPOSITIVO
 *  
 * 
 * 1) El periodo en modo bajo consumo pueden ser 
 *  ciertos valores puntuales predefinidos   :
 *                  1) 1 minuto
 *                  2) 5 minutos
 *                  3) 10 minutos
 *                  4) 30 minutos
 *                  5) 60 minutos
 *                  
 * 
 * 2) El dispositivo calibra sensor cuando 
 *  recibe el comando correspondiente
 * 
 * 
 * 
 * 
*/



void pwr_set_interval( sleep_interval_t periode){

    uint8_t value = 0;
    switch (periode)
    {
        case SLEEP_1MINUTE:
            value = 1;
            break;
        case SLEEP_5MINUTES:
            value = 5;
            break;
        case SLEEP_10MINUTES:
            value = 10;
            break;
        case SLEEP_30MINUTES:
            value = 30;
            break;
        case SLEEP_1HOUR:
            value = 60;
            break;
        default:
            value = 1;
            break;
    }
        __INTERVAL__ = value;
}



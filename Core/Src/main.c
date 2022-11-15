/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "timer.h"
#include "watchdog.h"
#include "adc.h"
#include "power_save.h"
// Modulos
#include "fsm.h"
#include "debug.h"
#include "mpu6050.h"
#include "sim7000g.h"
#include "AT45DB041.h"
#include "MQTT.h"
#include "battery.h" // 
#include "mmap.h"
#include "mem_services.h" // esto es lo que va no AT45
/* USER CODE END Includes */


#define COUNTER                (0)

#define MAX_COUNTER            (10)






//si esta definido default value, recargamos flash con valores default
//#define DEFAULT_VALUES

 #define MSG_INIT            "test drifter modular \n"
    


/* USER CODE END PM */
/* Private variables ---------------------------------------------------------*/

extern  UART_HandleTypeDef huart1;




// en wdt.c
extern IWDG_HandleTypeDef hiwdg;
// en timer.c
extern  TIM_HandleTypeDef  htim1;
// En sim7000g.c
extern DMA_HandleTypeDef hdma_usart2_tx;


PRIVATE  fsm_state_t               device;

#define  DATAFRAME_SIZE              255
PRIVATE  uint8_t    data_frame_buffer[DATAFRAME_SIZE];
PRIVATE  uint8_t    buf[255];
PRIVATE  uint8_t    counter = 0;
PRIVATE  pwr_mode_t  modo = RUN ;
PRIVATE  uint8_t     flag_params = 0;
PRIVATE  uint8_t     max_counter ;


static uint8_t gps[120] = {"SOY EL GPS \r\n"},sensor[80];



void inline get_data_frame_to_save(uint8_t* buffer,uint8_t len,uint8_t counter){
  

  memset(buffer,0,len);
  modulo_debug_print("FSM: ON FIELD\r\n");  
  mpu6050_get_measure(sensor,80);
  sim7000g_get_NMEA(gps,120);
  sprintf(buffer,"{ gps: %s sensor: %s \r\n}",gps,sensor);
  modulo_debug_print(buffer);
  write_data(buffer,counter);
}


void  check_flag_params(){
   flag_params = sim_get_update_params();
   if(flag_params )      {
   static uint8_t interval =0;
   static uint8_t max = 0;
   modo = RUN;
   uint8_t cmd[40]={0};
   memset(cmd,0,40);
   sim_copy_buffer_cmd(cmd);
   modulo_debug_print("\r\ncmd:");
   modulo_debug_print(cmd);
   sim_get_values(cmd,&interval,&max);
   sprintf(buf," get value:\r\r\n INTERVAL:%d  MAX:%d\r\n",interval,max);
   modulo_debug_print(buf);
   if( (interval != 0) &&  (max != 0)){
      uint8_t i = 0 , m = 0;
       mem_s_get_interval(&i);
       mem_s_get_max_amount_data(&m);
       // Con este if se evitan problema de repeticion me mensaje por servidor mqtt problematico
       if(i != interval && max != m){
         memset(buf,0,255);
         sprintf(buf," Nueva configuracion:\r\r\n INTERVAL:%d  MAX:%d\r\n",interval,max);
         modulo_debug_print(buf);
         mem_s_set_interval(&interval);
         mem_s_set_max_amount_data(&max);
         while(1);    
       }
   }
   else{
    modulo_debug_print("Parametros invalidos \r\n");
   }
  sim_set_update_params(0);
 }    
}



static void inline on_field(){
  // leo el contador
  mem_s_get_counter(&counter);
  get_data_frame_to_save(data_frame_buffer,DATAFRAME_SIZE,counter);
  //Aumento contador de muestras almacenadas
  if( counter >= max_counter){
      device = FSM_MEMORY_DOWNLOAD;   
      fsm_set_state(FSM_MEMORY_DOWNLOAD); 
      modulo_debug_print("ON FIELD => DOWNLOAD \r\n");}
  else{
     counter = counter +1 ;
     mem_s_set_counter(&counter);
  }
}


static void inline on_download(void){
  modulo_debug_print("FSM: DOWNLOAD\r\n");  
  counter = (max_counter< counter)? max_counter: counter;
 // counter = MAX_COUNTER;
  sprintf(data_frame_buffer,"extraer :%d datos\n",counter);
  modulo_debug_print(data_frame_buffer);
  while(counter !=  0){
    memset(data_frame_buffer,0,255);
    read_data(data_frame_buffer,counter);
    sim7000g_mqtt_publish(MQTT_TOPIC,data_frame_buffer,strlen(data_frame_buffer));
    modulo_debug_print(data_frame_buffer);
    wdt_reset();
    HAL_Delay(1000); 
    counter = counter -1;
    mem_s_set_counter(&counter);
  }
  wdt_reset();  
  mem_s_set_counter(&counter);
  device = FSM_ON_FIELD;
  fsm_set_state(FSM_ON_FIELD); 
  modulo_debug_print("DOWNLOAD => ON FIELD \r\n");
}








static void app_init(){
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  SystemClock_Config();
  // Memoria
  mem_s_init();
  battery_init();
  MX_DMA_Init();
  MX_RTC_Init();
  MX_TIM1_Init();
  //  Necesario para evitar que el micro se reinicio por WDT
  HAL_TIM_Base_Start_IT(&htim1);
  // Inicio modulos
  modulo_debug_init();
  sim7000g_init();
  pwr_init();
  fsm_init();
  mpu6050_init(); //! Sensor  
  // Cargar parametros desde memoria flash
  mem_s_get_max_amount_data(&max_counter);
  mem_s_get_counter(&counter);
}


static void mqtt_config(){
  sim7000g_check();
  sim7000g_get_signal();
  sim7000g_open_apn();
  sim7000g_get_operator();
  sim7000g_set_gps(1);
  sim7000g_set_mqtt_config(MQTT_URL, MQTT_ID, MQTT_PASS, MQTT_QOS);
  sim7000g_resume();
  #define PUB_MSG           sim_get_id()
  sim7000g_mqtt_publish("SIMO_INIT",PUB_MSG,strlen(PUB_MSG));
  gpio_interruption_init();
}



/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  app_init();
// Sirve para cargar valores por defecto a memoria flash
// prueba adc
  modulo_debug_print("init program \r\n");
  mqtt_config();
  // ! Inicia el WDT
  uint8_t string[200]={0};

  


  //MX_IWDG_Init();
  device = fsm_get_state();
  while (1)
  {   
    modo = pwr_get_mode();   //! Si te levanto una interrupcion por comandos en vez de una IRQ del RTC, volve a dormir
    wdt_reset();
    if(modo == RUN){
      switch (device){
       case FSM_ON_FIELD:
          on_field();
          break;
       case FSM_MEMORY_DOWNLOAD:
             on_download();
          break;
        default:
        break;
      }
    }
   check_flag_params();
    pwr_sleep();
 }
}






/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
















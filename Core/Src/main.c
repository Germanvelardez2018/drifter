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
#include "MPU6050.h"
#include "SIM7000G.h"
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



/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
 #define MSG_INIT            "test drifter modular \n"
    


/* USER CODE END PM */
/* Private variables ---------------------------------------------------------*/

extern  UART_HandleTypeDef huart1;




// en wdt.c
extern IWDG_HandleTypeDef hiwdg;
// en timer.c
extern  TIM_HandleTypeDef  htim1;
PRIVATE  fsm_state_t       device;
PRIVATE  uint8_t           buffer[255];

PRIVATE  uint8_t  buf[255];
PRIVATE  uint8_t  counter = 0;
uint8_t flag = 1;
uint8_t flag_params = 0;

extern DMA_HandleTypeDef hdma_usart2_tx;
PRIVATE uint8_t max_counter ;



static void inline on_field(){
  // leo el contador
  mem_s_get_counter(&counter);
  mem_s_get_max_amount_data(&max_counter);
  modulo_debug_print("FSM: ON FIELD\r\n");
  // Secuencia:
  memset(buffer,0,255);
 // mpu6050_get_measure(buffer,255);
 //sprintf(buffer,"Contador:%d.\r\n",counter);
  sim7000g_get_NMEA(buffer,255);
  modulo_debug_print(buffer);
  write_data(buffer,counter);
  //Aumento contador de muestras almacenadas
  if( counter >= max_counter){
      device = FSM_MEMORY_DOWNLOAD;   
      fsm_set_state(device); 
  }else{
    counter = counter +1 ;
    // seteo el contador
    mem_s_set_counter(&counter);
  }
}



static void inline on_download(void){
  modulo_debug_print("FSM: DOWNLOAD\r\n");  
  mem_s_get_counter(&counter);
  if(counter > MAX_COUNTER) counter = MAX_COUNTER;
 // counter = MAX_COUNTER;
  sprintf(buffer,"extraer :%d datos\n",counter);
  modulo_debug_print(buffer);
  flag = 1;
  while(counter >=  0 && flag){
    memset(buffer,0,255);
    read_data(buffer,counter);
    sim7000g_mqtt_publish(MQTT_TOPIC,buffer,strlen(buffer));
    modulo_debug_print("mensaje publicado:");
    modulo_debug_print(buffer);
    HAL_IWDG_Refresh(&hiwdg);
    HAL_Delay(2000);
    if(counter ==0 ){ 
       flag = 0;} 
    else{
       counter = counter -1;
       mem_s_set_counter(&counter);}
  }

  memset(buffer,0,255);
  HAL_IWDG_Refresh(&hiwdg);
  counter = 0;
  mem_s_set_counter(&counter);
  device = FSM_ON_FIELD;  
  fsm_set_state(device); 

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
  // Sensor
 // mpu6050_init();  
}






static void mqtt_config(){
  sim7000g_check();
  sim7000g_get_signal();
  sim7000g_open_apn();
  sim7000g_get_operator();
  sim7000g_set_gps(1);
  sim7000g_set_mqtt_config(MQTT_URL, MQTT_ID, MQTT_PASS, MQTT_QOS);
  sim7000g_resume();
  sim7000g_mqtt_subscription("CMD");
  sim7000g_sleep();

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

  uint8_t interval = 1;
  uint8_t max = MAX_COUNTER;



  modulo_debug_print("init program \r\n");
  modulo_debug_print(sim_get_id());


  mqtt_config();
  #define PUB_MSG            "check"
  sim7000g_mqtt_publish("check",PUB_MSG,strlen(PUB_MSG));
  gpio_interruption_init();

  MX_IWDG_Init();
  device =FSM_ON_FIELD;// fsm_get_state();
  pwr_mode_t  modo = RUN ;
  while (1)
  {   
    modo = pwr_get_mode();
    HAL_IWDG_Refresh(&hiwdg);

  if(modo == RUN){
      switch (device)
      {
       case FSM_ON_FIELD:
            on_field();

          break;
       case FSM_MEMORY_DOWNLOAD:
           on_download();
          break;
        default:
           //nothing
        break;
      }
      }
      flag_params = sim_get_update_params();
      if(flag_params )      {
        modo = RUN;
        sim_set_update_params(0);
        uint8_t cmd[40]={0};
        sim_copy_buffer_cmd(cmd);
        modulo_debug_print("cmd:");
        modulo_debug_print(cmd);
        sim_get_values(cmd,&interval,&max);
         
        // ! get_value devuelve 0,0 si hay error en parseo
        if( interval != 0 &&  max != 0){
           uint8_t i = 0 , m = 0;
            mem_s_get_interval(&i);
            mem_s_get_max_amount_data(&m);
            // Con este if se evitan problema de repeticion me mensaje por servidor mqtt problematico
            if(i != interval || max != m){
              sprintf(buf," INTERVAL:%d  MAX:%d\r\n",interval,max);
              modulo_debug_print(buf);
              mem_s_set_interval(&interval);
              mem_s_set_max_amount_data(&max);
              modulo_debug_print("parametros configurados. Reset \r\n");
              memset(cmd,40,0);
              while(1);
                
            }
        
        }
      }    

    pwr_sleep();
 }

}



// +SMSUB: "CMD"," ,1,1, " FORMATEO esta cadena




/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

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
#define MAX_COUNTER           (10)
#define INTERVAL              (10)


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

// Peticiones dma
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_usart2_tx;
extern  UART_HandleTypeDef huart1;




// en adc.c
extern  ADC_HandleTypeDef hadc1;

// en wdt.c
extern IWDG_HandleTypeDef hiwdg;
// en timer.c
extern  TIM_HandleTypeDef  htim1;
PRIVATE  fsm_state_t       device;
PRIVATE  uint8_t           buffer[255];

PRIVATE uint8_t buf[255];
PRIVATE  uint8_t           counter = 0;
PRIVATE  uint8_t           max_counter = 0;

uint8_t flag = 1;



static void inline on_field(){
  modulo_debug_print("FSM: ON FIELD\n");
  // Secuencia:
  memset(buffer,0,255);

  //Obtengo datos de sensores
  mem_s_get_counter(&counter);
  //Guardo datos de sensores
  //mpu6050_get_measure(buffer,255);
  sim7000g_get_NMEA(buffer,255);
  modulo_debug_print(buffer);
  write_data(buffer,counter);
  //Automento contador de muestras almacenadas
  if( counter >= max_counter){
      device = FSM_MEMORY_DOWNLOAD;   
  }else{
    counter = counter +1 ;
    mem_s_set_counter(&counter);
  }
}



static void inline on_download(void){
  modulo_debug_print("FSM: DOWNLOAD\n");  
  mem_s_get_counter(&counter);
  sprintf(buffer,"extraer :%d datos\n",counter);
  modulo_debug_print(buffer);
  sim7000g_mqtt_subscription("CMD");

  while(counter > 0){
    counter = counter -1;
    sprintf(buffer,"counter :%d \n",counter);
    modulo_debug_print(buffer);
    memset(buffer,0,255);
    read_data(buffer,counter);
    modulo_debug_print(buffer);
    modulo_debug_print("<=");
    HAL_IWDG_Refresh(&hiwdg);
    // envio por mqtt
    sim7000g_mqtt_publish(MQTT_TOPIC,buffer,strlen(buffer));
    HAL_Delay(10);
  }

  memset(buffer,0,255);
  battery_check_status(buffer,255);
  sim7000g_mqtt_publish(MQTT_TOPIC,buffer,strlen(buffer));
  sim7000g_mqtt_unsubscription("CMD");
   counter = 0;
   mem_s_set_counter(&counter);
   device = FSM_ON_FIELD;  
   fsm_set_state(device); 
   sim7000g_get_interval();
   sim7000g_get_max();

}





static void app_init(){
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  SystemClock_Config();
  mem_s_init();
//  MX_ADC1_Init();  // Esta en battery_init 
  battery_init();
  MX_DMA_Init();
  MX_RTC_Init();
  MX_TIM1_Init();
  // Necesario para evitar que el micro se reinicio por WDT
  HAL_TIM_Base_Start_IT(&htim1);
  // Inicio modulos
  modulo_debug_init();
  sim7000g_init();
  pwr_init();
  fsm_init();
  // Sensor
   //mpu6050_init();
  // Memoria
}




/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  // Configuracion inicial de los perifericos
  app_init();
  uint8_t test_adc[100]={"hola mundo "};

// Test bateria
while(1){

  battery_check_status(test_adc,100);
  delay(2500);
  modulo_debug_print(test_adc);
}

while(1);





// Sirve para cargar valores por defecto a memoria flash
// prueba adc
  uint8_t c= 0;
  uint8_t max = 5;
  uint8_t interval = 1;

 // mem_s_set_counter(&c);
 // mem_s_set_interval(&interval);
 // mem_s_set_max_amount_data(&max);

  mem_s_get_counter(&max);
  mem_s_get_max_amount_data(&max);
  mem_s_get_interval(&interval);
  sprintf(buffer,"default values: counter:%d, max_counter:%d, interval:%d \r\n",counter,max,interval);
  modulo_debug_print(buffer);
  sim7000g_check();
  sim7000g_get_signal();
  sim7000g_open_apn();
  sim7000g_get_operator();
  sim7000g_set_gps(1);
  sim7000g_set_mqtt_config(MQTT_URL, MQTT_ID, MQTT_PASS, MQTT_QOS);
  sim7000g_resume();
  sim7000g_mqtt_subscription("CMD");
  sim7000g_mqtt_check();

// formato de lo que recibis 
  sim7000g_mqtt_publish("SIMO_CONFIG2","RETORNO \r\n",strlen("RETORNO \r\n "));
  sim7000g_sleep();


  MX_IWDG_Init();

  device = fsm_get_state();
  pwr_mode_t  modo ;
  while (1)
  {   
  HAL_IWDG_Refresh(&hiwdg);
  modo = pwr_get_mode();

  if(modo == RUN){

      switch (device)
      {
       case FSM_ON_FIELD:
            sim7000g_resume();
            on_field();
            sim7000g_sleep();
          break;
       case FSM_MEMORY_DOWNLOAD:
           on_download();
          break;
        default:
        //nothing
        break;
      }
  }
    pwr_sleep();
  
  }
  /* USER CODE END 3 */
}




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

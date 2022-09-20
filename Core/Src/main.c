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
#include  "debug.h"
#include  "MPU6050.h"
#include "SIM7000G.h"
#include "AT45DB041.h"

#include "mmap.h"
#include "mem_services.h" // esto es lo que va no AT45
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
 #define MSG_INIT            "test drifter modular \n"
    
  #define BUFFER_FLAG          0
  #define FULL_ERASE           0
  #define CHECK_TEMP           1

/* USER CODE END PM */
/* Private variables ---------------------------------------------------------*/


// Peticiones dma
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_usart2_tx;
// en wdt.c
extern IWDG_HandleTypeDef hiwdg;
// en timer.c
extern  TIM_HandleTypeDef  htim1;
PRIVATE  fsm_state_t       device;
PRIVATE  uint8_t           buffer[255];
PRIVATE  uint8_t           counter = 0;
PRIVATE  uint8_t           max_counter = 0;




static void inline on_field(){
  modulo_debug_print("FSM: ON FIELD\n");
  // Secuencia:
  //Obtengo datos de sensores
  mem_s_get_counter(&counter);
  //Guardo datos de sensores
  mpu6050_get_measure(buffer,255);
  write_data(buffer,counter);
  memset(buffer,0,255);
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
  status_t ret = STATUS_ERROR;
  
  mem_s_get_counter(&counter);
  sprintf(buffer,"extraer :%d datos\n",counter);
  modulo_debug_print(buffer);
  
  while(counter > 0){
    counter = counter -1;
    memset(buffer,0,200);
    read_data(buffer,counter);
    modulo_debug_print(buffer);
    modulo_debug_print("<=");
    HAL_Delay(100);
  }
   counter = 0;
   mem_s_set_counter(&counter);
   device = FSM_ON_FIELD;   
   

  //Me conecto a servidor
  //Leo las muestras y las envio al servidor
}










static void app_init(){
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  SystemClock_Config();
  mem_s_init();
  MX_DMA_Init();
  MX_RTC_Init();

  MX_TIM1_Init();
  // Necesario para evitar que el micro se reinicio por WDT
  HAL_TIM_Base_Start_IT(&htim1);
  // Inicio modulos
  modulo_debug_init();
  sim7000g_init();
  pwr_init();
  MX_IWDG_Init();
  fsm_init();
  // Sensor
  mpu6050_init();
  // Memoria
  HAL_Delay(500);
 

}




/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  // Configuracion inicial de los perifericos
  app_init();


//write_data("hola mundo\n",13);
#ifdef test
    at45db_resumen();
    at45db_write_buffer2("stm32\n",8,0);
    at45db_sleep();

    memset(buffer,0,255);

    at45db_resumen();
    at45db_read_buffer2(buffer,8,0);

    at45db_sleep();

    //read_data(buffer,13);
    modulo_debug_print("obtenido de buffer: ");
    modulo_debug_print(buffer);
#endif




  uint8_t c= 0;
  uint8_t max = 5;
  mem_s_set_counter(&c);

  mem_s_set_max_amount_data(&max);

  mem_s_get_max_amount_data(&max_counter);
  sprintf(buffer, " max counter:%d\n",max_counter);
  modulo_debug_print(buffer);

  mem_s_get_counter(&counter);
  sprintf(buffer, "  counter:%d\n",counter);
  modulo_debug_print(buffer);


  modulo_debug_print(MSG_INIT);






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
    pwr_sleep();
    HAL_Delay(1000);
    modulo_debug_print("\n-> wake up\n");

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

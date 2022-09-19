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
extern  TIM_HandleTypeDef htim1;
PRIVATE  fsm_state_t device;
PRIVATE  uint8_t     buffer[255];
PRIVATE uint8_t      counter = 0;



// callbacks






static void inline write_data(uint8_t* buffer, uint16_t page){

    uint8_t len =  strlen(buffer)+1;
    uint8_t _len = len;
    at45db_resumen();
    at45db_write_page(&(_len),1,(page+ MMAP_DATA_OFSSET),0);
    at45db_write_page(buffer ,len,(page+ MMAP_DATA_OFSSET),1);
    at45db_sleep();

}






static void inline read_data(uint8_t* buffer, uint16_t page){
   at45db_resumen();
    uint8_t len=0;
//    at45db_read_page((&len),1,(page + MMAP_DATA_OFSSET),0);
//    at45db_read_page(buffer ,len,(page + MMAP_DATA_OFSSET),1);
    at45db_sleep();

}









static void inline on_field(){
  modulo_debug_print("FSM: ON FIELD\n");
  
  // Secuencia:
  //Obtengo datos de sensores
  mem_s_get_counter(&counter);
  sprintf(buffer," medicion numero:%d '  \n",counter);
  modulo_debug_print("dato insertado=>");
  modulo_debug_print(buffer);
  //Guardo datos de sensores
  
  //mem_load_string(buffer,strlen(buffer)+1,counter);
   write_data(buffer,counter);
  
  
  memset(buffer,0,255);

  //Automento contador de muestras almacenadas
  counter = counter +1 ;
  mem_s_set_counter(&counter);

  if( counter >= 4){
      device = FSM_MEMORY_DOWNLOAD;   
  }

}


static void inline on_download(void){
  modulo_debug_print("FSM: DOWNLOAD\n");
  status_t ret = STATUS_ERROR;
  uint8_t counter = 0 ;
  mem_s_get_counter(&counter);
  sprintf(buffer,"extraer :%d datos\n",counter);
  modulo_debug_print(buffer);
  memset(buffer,0,255);


  while(counter > 0){
    sprintf(buffer,"\ncounter:%d\n",counter);
    modulo_debug_print(buffer);
   read_data(buffer,1);

     
    modulo_debug_print("enviando datos:");
    modulo_debug_print(buffer);
    memset(buffer,0,200);

    
    counter = counter -1 ;
    HAL_Delay(100);

  }

   counter = 0;
   mem_s_set_counter(&counter);
   device = FSM_ON_FIELD;   
   modulo_debug_print("\n-> on field again\n");

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
 // HAL_TIM_Base_Start_IT(&htim1);
  // Inicio modulos
  modulo_debug_init();
  sim7000g_init();
  pwr_init();
  MX_IWDG_Init();
  fsm_init();
  // Sensor
  mpu6050_init();
  // Memoria
  HAL_Delay(1000);
 

}




/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  // Configuracion inicial de los perifericos
  app_init();
  uint8_t c= 0;
  mem_s_set_counter(&c);

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
    //pwr_sleep();
    HAL_Delay(3500);
        modulo_debug_print("\n-> desperto\n");

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

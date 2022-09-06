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
#include "uart.h"
#include "watchdog.h"
#include "spi.h"
#include "adc.h"
#include "i2c.h"
#include "dma.h"

// Modulos
#include "fsm.h"
#include  "debug.h"
#include  "AT45DB041.h"
#include  "MPU6050.h"
#include "SIM7000G.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
 #define MSG_INIT            "Init debug con DMA \n"
  
  
  #define MSG                 " Driver  AT45DB by Velardez German \n"
  #define MSG_LEN             (strlen(MSG) +1)

  #define BUFFER_FLAG          0
  #define FULL_ERASE           0
  #define CHECK_TEMP           1

/* USER CODE END PM */
/* Private variables ---------------------------------------------------------*/


// Peticiones dma
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_usart2_tx;


// en timer.c
extern  TIM_HandleTypeDef htim4;




// callbacks


static void on_field(){
  modulo_debug_print("FSM: ON FIELD\n");

}


static void on_download(){
  modulo_debug_print("FSM: DOWNLOAD");

}







/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);



static void app_init(){
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  //MX_IWDG_Init();
  MX_TIM4_Init();
  // Necesario para evitar que el micro se reinicio por WDT
  //HAL_TIM_Base_Start_IT(&htim4);
  // Inicio modulos
  modulo_debug_init();
  sim7000g_init();
  pwr_init();
}




/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  // Configuracion inicial de los perifericos
  app_init();
  /* USER CODE BEGIN 2 */
  // MENSAJE INICIAL
  modulo_debug_print(MSG_INIT);



  
  
// INICIO MODULO SIM


  status_t ret = STATUS_ERROR;
    ret =  sim7000g_check();
    if(ret == STATUS_OK)
    {
       modulo_debug_print("sim7000g ready\n");
    } else{
       modulo_debug_print("sim7000g NOT ready\n");
    }
    


  fsm_set_callbacks(on_field,on_download);

  fsm_loop();


  while (1)
  {   
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

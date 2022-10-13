#include "gpio.h"
#include "uart.h"

PRIVATE driver_status_t __DRIVER_INITED__ = DRIVER_NO_STARTED;



void GPIO_init(void){
  if(__DRIVER_INITED__ == DRIVER_NO_STARTED){
    MX_GPIO_Init();
    __DRIVER_INITED__ = DRIVER_STARTED;
  } 
}



 // Dummy, apagar todos los gpio es complejo y no lo veo necesario
 // Dejo funcion para mantener coherencia con otros drivers
void GPIO_deinit(void){
if(__DRIVER_INITED__ == DRIVER_STARTED){
   if(1) 
   {
      __DRIVER_INITED__ = DRIVER_NO_STARTED;
   }
   else{
      Error_Handler();
   }
}
}




/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  
  HAL_GPIO_EXTI_IRQHandler(SIM7000G_RI_Pin);
  
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */






/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
 void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4 | GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2|GPIO_PIN_13 | GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA12  // Modificacion manual*/
  GPIO_InitStruct.Pin =  GPIO_PIN_12 | GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB2 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_13 | GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode =GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  

}


void gpio_interruption_init(){
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}






void gpio_interruption_deinit(){
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

}
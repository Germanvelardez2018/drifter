#include "watchdog.h"


IWDG_HandleTypeDef hiwdg;
#define  FULL_TIME                                        (4095)//4095
#define  WDT_TIME                                         (4095)


PRIVATE uint8_t   __WDT_STARTED__ = 0;


/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
 void MX_IWDG_Init(void)
{
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Reload = WDT_TIME;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
      modulo_debug_print("MURIO WDT\n");
      Error_Handler();
  }
  __WDT_STARTED__ = 1;
}



void wdt_reset(){

    if(__WDT_STARTED__)HAL_IWDG_Refresh(&hiwdg);

}

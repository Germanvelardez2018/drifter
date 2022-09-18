#include "main.h"
#include "debug.h"
#include "uart.h"
#include "string.h"


extern UART_HandleTypeDef huart2;


void  modulo_debug_init(void){
   UART_DEBUG_init();
}


void modulo_debug_print(char* string){
    HAL_UART_Transmit(&huart2,string,strlen(string),1000);
  //   HAL_UART_Transmit_DMA(&huart2,string,strlen(string));
    // HAL_UART_Transmit_IT(&huart2,string,strlen(string));
}
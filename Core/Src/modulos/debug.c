#include "main.h"
#include "debug.h"
#include "uart.h"
#include "string.h"


extern UART_HandleTypeDef huart2;


void  modulo_debug_init(void){
   UART_DEBUG_init();
}



void  modulo_debug_deinit(void){
   UART_DEBUG_deinit();
}


void modulo_debug_print(char* string){
   // HAL_UART_Transmit(&huart2,string,strlen(string),500);
}
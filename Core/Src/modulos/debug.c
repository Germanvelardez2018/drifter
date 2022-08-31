#include "main.h"
#include "debug.h"
#include "uart.h"
#include "string.h"


extern UART_HandleTypeDef huart2;

extern DMA_HandleTypeDef hdma_usart2_tx;





// Notas importantisimas

// El DMA necesita que el periferico UART tenga habilitadas las interrupciones. 
// Solo dios sabe porque. Si no estan habilitadas, entonces dma corre una sola vez y
// luego hay que mandar configuracion inicial de uart para que tome otra peticion





void USART2_IRQHandler(void){
           
            HAL_UART_IRQHandler(&huart2);
            
}






 HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){

     UNUSED(huart);

   // HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,1);
   
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);



}






        





void  modulo_debug_init(void){
    MX_USART2_UART_Init();

}

void modulo_debug_print(char* string){
     HAL_UART_Transmit_DMA(&huart2,string,strlen(string));
    // HAL_UART_Transmit_IT(&huart2,string,strlen(string));

}
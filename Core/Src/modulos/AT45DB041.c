/**
 * @file AT45DB041.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <stdio.h>
#include "main.h"
#include "gpio.h"
#include "spi.h"
#include "AT45DB041.h"
#include "debug.h"
extern   SPI_HandleTypeDef hspi1;





// READ FUNCTIONS
#define      CMD_READBUFF1                    (0xD4)
#define      CMD_READBUFF2                    (0x56)
#define      CMD_READPAGE                     (0x52)      
#define      CMD_READPAGEHF                   (0x0B)  // Read page in high freq.
#define      CMD_READPAGELF                   (0x03)  // Read pafe in low freq.

//WRITE FUNCTIONS
#define    CMD_WRITEPAGE_B1                   (0x82)
#define    CMD_WRITEPAGE_B2                   (0x85)
#define    CMD_WRITEBUFF1                     (0x84)
#define    CMD_WRITEBUFF2                     (0x87)
#define    CMD_PAGETOBUFF1                    (0x53)
#define    CMD_PAGETOBUFF2                    (0x55)

#define    CMD_PAGETHROUGHBUFF1               (0x83)  // Write tn page through buffer 1 with automatic erase
#define    CMD_PAGETHROUGHBUFF2               (0X86)  // Write to page through buffer 2 with automatic erase

#define    CMD_PAGEBUFF1                      (0X88)
#define    CMD_PAGEBUFF1                      (0X89)

#define    CMD_PAGEBUFF1POS                   (0x02) // ! Permite seleecionar la posicion inicial

// COMPARE FUNCTIONS
#define    CMD_CMPBUFF1                      (0x60)
#define    CMD_CMPBUFF2                      (0x61)

// STATE
#define    CMD_GETSTATUS                    (0xD7)   //! status register 2 bytes
#define    CMD_RESUMEN                      (0xAB)
#define    CMD_LOWPOWER                     (0xB9)  
#define    CMD_READID                       (0x9F)
// ID_DEVICE

#define DEVICE_ID_0                          0x1F    //id byte  1
#define DEVICE_ID_1                          0x24    //id byte  2
#define DEVICE_ID_2                          0x00    //id byte  3
#define EDI                                  0x01    // id byte 4



// CONFIG SIZE FROM 265 TO 255 BYTE PER PAGE
#define CMD_RESIZE                       (0x3D, 0x2A, 0x80, 0xA6)
#define CMD_READSTATUS                   (0x57)

// ERASE MEMORY

#define CMD_ERASEPAGE                   (0X81)
#define CMD_ERASEBLOCK                  (0x50)
#define CMD_ERASESECTOR                 (0x7C)
#define SEC_ERASECHIP                   (0x9A, 0x80, 0x94, 0xC7) //! dont use this array to init a buffer. It won't work


// MASK

#define AT45DB_STATUS_COMP                (1 << 6) /* COMP */
#define AT45DB_STATUS_READY               (1 << 7) /* RDY/BUSY */



#define AT45DB_STATUS_PGSIZE                (1 << 0) /* PAGE SIZE */
#define AT45DB_STATUS_PROTECT               (1 << 1) /* PROTECT */
#define AT45DB_STATUS_COMP                  (1 << 6) /* COMP */
#define AT45DB_STATUS_READY                 (1 << 7) /* RDY/BUSY */


#define dummyByte                           (0xFF)
#define AT45DB_TIMEOUT                       (50)


#define PRIVATE    static



#define AT45DB_SPI              &hspi1
#define CS_Pin                  SPI1_CS_Pin
#define CS_GPIO_Port            SPI1_CS_Port          







// You need define this function with your HAL function.


PRIVATE void delay(uint32_t time){
        HAL_Delay(time);
}


PRIVATE status_t spi_init(){
        MX_SPI1_Init();

        return STATUS_OK;
}


PRIVATE status_t gpio_init(){
        GPIO_init();

        return STATUS_OK;
}


PRIVATE  status_t  gpio_write(level_t value){
        HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,(GPIO_PinState)value);
        return STATUS_OK;
}


PRIVATE  status_t  spi_write(uint8_t* buffer, size_t len ){
            HAL_SPI_Transmit(AT45DB_SPI,buffer,len,1000);
          //  HAL_SPI_Transmit_DMA(AT45DB_SPI,buffer,len);
            return STATUS_OK;
}

PRIVATE  status_t  spi_read(uint8_t* buffer, size_t len){

        HAL_SPI_Receive(AT45DB_SPI,buffer,len,1000);  
        //HAL_SPI_Receive_DMA(AT45DB_SPI,buffer,len);
        return STATUS_OK;
}





// PRIVATE FUNCTION API


PRIVATE uint8_t  at45db_get_status(){
        uint8_t first_byte_status[2] = {0};
        uint8_t cmd = CMD_GETSTATUS;
        gpio_write(0);
        spi_write(&cmd,1); // Leo los 2 bytes de registro de status
        spi_read(&first_byte_status,2); // Leo los 2 bytes de registro de status
        gpio_write(1);
        return first_byte_status[0];
}


PRIVATE uint8_t at45db_is_ready(){
        uint8_t ret= 0; // Device bussy
        ret = at45db_get_status() ;
        return (ret & AT45DB_STATUS_READY);
}


PRIVATE void at45db_wait(uint16_t timeout){
        uint8_t ret = at45db_is_ready();
        while(!(ret = at45db_is_ready) || (timeout != 0) ){
                timeout --;
                delay(1);
        }
}




PRIVATE uint8_t  at45db_check_id(){
        uint8_t ret = 0;
        uint8_t data[4]={0};
        uint8_t cmd = CMD_READID;
        gpio_write(0);
        spi_write(&cmd,1);
        spi_read(&data,4);
        gpio_write(1);
        if(data[0] == DEVICE_ID_0   
        || data[1] == DEVICE_ID_1        
        || data[2] == DEVICE_ID_2        
        || data[3] == EDI) {
                ret = 1; // id checked. OK
        }
        return ret;
}






 uint8_t at45db_set_size_page( size_page_t size){
        gpio_write(0);
        static uint8_t at45db_pgsize_cmd[] = {0x3D, 0x2A, 0x80, 0xA6};  // 256
        if( size == SIZE_PAGE_264){
                at45db_pgsize_cmd[3] = 0xA7; // 264 It's the default mode
        }
        spi_write(at45db_set_size_page,4);
        gpio_write(1);
        at45db_wait(AT45DB_TIMEOUT);
}




 uint8_t get_status(){
        uint8_t res = at45db_status();
        return res;
}

uint8_t is_ready(){
        uint8_t ret = at45db_is_ready();
        //ret = at45db_check_id();
        return ret;
}


uint8_t at45db_write_buffer1(uint8_t* data,uint8_t len, uint8_t pos){
        uint8_t ret=0;
        uint32_t address =  pos ;   // position into the buffer
        uint8_t cmd[4] ={0};
        cmd[0] = CMD_WRITEBUFF1;
        cmd[1] = (address >> 24) & 0xFF;
        cmd[2] = (address >> 16) & 0xFF;
        cmd[3] = (address >> 8)  & 0xFF;
        gpio_write(0);
        spi_write(&cmd,4);
        delay(1);
        spi_write(data,len);
        gpio_write(1);
        at45db_wait(AT45DB_TIMEOUT);
        return ret;
}




uint8_t at45db_read_buffer1(uint8_t* data,uint8_t len, uint8_t pos){
        uint8_t ret=0;
        uint32_t address =  pos ;   // position into the buffer
        uint8_t cmd[4] ={0};
        cmd[0] = CMD_READBUFF1;
        cmd[1] = (address >> 24) & 0xFF;
        cmd[2] = (address >> 16) & 0xFF;
        cmd[3] = (address >> 8)  & 0xFF;
        gpio_write(0);
        spi_write(&cmd,4);
        spi_read(data,len);
        gpio_write(1);
        at45db_wait(AT45DB_TIMEOUT);
        return ret;
}

















uint8_t at45db_write_page(uint8_t* data, uint8_t len, uint16_t pag,uint8_t pos){
        uint8_t ret=0;
        uint8_t _len = len ;  // 
        uint32_t address =  (pag << 9) | pos ;   // position into the buffer
        uint8_t cmd[4] ={0};
        cmd[0] = CMD_WRITEPAGE_B1;
        cmd[1] = (address >> 24) & 0xFF;
        cmd[2] = (address >> 16) & 0xFF;
        cmd[3] = (address >> 8)  & 0xFF;
        gpio_write(0);
        spi_write(&cmd,4);
        delay(1);
        spi_write(&_len,1);
        delay(1);
        spi_write(data,_len);
        delay(1);
        gpio_write(1);
        at45db_wait(AT45DB_TIMEOUT);
        return ret;        
}



uint8_t at45db_read_page(uint8_t* data, uint8_t len, uint16_t pag,uint8_t pos){
       uint8_t ret=0;
        uint32_t address =  (pag << 9) | pos ;   
        uint8_t cmd[5] ={0};
        uint8_t _len = 0;
        cmd[0] = CMD_READPAGEHF;
        cmd[1] = (address >> 24) & 0xFF;
        cmd[2] = (address >> 16) & 0xFF;
        cmd[3] = (address >> 8)  & 0xFF;
        cmd[4] = dummyByte;
        gpio_write(0);
        spi_write(&cmd,5);
        delay(1);
        spi_read(&_len,1);
        delay(1);
        spi_read(data,_len);
        delay(1);
        gpio_write(1);
        //wait
        at45db_wait(AT45DB_TIMEOUT);
        return ret;        
}









status_t at45db_save_measure(uint8_t* buffer){
        status_t ret;
        at45db_resumen();
        at45db_write_page(buffer,strlen(buffer+1),0,0);
        at45db_sleep();        
        return ret;
}


status_t at45db_download_measure(uint8_t* buffer, uint8_t len){
        status_t ret;
        at45db_resumen();
        ret = at45db_read_page(buffer,len,0,0);
        at45db_sleep();     
        return ret;
}


uint8_t at45db_full_erase_memory(){
        uint8_t ret =0;
        uint8_t cmd[4]={0};
        cmd[0] = 0xC7;
        cmd[1] = 0x94;
        cmd[2] = 0x80;
        cmd[3] = 0x9a;
        gpio_write(0);
        spi_write(&cmd,4);
        gpio_write(1);
        at45db_wait(2500);
        return ret;
}


void at45db_resumen(){
        uint8_t ret =0;
        uint8_t cmd=CMD_RESUMEN;
        gpio_write(0);
        spi_write(&cmd,1);
        gpio_write(1);
        return ret;
}


void at45db_sleep(){
        uint8_t ret =0;
        uint8_t cmd=CMD_LOWPOWER;
        gpio_write(0);
        spi_write(&cmd,1);
        gpio_write(1);
        return ret;
}
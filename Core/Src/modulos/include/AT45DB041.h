/**
 * @file AT45DB041.h
 * @author your name (you@domain.com)
 * @brief  Driver for AT45DB041E memory flash extern (SPI) LOW LEVEL
 * @version 0.1
 * @date 2022-08-24
 * 
 * @copyright Copyright (c) 2022
 * 
 *
 */


#include <stdio.h>



// Private functions 


typedef enum {
    STATUS_ERROR = 0,
    STATUS_OK
} status_t;

typedef enum {
    LEVEL_LOW =0,
    LEVEL_HIGH
} level_t;


typedef enum{
    SIZE_PAGE_256 = 0,
    SIZE_PAGE_264 = 1
} size_page_t;





/**
 * @brief Get the status object
 * 
 * @return ** uint8_t 
 */
 uint8_t get_status(void);



/**
 * @brief check the status register. If the device is bussy return 0 else return 1 
 * 
 * @return ** uint8_t 
 */
 uint8_t is_ready(void);




/**
 * @brief  The memory page size for default its 264 bytes per page. 
 * 
 * @param size 265 bytes or 256 bytes
 * @return ** uint8_t 
 */
 uint8_t at45db_set_size_page( size_page_t size);




/**
 * @brief Write in buffer 1, SRAM....volatile
 * 
 * @param data  bytes array 
 * @param len   lenght of the array
 * @param pos   Initial position into the buffer1
 * @return ** uint8_t 
 */
 uint8_t write_buffer1(uint8_t* data,uint16_t len, uint16_t pos);



/**
 * @brief Read en buffer 1, en SRAM ...volatile
 * 
 * @param data  bytes array
 * @param len  lenght of the array
 * @param pos Initial position into the buffer1
 * @return ** uint8_t 
 */
uint8_t read_buffer1(uint8_t* data,uint16_t len, uint16_t pos);





/**
 * @brief 
 * 
 * @param data  data to write in page. You can write more than one page
 * @param len   length of the data
 * @param pag   page to write. AT45DB041E has 2048 pages
 * @param pos   Initial position in the page
 * @return ** uint8_t 
 */
uint8_t write_page(uint8_t* data, uint16_t len, uint16_t pag,uint16_t pos);


/**
 * @brief 
 * 
 * @param data  buffer to use. You can read more than one page
 * @param len   length of the data
 * @param pag   page to read. AT45DB041E has 2048 pages
 * @param pos   Initial position in the page
 * @return ** uint8_t  
 */
uint8_t read_page(uint8_t* data, uint16_t len, uint16_t pag,uint16_t pos);


/**
 * @brief Resume the device from the sleep mode
 * 
 * @return ** void 
 */
void at45_resumen();

/**
 * @brief  Sleep mode
 * 
 * @return ** void 
 */
void at45_sleep();
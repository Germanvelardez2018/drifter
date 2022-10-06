#include "main.h"
#include "AT45DB041.h"
#include "string.h"
#include "mmap.h"


PRIVATE status_t inline mem_resume(){
    at45db_resumen();
}


PRIVATE status_t inline mem_sleep(){
    at45db_sleep();
}


PRIVATE void inline mem_init(){
    at45db_init();
    
}


PRIVATE status_t mem_write_page(uint8_t* data, uint8_t len, uint16_t pag,uint8_t pos){
   status_t ret = STATUS_ERROR;
   at45db_write_page(data,  len, pag, pos);
   return ret;
}


PRIVATE status_t mem_read_page(uint8_t* data, uint8_t len, uint16_t pag,uint8_t pos){
   status_t ret = STATUS_ERROR;
   ret =  at45db_read_page(data,len,pag,pos);
    return ret;
}


PRIVATE status_t mem_read_buffer(uint8_t* data, uint8_t len,uint8_t pos){
   status_t ret = STATUS_ERROR;
   ret =  at45db_read_buffer2(data,len,pos);
    return ret;
}


PRIVATE status_t mem_write_buffer(uint8_t* data, uint8_t len,uint8_t pos){
   status_t ret = STATUS_ERROR;
   ret =  at45db_write_buffer2(data,len,pos);
    return ret;
}


status_t   write_data(uint8_t* buffer, uint32_t page){
    status_t ret = STATUS_ERROR;
    uint8_t len =  strlen(buffer)+1;
    uint8_t _len = len;
    at45db_resumen();
    ret = at45db_write_page(&(_len),1,(page+ MMAP_DATA_OFSSET),0);
    ret = at45db_write_page(buffer ,len,(page+ MMAP_DATA_OFSSET),1);
    at45db_sleep();
    return ret;
}


status_t   read_data(uint8_t* buffer, uint32_t page){
    status_t ret = STATUS_ERROR;
    uint8_t len=0;
    at45db_resumen();
    at45db_read_page((&len),1,(page + MMAP_DATA_OFSSET),0);
    at45db_read_page(buffer ,len,(page + MMAP_DATA_OFSSET),1);
    at45db_sleep();
    return ret;
}


PRIVATE status_t mem_get_counter(uint8_t* counter){
   status_t ret = STATUS_OK;
   #define ANYOFFSET            2
    ret = mem_read_buffer(counter, 1,ANYOFFSET);
   return ret;
}


PRIVATE status_t  mem_set_counter(uint8_t* counter){
    status_t ret = STATUS_OK;
    ret = mem_write_buffer(counter, 1,ANYOFFSET);
    return ret;
}

//------------------------------------------------------

status_t mem_s_init(){
    status_t ret = STATUS_OK;
    // Iniciar memoria
    mem_init();
    modulo_debug_print("memoria iniciada \r\n");
    mem_resume();
    return ret;
}


status_t mem_s_get_fsm_state(uint8_t* fsm_state){
    status_t ret= STATUS_OK;
    mem_resume();
    ret = mem_read_page(fsm_state,1,MMAP_FSM_STATE,0);
    mem_sleep();
    return ret;
}


status_t mem_s_set_fsm_state(uint8_t* fsm_state){
    status_t ret= STATUS_OK;
    mem_resume();
    ret = mem_write_page(fsm_state,1,MMAP_FSM_STATE,0);
    mem_sleep();
    return ret;
}


status_t mem_s_set_counter(uint8_t* counter){
    status_t ret = STATUS_ERROR;
    at45db_resumen();
    ret = mem_set_counter(counter);
    at45db_sleep();
    return ret;
}


status_t mem_s_get_counter(uint8_t* counter){
    status_t ret = STATUS_ERROR;
    at45db_resumen();
    ret = mem_get_counter(counter);
    at45db_sleep();
    return ret;
}


// Interval
status_t mem_s_get_interval(uint8_t* interval){
    status_t ret = STATUS_OK;
    mem_resume();
    ret = mem_read_page(interval,1,MMAP_INTERVAL,0);
    // Si ocurre error de lectura, enviar valor por default
    mem_sleep();
    return ret;
}


status_t mem_s_set_interval(uint8_t* interval){
    status_t ret = STATUS_OK;
    mem_resume();
    ret = mem_write_page(interval,1,MMAP_INTERVAL,0);
    mem_sleep();
    return ret;
}


status_t mem_s_get_max_amount_data(uint8_t* max_amount_data){
    status_t ret = STATUS_OK;
    mem_resume();
    ret = mem_read_page(max_amount_data,1,MMAP_MAX_AMOUNT_DATA,0);
    mem_sleep();
    return ret;
}


status_t mem_s_set_max_amount_data(uint8_t* max_amount_data){
     status_t ret = STATUS_OK; 
     mem_resume();
     ret = mem_write_page(max_amount_data,1,MMAP_MAX_AMOUNT_DATA,0);    
     mem_sleep();
    return ret;
}



status_t mem_s_get_max_accelerometer_offset(int16_t* x,int16_t* y, int16_t* z ){
   status_t ret = STATUS_OK;
   #define MEM_SIZE  (6)
   uint8_t data[MEM_SIZE]={0};
   ret = mem_read_page(data,6,MMAP_OFFSET_ACELEROMETER,0);
   // Si ocurre error de lectura, enviar valor por default
   if(ret == STATUS_ERROR){
        (*x) = MMAP_DEFAULT_OFFSET_ACCELEROMETER;
        (*y) = MMAP_DEFAULT_OFFSET_ACCELEROMETER;
        (*z) = MMAP_DEFAULT_OFFSET_ACCELEROMETER;
    } else{
        (*x) = (int16_t)(data[0] << 8 | data [1]);
        (*x) = (*x);
        (*y) = (int16_t)(data[2] << 8 | data [3]);
        (*y) = (*y);
        (*z) = (int16_t)(data[4] << 8 | data [5]);   
        (*z) = (*z);

    }
    return ret;
}


status_t mem_s_set_max_accelerometer_offset(int16_t* x, int16_t* y,int16_t* z){
    status_t ret = STATUS_OK;
    #define MEM_SIZE  (6)
    uint8_t data[MEM_SIZE]={0};
    uint8_t* p_data;
    data[0] = ((*x) >> 8) & 0xff;
    data[1] = (*x) & 0xff;
    data[2] = ((*y) >> 8) & 0xff;
    data[3] = (*y) & 0xff;
    data[4] = ((*z) >> 8) & 0xff;
    data[5] = (*z) & 0xff;  
    // |   x H L       |      y H L           |       z H L          |
   ret = mem_write_page(data,MEM_SIZE,MMAP_OFFSET_ACELEROMETER,0);
   // Si ocurre error de lectura, enviar valor por default    
    return ret;
}





void mem_full_clear(){
    at45db_full_erase_memory();
}



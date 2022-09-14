#include "main.h"
#include "AT45DB041.h"
#include "string.h"
#include "mmap.h"



PRIVATE void delay(uint32_t timeout){

}

PRIVATE status_t mem_write_page(uint8_t* data, uint16_t len, uint16_t pag,uint16_t pos){

}


PRIVATE status_t mem_read_page(uint8_t* data, uint16_t len, uint16_t pag,uint16_t pos){

}


PRIVATE status_t mem_read_buffer(uint8_t* data, uint16_t len){

}


PRIVATE status_t mem_write_buffer(uint8_t* data, uint16_t len){

}


PRIVATE status_t mem_resume(){

}


PRIVATE status_t mem_sleep(){

}




//------------------------------------------------------


status_t mem_s_init(){
    status_t ret = STATUS_OK;
    //iniciar periferico y pines gpio

    // Iniciar memoria
    
    //sleep memoria



    
    return ret;
}


// Interval
status_t mem_s_get_interval(uint32_t* interval){
    status_t ret = STATUS_OK;
    ret = mem_read_page(interval,1,MMAP_INTERVAL,0);
    // Si ocurre error de lectura, enviar valor por default
    if(ret == STATUS_ERROR){
        (*interval) = MMAP_DEFAULT_INTERVAL;
    } 
    return ret;
}


status_t mem_s_set_interval(uint32_t* interval){
    status_t ret = STATUS_OK;
    ret = mem_write_page(interval,1,MMAP_INTERVAL,0);
    return ret;
}


status_t mem_s_get_max_amount_data(uint8_t* max_amount_data){
    status_t ret = STATUS_OK;
    ret = mem_read_page(max_amount_data,1,MMAP_MAX_AMOUNT_DATA,0);
    // Si ocurre error de lectura, enviar valor por default
    if(ret == STATUS_ERROR){
        (*max_amount_data) = MMAP_DEFAULT_MAX_AMOUNT_DATA;
    } 
    return ret;
}


status_t mem_s_set_max_amount_data(uint8_t* max_amount_data){
     status_t ret = STATUS_OK;
      ret = mem_write_page(max_amount_data,1,MMAP_MAX_AMOUNT_DATA,0);    
    return ret;
}



// ?? Tiene sentido mantener estado de bateria en memoria?, Cuando demoro en medir baterias

status_t mem_s_get_battery_state(battery_state_t* battery_state){
    status_t ret = STATUS_OK;
    ret = mem_read_page(battery_state,1,MMAP_F_BATTERY_STATE,0);
    // Si ocurre error de lectura, enviar valor por default
    if(ret == STATUS_ERROR){
        (*battery_state) = MMAP_DEFAULT_BATTERY;
    } 
    return ret;
}



status_t mem_s_set_battery_state(battery_state_t* battery_state){
    status_t ret = STATUS_OK;
    ret = mem_write_page(battery_state,1,MMAP_F_BATTERY_STATE,0);
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

    // se guarda primero H despues L
// REVISAR
   
    //data[0] = (int8_t)(x >> 8 );
    //data[1] = ( int8_t )(x); 
    //data[2] = (int8_t)(y >> 8 );
    //data[3] = ( int8_t )(y); 
    //data[4] =  (int8_t)(z >> 8 );
    //data[5] = ( int8_t )(z); 
    

   
   ret = mem_write_page(data,MEM_SIZE,MMAP_OFFSET_ACELEROMETER,0);
   // Si ocurre error de lectura, enviar valor por default

  
   

    
    return ret;
}



#include "main.h"
#include "AT45DB041.h"
#include "string.h"
#include "mmap.h"


// Contador de memorias almacenadas. Se guarda en SRAM del buffer de memoria
PRIVATE uint8_t counter = 0;



PRIVATE void delay(uint32_t timeout){
    HAL_Delay(timeout);
}

PRIVATE status_t mem_write_page(uint8_t* data, uint8_t len, uint16_t pag,uint8_t pos){
   status_t ret = STATUS_ERROR;
   ret = at45db_write_page(data,  len, pag, pos);
   return ret;

}


PRIVATE status_t mem_read_page(uint8_t* data, uint8_t len, uint16_t pag,uint8_t pos){
   status_t ret = STATUS_ERROR;
   ret =  at45db_read_page(data,len,pag,pos);
    return ret;
}


PRIVATE status_t mem_read_buffer(uint8_t* data, uint8_t len,uint8_t pos){
   status_t ret = STATUS_ERROR;
   ret =  at45db_write_buffer1(data,len,pos);
    return ret;
}


PRIVATE status_t mem_write_buffer(uint8_t* data, uint8_t len,uint8_t pos){
   status_t ret = STATUS_ERROR;
   ret =  at45db_read_buffer1(data,len,pos);
    return ret;
}



PRIVATE status_t mem_load_string(uint8_t* string , uint8_t len, uint16_t pag){
    status_t ret = STATUS_ERROR;
    // Formato, primer byte es len, luego buffer string
    uint8_t _len = len;
    ret = at45db_write_page(_len,1,pag,0);
    ret = at45db_write_page(string ,len,pag,1);
    return ret;

}



PRIVATE status_t mem_download_string(uint8_t* string , uint16_t pag){
    status_t ret = STATUS_ERROR;
    // Formato, primer byte es len, luego buffer string
    uint8_t _len = 0;
    ret = at45db_read_page(_len,1,pag,0);
    ret = at45db_read_page(string ,_len,pag,1);
    return ret;

}

PRIVATE status_t mem_resume(){

}


PRIVATE status_t mem_sleep(){

}



PRIVATE status_t mem_get_counter(){

}

PRIVATE status_t mem_set_counter(uint8_t* counter){

}



//------------------------------------------------------


status_t mem_s_init(){
    status_t ret = STATUS_OK;
    //iniciar periferico y pines gpio

    // Iniciar memoria

    // Cargar contador
    ret = mem_get_counter(counter);
    // si obtengo error en lectura, cargo valor por default
    if( ret == STATUS_ERROR) counter = MMAP_DEFAULT_COUNTER;
    
    //sleep memoria
    return ret;
}


// Interval
status_t mem_s_get_interval(uint8_t* interval){
    status_t ret = STATUS_OK;
    ret = mem_read_page(interval,1,MMAP_INTERVAL,0);
    // Si ocurre error de lectura, enviar valor por default
    if(ret == STATUS_ERROR){
        (*interval) = MMAP_DEFAULT_INTERVAL;
    } 
    return ret;
}


status_t mem_s_set_interval(uint8_t* interval){
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









// Los datos se guardan como lifo, ultimo entrar primero en salir
status_t mem_s_load_data(uint8_t* string ){
    status_t ret = STATUS_ERROR;
    // verifico parametros
    if(string == NULL)          return ret;
    uint16_t len = strlen(string) + 1;
    if(len > 255)    return ret;
    // cargo contador
    uint8_t counter = MMAP_DEFAULT_COUNTER;
    ret = mem_get_counter(counter);
    if(ret == STATUS_ERROR) return ret;
    // escribo en pagina correspondiente
    ret = mem_load_string(string,len,counter);
    // incremento contador
    counter ++;
    // guardo contador
    mem_set_counter(&counter);    
    return ret;
}


status_t mem_s_download_data(uint8_t* buffer){
    status_t ret = STATUS_ERROR;
    uint16_t counter = MMAP_DEFAULT_COUNTER;
    ret = mem_get_counter(&counter);
    // No se puede leer el contador, error
    if(ret == STATUS_ERROR) return ret;
    ret = mem_download_string(buffer, counter);
    if( ret == STATUS_OK){
        counter --;
        ret = mem_set_counter(&counter);
        if(counter == 0 ) return STATUS_ERROR;

    } 
    return ret;
}


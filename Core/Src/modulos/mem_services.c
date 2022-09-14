#include "main.h"
#include "AT45DB041.h"
#include "string.h"




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
    return ret;
}
status_t mem_s_set_interval(uint32_t* interval){
    status_t ret = STATUS_OK;
    return ret;
}


status_t mem_s_get_max_amount_data(uint32_t* max_amount_data){
    status_t ret = STATUS_OK;
    return ret;
}
status_t mem_s_set_max_amount_data(uint32_t* max_amount_data){
    status_t ret = STATUS_OK;
    return ret;
}


status_t mem_s_get_battery_state(battery_state_t* battery_state){
    status_t ret = STATUS_OK;
    return ret;
}
status_t mem_s_set_battery_state(battery_state_t* battery_state){
    status_t ret = STATUS_OK;
    return ret;
}


status_t mem_s_get_max_accelerometer_offset(int16_t* x,int16_t* y, int16_t* z ){
    status_t ret = STATUS_OK;
    return ret;
}
status_t mem_s_set_max_accelerometer_offset(int16_t* x, int16_t* y,int16_t* z){
    status_t ret = STATUS_OK;
    return ret;
}


status_t mem_s_get_max_data(uint8_t* data_buffer){
    status_t ret = STATUS_OK;
    return ret;
}
status_t mem_s_set_max_data(uint8_t* data_buffer){
    status_t ret = STATUS_OK;
    return ret;
}

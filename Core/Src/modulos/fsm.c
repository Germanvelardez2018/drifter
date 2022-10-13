#include "fsm.h"
#include "power_save.h"
#include "mem_services.h"

PRIVATE fsm_state_t __DEVICE_STATE_IN_SRAM__ = FSM_UNDEFINED; // Antes de inicializa




PRIVATE fsm_state_t fsm_get_state_from_flash(){
    // Funcion dummy por el momento
    status_t ret = STATUS_ERROR;
    uint8_t buf_state = 0;
    ret = mem_s_get_fsm_state(&buf_state);
            

    fsm_state_t state = FSM_ERROR;  
      
    
    if(ret == STATUS_ERROR){
          modulo_debug_print("ret con error\n");
          return state;


    } 
    switch(buf_state)
    {
    case 1:
        state = FSM_ON_FIELD;
        break;
    case 2:
        state = FSM_MEMORY_DOWNLOAD;
        break;
    case 255:
        state = FSM_UNDEFINED;
        break;
    
    default:
        state = FSM_ERROR;
        break;
    }
    modulo_debug_print("final  de fsm mem\n");
    return state;
}


PRIVATE status_t fsm_set_state_into_flash(fsm_state_t new_state){
    // Funcion dummy por el momento
    status_t ret = STATUS_ERROR;
    uint8_t buf_state = 0;
    ret = mem_s_get_fsm_state(&buf_state);    
    switch (new_state)
    {
    case FSM_UNDEFINED:
            buf_state = 0xff;
    break;
    case  FSM_ON_FIELD:
            ret = STATUS_OK;
            buf_state = 1;
    break;
    case FSM_MEMORY_DOWNLOAD:
            buf_state = 2;
            ret = STATUS_OK;

    break;
    
    default:
    break;
    }

    ret = mem_s_set_fsm_state(&buf_state);
    return ret;

}


status_t fsm_init(){
    status_t ret = STATUS_OK;
    __DEVICE_STATE_IN_SRAM__ = FSM_UNDEFINED;
    // Leo estado desde flash
    //fsm_state_t state_from_flash = fsm_get_state_from_flash();
    fsm_state_t state_from_flash = FSM_ON_FIELD;
    // Si es estado valido, actualizo sram
    if(state_from_flash != FSM_ERROR){
        __DEVICE_STATE_IN_SRAM__ = state_from_flash;
        ret = STATUS_ERROR;
    }
      

    return ret;
}


status_t fsm_load_from_flash(){
     status_t ret = STATUS_ERROR;
    // Leo el estado de la maquina desde memoria no volatil
    //cargo este valor en el estado sram
    return ret;
}


status_t fsm_set_state( fsm_state_t new_state){
    status_t ret = STATUS_ERROR;
    // Cargo el nuevo estado en memoria
    ret = fsm_set_state_into_flash(new_state);
    // Si no tenemos error actualizo estado en sram
    if(ret == STATUS_OK)__DEVICE_STATE_IN_SRAM__ = new_state;
    return ret;
}


 fsm_state_t fsm_get_state(void){
    return __DEVICE_STATE_IN_SRAM__;
 }









































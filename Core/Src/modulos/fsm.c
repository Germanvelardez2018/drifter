#include "fsm.h"
#include "power_save.h"
#include "mem_services.h"


#define   FSM_STATE_DEFAULT                        (FSM_ON_FIELD)

PRIVATE fsm_state_t __DEVICE_STATE_IN_SRAM__ = FSM_STATE_DEFAULT; // Antes de inicializa







PRIVATE fsm_state_t fsm_get_state_from_flash(){
    // Funcion dummy por el momento
    fsm_state_t state = FSM_ON_FIELD;
    uint8_t buf_state = 0;
    mem_s_get_fsm_state(&buf_state);

    // if state es 1, entonces state = DOWNLOAD
    if(buf_state == 1) state = FSM_MEMORY_DOWNLOAD;
    return state;
}


PRIVATE void fsm_set_state_into_flash(fsm_state_t state){
    uint8_t value = (state == FSM_MEMORY_DOWNLOAD)? 1 : 0;
    mem_s_set_fsm_state(&value);

}


status_t fsm_init(){
    status_t ret = STATUS_OK;
    // Leo estado desde flash
    __DEVICE_STATE_IN_SRAM__ = fsm_get_state_from_flash();
  
  
  
    return ret;
}








status_t fsm_set_state( fsm_state_t new_state){
    status_t ret = STATUS_ERROR;
    // Cargo el nuevo estado en memoria
    uint8_t value = (new_state == FSM_MEMORY_DOWNLOAD)? 1 : 0;
    mem_s_set_fsm_state(&value);    // Si no tenemos error actualizo estado en sram
    __DEVICE_STATE_IN_SRAM__ = new_state;
    return ret;
}


 fsm_state_t fsm_get_state(void){
 // Funcion dummy por el momento
    fsm_state_t state = FSM_ON_FIELD;
    uint8_t buf_state = 0;
    mem_s_get_fsm_state(&buf_state);

    // if state es 1, entonces state = DOWNLOAD
    if(buf_state == 1) state = FSM_MEMORY_DOWNLOAD;
    return state;    
 }




































#include "fsm.h"
#include "power_save.h"


PRIVATE fsm_state_t __DEVICE_STATE_IN_SRAM__ = FSM_UNDEFINED; // Antes de inicializa




PRIVATE delay(uint32_t time){
    HAL_Delay(time);
}

PRIVATE fsm_state_t fsm_get_state_from_flash(){
    // Funcion dummy por el momento
    fsm_state_t state = FSM_ON_FIELD;
    return state;
}


PRIVATE status_t fsm_set_state_into_flash(fsm_state_t new_state){
    status_t ret= STATUS_OK;
    // Logica para cargar estado en flash
    return ret;
}


status_t fsm_init(){
    status_t ret = STATUS_OK;
    __DEVICE_STATE_IN_SRAM__ = FSM_UNDEFINED;
    // Leo estado desde flash
    status_t state_from_flash = fsm_get_state_from_flash();
    // Si es estado valido, actualizo sram
    if(state_from_flash != FSM_ERROR){
        __DEVICE_STATE_IN_SRAM__ = state_from_flash;
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










































#include "SIM7000G.h"
#include "uart.h"
#include <string.h>



#define CMD_AT                                        "AT\r\n"
#define  CMD_OK                                       "OK\r\n"
#define CMD_VERSION                                   "ATI\r\n"
#define CMD_ECHO_ON                                   "ATE1\r\n"
#define CMD_ECHO_OFF                                  "ATE0\r\n"
#define CMD_GET_SIGNAL                                "AT+CSQ\r\n"
#define CMD_PWR_GPS_ON                                "AT+CGNSPWR=1\r\n"
#define CMD_PWR_GPS_OFF                               "AT+CGNSPWR=0\r\n"
#define CMD_GETGPSINFO                                "AT+CGNSINF\r\n"
#define CMD_GETOPERATOR                               "AT+COPS?\r\n"



//  !APNS
//  !PERSONAL
//!  APN_PERSONAL                 "datos.personal.com"
//!  APN_USR_PASS_PERSONAL        "datos"
//  !TUENTI
//!  APN_TUENTI                   "internet.movil"
//!  APN_USER_PASS_TUENTI         "internet"


#define CMD_OPEN_APN_TUENTI                          "AT+CNACT=1,\"internet.movil\"\r\n"
#define CMD_OPEN_APN_PERSONAL                        "AT+CNACT=1,\"datos.personal.com\"\r\n"
#define CMD_GET_APN                                  "AT+CNACT?"       


// ! Modo sleep y resume
#define CMD_LOW_PWR_ON                              "AT+CPSMS=1\r\n"
#define CMD_LOW_PWR_OFF                             "AT+CPSMS=0\r\n"
      

//! Configuracion MQTT
 #define CMD_MQTT                                    "AT+SMCONF="
 #define CMD_MQTT_URL                                " \"URL\""
 #define CMD_MQTT_USER                               "\"USERNAME\""
 #define CMD_MQTT_PASSWORD                           "\"PASSWORD\""
 #define CMD_MQTT_QOS                                "\"QOS\""
 #define CMD_MQTT_COMMIT                             "AT+SMCONN\r\n"
 #define CMD_MQTT_PUBLISH                            "AT+SMPUB=\"%s\",\"%d\",1,1 \r\n" 
#define CMD_MQTT_SUBSCRIBE                           "AT+SMSUB=\"%s\",%d \r\n"   // topic , QoS

#define CMD_MQTT_TOPIC_CONFIG                        "SIMO_CONFIG"



extern UART_HandleTypeDef huart1;


#define SIM_BUFFER_SIZE                               (255)
static uint8_t buffer[SIM_BUFFER_SIZE]={0};
static uint8_t buff_counter =0;

#define SIM7000G_TIMEOUT                             (1000)
#define SIM7000G_UART                                (&huart1)
#define SIM7000G_BUFFER                              (buffer)
#define IS_EQUAL                                     (0)

PRIVATE void delay(uint32_t time){
        HAL_Delay(time);
}








/**
 * @brief Buffer de recepccion
 * 
 * @return ** uint8_t* 
 */
PRIVATE  uint8_t* __comm_get_buffer(){
    return SIM7000G_BUFFER;
}



PRIVATE status_t check_response(char* response){
    status_t ret = STATUS_ERROR;
     if(response == NULL ) return 0;
    uint32_t len_reponse = strlen(response)  ;
    uint32_t len_buffer = strlen(SIM7000G_BUFFER);
    uint32_t index = len_buffer - len_reponse ;  
    ret = (strncmp(&(SIM7000G_BUFFER[index]),response,len_reponse) == IS_EQUAL)?STATUS_OK:ret;
    return ret;

}



PRIVATE status_t uart_write(uint8_t* buffer,uint8_t len){
    status_t ret = (HAL_UART_Transmit(SIM7000G_UART,buffer,len,SIM7000G_TIMEOUT) == HAL_OK)?STATUS_OK:STATUS_ERROR;
    return ret;

}

PRIVATE status_t uart_read(uint8_t* buffer,uint8_t len){
    status_t ret = (HAL_UART_Receive(SIM7000G_UART,buffer,len,SIM7000G_TIMEOUT) == HAL_OK)?STATUS_OK:STATUS_ERROR;
    return ret;
}


PRIVATE status_t send_command(uint8_t* string_cmd,uint8_t* response_expected){

    status_t ret = STATUS_ERROR;
    //Envio comando
    ret  = uart_write(string_cmd,strlen(string_cmd));
    // leo respuesta y almaceno en buffer SIM7000G
    ret  = uart_read(SIM7000G_BUFFER,SIM_BUFFER_SIZE);
    // checkeo buffer rx con respuesta esperada
    ret  = check_response(response_expected) ;
    
    return ret;
}






status_t sim7000g_check(){

    status_t ret = STATUS_ERROR;
    send_command(CMD_AT,CMD_OK);
    
    return ret;
}



status_t sim7000g_init(){
    status_t ret = STATUS_ERROR;
    return ret;
}


status_t sim7000g_set_mqtt_config(){
    status_t ret = STATUS_ERROR;
    return ret;
}



status_t sim7000g_mqtt_publis(){
    status_t ret = STATUS_ERROR;
    return ret;
}

status_t sim7000g_set_mqtt_subscribe(){
    status_t ret = STATUS_ERROR;
    return ret;
}

status_t sim7000g_set_mqtt_dessubscribe(){
    status_t ret = STATUS_ERROR;
    return ret;
}



status_t sim7000g_resume(){
    status_t ret = STATUS_ERROR;
    return ret;
}


status_t sim7000g_sleep(){
    status_t ret = STATUS_ERROR;
    return ret;
}




status_t sim7000g_get_NMEA( uint8_t* buffer, uint8_t len){
    status_t ret = STATUS_ERROR;
    return ret;
}



status_t sim7000g_test(){
    status_t ret = STATUS_ERROR;
    return ret;
}
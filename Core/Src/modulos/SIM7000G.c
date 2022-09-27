
#include "sim7000g.h"
#include "uart.h"
#include "gpio.h"
#include <string.h>
#include "debug.h"




 //  0   ENVIAR UNA SOLA VEZ, SIN CONFIRMACION. SE ENVIA EL MENSAJE UNA VEZ, NO SE ESPERA CONFIRMACION.
 //  1   ENVIAR, CON POSIBLE REENVIO DEPENDIENTE DE CONFIRMACION. SE RECIBE MENSAJE AL MENOS UNA VEZ
 //  2   ENVIAR Y RECIBIR CONFIRMACION, SOLO UNA VEZ. SE RECIBE EL MENSAJE EXACTAMENTE UNA VEZ


 typedef enum {
    MQTT_QOS_ONCE_SEND = 0,
    MQTT_QOS_AT_LEAST_ONCE_RECEIVED = 1,
    MQTT_QOS_ONCE_RECEIVED = 2
 } mqtt_qos;



#define BROCKER_TOPIC_COMMAND                       "SIMO_CMD"



#define CMD_OPEN_APN_TUENTI                          "AT+CNACT=1,\"internet.movil\"\r\n"
#define CMD_OPEN_APN_PERSONAL                        "AT+CNACT=1,\"datos.personal.com\"\r\n"

#define CMD_GET_APN                                  "AT+CNACT?"       





// ! Modo sleep y resume
#define CMD_LOW_PWR_ON                              "AT+CPSMS=1\r\n"
#define CMD_LOW_PWR_OFF                             "AT+CPSMS=0\r\n"
      





#define  CMD_AT                                        "AT\r\n"
#define  CMD_OK                                       "OK\r\n"
#define  CMD_VERSION                                   "ATI\r\n"
#define  CMD_ECHO_ON                                   "ATE1\r\n"
#define  CMD_ECHO_OFF                                  "ATE0\r\n"
#define  CMD_GET_SIGNAL                                "AT+CSQ\r\n"
#define  CMD_GPS_ON                                "AT+CGNSPWR=1\r\n"
#define  CMD_GPS_OFF                               "AT+CGNSPWR=0\r\n"
#define  CMD_GETGPSINFO                                "AT+CGNSINF\r\n"
#define  CMD_GETOPERATOR                               "AT+COPS?\r\n"



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
#define CMD_MQTT_UMSUBSCRIBE                         "AT+SMUMSUB=\"%s\"\r\n"   // topic , QoS
#define CMD_MQTT_TOPIC_CONFIG                        "SIMO_CONFIG"



extern UART_HandleTypeDef huart1;


#define SIM_BUFFER_SIZE                               (254)
PRIVATE uint8_t buffer[SIM_BUFFER_SIZE]={0};

#define SIM7000G_TIMEOUT                             (500)
#define SIM7000G_UART                                (&huart1)
#define SIM7000G_BUFFER                              (&buffer[0])
#define IS_EQUAL                                     (0)




#define SIM7000G_BAT_ENA_Pin GPIO_PIN_4
#define SIM7000G_BAT_ENA_GPIO_Port GPIOA






#define SIM7000G_PWRKEY_Pin GPIO_PIN_12
#define SIM7000G_PWRKEY_GPIO_Port GPIOA
















PRIVATE void PWRKEY_set(level_t level){
    HAL_GPIO_WritePin(SIM7000G_PWRKEY_GPIO_Port,SIM7000G_PWRKEY_Pin,(GPIO_PinState)level) ;
}


PRIVATE void BAT_ENA_set(level_t level){
    HAL_GPIO_WritePin(SIM7000G_BAT_ENA_GPIO_Port,SIM7000G_BAT_ENA_Pin,(GPIO_PinState)level) ;
}



PRIVATE status_t uart_init(){
    status_t ret = STATUS_OK;
    UART_SIMCOM_init();
    return ret;
}



PRIVATE status_t uart_write(uint8_t* buffer,uint8_t len){
    HAL_StatusTypeDef res = HAL_UART_Transmit(SIM7000G_UART,buffer,len,SIM7000G_TIMEOUT) ;
    status_t ret = (res == HAL_OK)?STATUS_OK:STATUS_ERROR;
    return ret;
}


PRIVATE status_t uart_read(uint8_t* buffer,uint8_t len){
    memset(SIM7000G_BUFFER,0,SIM_BUFFER_SIZE);
    HAL_StatusTypeDef res = HAL_UART_Receive(SIM7000G_UART,buffer,len,SIM7000G_TIMEOUT);
    status_t ret = ( res == HAL_OK)?STATUS_OK:STATUS_ERROR;
    return ret;
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




PRIVATE status_t send_command(uint8_t* string_cmd,uint8_t* response_expected){
    status_t ret = STATUS_ERROR;
    //Envio comando

    modulo_debug_print("cmd enviado:");
    modulo_debug_print(string_cmd);
    ret  = uart_write(string_cmd,strlen(string_cmd));     
    modulo_debug_print("\n");

    // leo respuesta y almaceno en buffer SIM7000G
    uart_read(SIM7000G_BUFFER,SIM_BUFFER_SIZE);
    modulo_debug_print("respuesta leida:");
    modulo_debug_print(SIM7000G_BUFFER);
    modulo_debug_print("\n");


    // checkeo buffer rx con respuesta esperada
    ret  = check_response(response_expected) ;
    modulo_debug_print("respuesta esperada:");
    modulo_debug_print(response_expected);
    modulo_debug_print("\n");

    return ret;
}





status_t sim7000g_read_input(uint8_t len, uint32_t timeout){

    memset(SIM7000G_BUFFER,0,SIM_BUFFER_SIZE);
    HAL_StatusTypeDef res = HAL_UART_Receive(SIM7000G_UART,buffer,len,timeout);
    status_t ret = ( res == HAL_OK)?STATUS_OK:STATUS_ERROR;

    return ret;

}

uint8_t* sim7000g_get_buffer(){
    return SIM7000G_BUFFER;
}



status_t sim7000g_check(){
    status_t ret = STATUS_ERROR;
   
    ret = send_command(CMD_AT,CMD_OK);
    modulo_debug_print("Check SIMG7000G");

    while ( ret == STATUS_ERROR){
        ret = send_command(CMD_VERSION,CMD_OK);
        modulo_debug_print(".");
        delay(1000);
    }
    modulo_debug_print("\n");
    
    ret = send_command(CMD_ECHO_OFF,CMD_OK);
     ret = send_command(CMD_AT,CMD_OK);

    return ret;
}



status_t sim7000g_init(){
    uart_init();
    // Necesario para alimentar la placa y encender el sim7000g   
    status_t ret = STATUS_OK;
    modulo_debug_print("SIM7000G init\n");
    PWRKEY_set(LEVEL_HIGH);
    BAT_ENA_set(LEVEL_HIGH);
    return ret;
}





status_t sim7000g_resume(){
    status_t ret = STATUS_ERROR;
      ret = send_command(CMD_LOW_PWR_OFF,CMD_OK);
    return ret;
}


status_t sim7000g_sleep(){
    status_t ret = STATUS_ERROR;
     ret = send_command(CMD_LOW_PWR_ON,CMD_OK);
    return ret;
}




status_t simg7000g_set_gps(uint8_t value){
    
    status_t ret = STATUS_ERROR;
    if(value == 0){
     ret = send_command(CMD_GPS_OFF,CMD_OK);
    }
    else{
         ret = send_command(CMD_GPS_ON,CMD_OK);
    }
    return ret;
}

status_t sim7000g_get_NMEA( uint8_t* buff, uint8_t len){
    status_t ret = STATUS_ERROR;
    send_command(CMD_GETGPSINFO,CMD_OK);
    uint8_t len_response = strlen(SIM7000G_BUFFER);
    strcpy(buff,SIM7000G_BUFFER);
    return ret;
}



status_t sim7000g_test(){
    status_t ret = STATUS_ERROR;
    return ret;
}





status_t sim7000g_get_operator(){
    return send_command(CMD_GETOPERATOR,CMD_OK);;
}



status_t sim7000g_get_signal(){
    return send_command(CMD_GET_SIGNAL,CMD_OK);
}



status_t sim7000g_open_apn(){
    return send_command(CMD_OPEN_APN_PERSONAL,CMD_OK);
}



status_t sim7000g_set_mqtt_config(uint8_t* url, uint8_t* user, uint8_t* password, uint8_t* qos){
    status_t ret = STATUS_ERROR;
    if ((url == NULL) || ( user == NULL) || (password == NULL) || (qos == NULL)) return ret;
    uint8_t* buffer[255]={0};
    sprintf(buffer,"%s %s,\"%s\" \r\n",CMD_MQTT,CMD_MQTT_URL,url);    
    delay(500);
    ret = send_command(buffer,CMD_OK);
    sprintf(buffer,"%s %s,\"%s\" \r\n",CMD_MQTT,CMD_MQTT_USER,user);    
    delay(500);
    ret = send_command(buffer,CMD_OK);
    sprintf(buffer,"%s %s,\"%s\" \r\n",CMD_MQTT,CMD_MQTT_PASSWORD,password);    
    delay(500);
    ret = send_command(buffer,CMD_OK);
    sprintf(buffer,"%s %s,%d \r\n",CMD_MQTT,CMD_MQTT_QOS,qos);    
    delay(500);
    ret = send_command(buffer,CMD_OK);
    
    
    
    delay(500);
    ret = send_command(CMD_MQTT_COMMIT,CMD_OK);
    delay(1000);
     // Nos subcribimos a topic de configuracion  
sprintf(buffer,CMD_MQTT_SUBSCRIBE,"SIMO_CONFIG",MQTT_QOS_AT_LEAST_ONCE_RECEIVED);    
ret = send_command(buffer,CMD_OK);
  
    return ret;
}




status_t sim7000g_mqtt_publish(uint8_t* topic, uint8_t* payload, uint8_t len_payload){
    status_t ret = STATUS_ERROR;
    uint8_t  buffer[255]={0};
    if( (topic != NULL) || (payload != NULL)){
        sprintf(buffer,CMD_MQTT_PUBLISH,topic,len_payload);    
        ret = send_command(buffer,CMD_OK);
        delay(500);
        ret = send_command(payload,CMD_OK);
    }
    return ret;
}





/**
 *  QoS
 *  0   ENVIAR UNA SOLA VEZ, SIN CONFIRMACION. SE ENVIA EL MENSAJE UNA VEZ, NO SE ESPERA CONFIRMACION.
 *  1   ENVIAR, CON POSIBLE REENVIO DEPENDIENTE DE CONFIRMACION. SE RECIBE MENSAJE AL MENOS UNA VEZ
 *  2   ENVIAR Y RECIBIR CONFIRMACION, SOLO UNA VEZ. SE RECIBE EL MENSAJE EXACTAMENTE UNA VEZ
 * 
 * **/


status_t sim7000g_mqtt_subscription(uint8_t* topic){
    status_t ret = STATUS_ERROR;
    uint8_t  buffer[255]={0};
    if( topic != NULL){
        sprintf(buffer,CMD_MQTT_SUBSCRIBE,topic,MQTT_QOS_AT_LEAST_ONCE_RECEIVED);    
        ret = send_command(buffer,CMD_OK);
    }
    return ret;
}



status_t simg7000g_mqtt_unsubscription(uint8_t* topic){
    status_t ret = STATUS_ERROR;
    uint8_t  buffer[255]={0};
    if(topic != NULL){
        sprintf(buffer,CMD_MQTT_UMSUBSCRIBE,topic);    
        ret = send_command(buffer,CMD_OK);
    }
    return ret;
}




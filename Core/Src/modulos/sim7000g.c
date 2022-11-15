#include "sim7000g.h"
#include "uart.h"
#include "gpio.h"
#include "debug.h"
#include "mem_services.h"
#include "fsm.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 //  0   ENVIAR UNA SOLA VEZ, SIN CONFIRMACION. SE ENVIA EL MENSAJE UNA VEZ, NO SE ESPERA CONFIRMACION.
 //  1   ENVIAR, CON POSIBLE REENVIO DEPENDIENTE DE CONFIRMACION. SE RECIBE MENSAJE AL MENOS UNA VEZ
 //  2   ENVIAR Y RECIBIR CONFIRMACION, SOLO UNA VEZ. SE RECIBE EL MENSAJE EXACTAMENTE UNA VEZ


 typedef enum {
    MQTT_QOS_ONCE_SEND = 0,
    MQTT_QOS_AT_LEAST_ONCE_RECEIVED = 1,
    MQTT_QOS_ONCE_RECEIVED = 2
 } mqtt_qos;



 
extern  UART_HandleTypeDef huart1;
extern  UART_HandleTypeDef huart2;



#define BROCKER_TOPIC_COMMAND                       "SIMO_CMD"

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
#define CMD_GET_APN                                  "AT+CNACT?\r\n"       


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
 #define CMD_MQTT_PUBLISH                            "AT+SMPUB=\"%s\",\"%d\",2,1 \r\n" 
#define CMD_MQTT_SUBSCRIBE                           "AT+SMSUB=\"%s\",%d \r\n"   // topic , QoS
#define CMD_MQTT_UMSUBSCRIBE                         "AT+SMUMSUB=\"%s\"\r\n"   // topic , QoS
#define CMD_MQTT_TOPIC_CONFIG                        "SIMO_CONFIG"
#define CMD_MQTT_CHECK_STATUS                        "AT+SMSTATE? \r\n"
#define CMD_MQTT_KEEK_ALIVE                          "AT+SMCONF=\"KEEPTIME\",60\r\n"

#define SIM7000G_TIMEOUT                             (1000)
#define SIM7000G_UART                                (&huart1)
#define SIM7000G_BUFFER                              (&buffer[0])
#define IS_EQUAL                                     (0)
#define SIM7000G_BAT_ENA_Pin GPIO_PIN_4
#define SIM7000G_BAT_ENA_GPIO_Port GPIOA
#define SIM7000G_PWRKEY_Pin GPIO_PIN_12
#define SIM7000G_PWRKEY_GPIO_Port GPIOA
#define SIM_BUFFER_SIZE                               (254)
#define COMMAND_SIZE                                    40
#define BUFFER_ID_SIZE                                (100)

#define CHAR_END                                        ('x')



PRIVATE uint8_t buffer[SIM_BUFFER_SIZE]={0};
// Buffer donde se almacena string con comando recibido
PRIVATE uint8_t buffer_counter = 0;
PRIVATE uint8_t buffer_cmd[COMMAND_SIZE]={0};
PRIVATE uint8_t _UPDATE_PARAMS_ = 0;
PRIVATE uint8_t interval = 0 , m = 0;
PRIVATE uint8_t flag_cmd = 0;
PRIVATE uint8_t buffer_id[BUFFER_ID_SIZE]="Sin datos del dispositivo \r\n";


// formato interval, max 
PRIVATE   void get_values(char* b,int8_t* interval, uint8_t* max)
{
    uint8_t ret = 0;
    uint8_t index = 0;
    uint8_t len = 0;
    char *token = strtok(b,",");
    uint8_t n;
     if(token != NULL){
        while(token != NULL){
          n =(uint8_t) atoi(token);
          if(n != 0){
          if(index == 0){
          *interval =n; 
          }else{
          *max = n;
          }
            index ++;
        }     
            token = strtok(NULL, ",");
 }
 }
}



void sim_get_values(char* b,int8_t* i, uint8_t* m){
     get_values(b, i,  m);
}


PRIVATE status_t uart_init(){
    status_t ret = STATUS_OK;
    UART_SIMCOM_init();
    return ret;
}


uint8_t* sim_get_id(void){
    uint8_t counter,interval, max_counter ;
    fsm_state_t state = FSM_ON_FIELD;
    mem_s_get_counter(&counter);
    mem_s_get_max_amount_data(&max_counter);
    mem_s_get_interval(&interval);
    state = fsm_get_state();
    sprintf(buffer_id,"Params: counter:%d, max_counter:%d, interval:%d, last state:%s \r\n",counter,max_counter,interval,((state == FSM_ON_FIELD)?"ON FIELD": "DOWNLOAD"));
    modulo_debug_print(buffer_id);
    return buffer_id;
}






PRIVATE void PWRKEY_set(level_t level){
    HAL_GPIO_WritePin(SIM7000G_PWRKEY_GPIO_Port,SIM7000G_PWRKEY_Pin,(GPIO_PinState)level) ;
}


PRIVATE void BAT_ENA_set(level_t level){
    HAL_GPIO_WritePin(SIM7000G_BAT_ENA_GPIO_Port,SIM7000G_BAT_ENA_Pin,(GPIO_PinState)level) ;
}


PRIVATE status_t uart_write(uint8_t* buffer,uint8_t len,uint32_t timeout){
    HAL_StatusTypeDef res = HAL_UART_Transmit(SIM7000G_UART,buffer,len,timeout) ;
    status_t ret = (res == HAL_OK)?STATUS_OK:STATUS_ERROR;
    return ret;
}


PRIVATE status_t uart_read(uint8_t* buffer,uint8_t len,uint32_t timeout){
    memset(SIM7000G_BUFFER,0,SIM_BUFFER_SIZE);
    HAL_StatusTypeDef res = HAL_UART_Receive(SIM7000G_UART,buffer,len,timeout);
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


PRIVATE status_t send_command(uint8_t* string_cmd,uint8_t* response_expected,uint32_t timeout){
    status_t ret = STATUS_ERROR;
    //Envio comando
    modulo_debug_print("cmd enviado:");
    modulo_debug_print(string_cmd);
    ret  = uart_write(string_cmd,strlen(string_cmd),timeout);     
    modulo_debug_print("\n");
    // leo respuesta y almaceno en buffer SIM7000G
    uart_read(SIM7000G_BUFFER,SIM_BUFFER_SIZE,timeout);
    modulo_debug_print("respuesta leida:");
    modulo_debug_print(SIM7000G_BUFFER);
    modulo_debug_print("\n");
    // checkeo buffer rx con respuesta esperada
    ret  = check_response(response_expected) ;
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
    ret = send_command(CMD_AT,CMD_OK,500);
    modulo_debug_print("Check SIMG7000G\r\n");
    while ( ret == STATUS_ERROR){
        ret = send_command(CMD_VERSION,CMD_OK,500);
        delay(400);
    }
    ret = send_command(CMD_ECHO_OFF,CMD_OK,500);
    ret = send_command(CMD_AT,CMD_OK,500);
    return ret;
}


uint8_t* sim_get_buffer_cmd(){
    return buffer_cmd;
}


void sim_copy_buffer_cmd(uint8_t* buffer_command){
    strncpy(buffer_command,buffer_cmd,buffer_counter+1);
}


uint8_t sim_get_update_params(void){
    return _UPDATE_PARAMS_;
}


void sim_set_update_params(uint8_t value){
    _UPDATE_PARAMS_ = value;
}






 HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
        if(buffer_cmd[buffer_counter] != CHAR_END ){
            buffer_counter++;
            HAL_UART_Receive_IT(&huart1,&(buffer_cmd[buffer_counter]),1);
            HAL_GPIO_TogglePin(LED_GPIO_Port, GPIO_PIN_2);
        }
        else{
                buffer_cmd[buffer_counter]=0;
                sim_set_update_params(1);
            }
        }



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
        HAL_ResumeTick();
        buffer_counter= 0;   
        memset(buffer_cmd,0,COMMAND_SIZE);
        HAL_UART_Receive_IT(&huart1,buffer_cmd,1);
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
      ret = send_command(CMD_LOW_PWR_OFF,CMD_OK,500);
    return ret;
}


status_t sim7000g_sleep(){
    status_t ret = STATUS_ERROR;
     ret = send_command(CMD_LOW_PWR_ON,CMD_OK,500);
    return ret;
}


status_t sim7000g_set_gps(uint8_t value){
    status_t ret = STATUS_ERROR;
    if(value == 0){
     ret = send_command(CMD_GPS_OFF,CMD_OK,500);
    }
    else{
         ret = send_command(CMD_GPS_ON,CMD_OK,500);
    }
    return ret;
}


status_t sim7000g_get_NMEA( uint8_t* buff, uint8_t len){
    status_t ret = STATUS_OK;
    send_command(CMD_GETGPSINFO,CMD_OK,2000);
    uint8_t len_response = strlen(SIM7000G_BUFFER);
    strcpy(buff,SIM7000G_BUFFER);
    return ret;
}


status_t sim7000g_get_operator(){
    return send_command(CMD_GETOPERATOR,CMD_OK,500);
}


status_t sim7000g_get_signal(){
    return send_command(CMD_GET_SIGNAL,CMD_OK,500);
}


status_t sim7000g_open_apn(){
    return send_command(CMD_OPEN_APN_PERSONAL,CMD_OK,500);
}


status_t sim7000g_set_mqtt_config(uint8_t* url, uint8_t* user, uint8_t* password, uint8_t* qos){
    status_t ret = STATUS_ERROR;
    if ((url == NULL) || ( user == NULL) || (password == NULL) || (qos == NULL)) return ret;
    #define MQTT_BUFFER_CONFIG             150
    uint8_t* buffer[MQTT_BUFFER_CONFIG]={0};
    sprintf(buffer,"%s %s,\"%s\" \r\n",CMD_MQTT,CMD_MQTT_URL,url);    
    ret = send_command(buffer,CMD_OK,500);
    delay(500);
    ret = send_command(CMD_MQTT_KEEK_ALIVE,CMD_OK,500);
    delay(500);
    sprintf(buffer,"%s %s,\"%s\" \r\n",CMD_MQTT,CMD_MQTT_USER,user);    
    delay(500);
    ret = send_command(buffer,CMD_OK,500);
    sprintf(buffer,"%s %s,\"%s\" \r\n",CMD_MQTT,CMD_MQTT_PASSWORD,password);    
    delay(500);
    ret= send_command(buffer,CMD_OK,500);
    sprintf(buffer,"%s %s,%d \r\n",CMD_MQTT,CMD_MQTT_QOS,qos);    
    delay(500);
    ret = send_command(buffer,CMD_OK,500);
    delay(500);
    ret = send_command(CMD_MQTT_COMMIT,CMD_OK,2000);
    delay(500);   
    sprintf(buffer,CMD_MQTT_SUBSCRIBE,"CMD",1);    
    ret = send_command(buffer,CMD_OK,500);
    delay(500);
    // configurar la interrupcion del PIN RI SIM7000G
    ret = send_command("AT+CFGRI=1\r\n",CMD_OK,500); // 
    return ret;
}


status_t sim7000g_mqtt_publish(uint8_t* topic, uint8_t* payload, uint8_t len_payload){
    status_t ret = STATUS_ERROR;
    uint8_t  buffer[255]={0};
    if( (topic != NULL) || (payload != NULL)){
        sprintf(buffer,CMD_MQTT_PUBLISH,topic,len_payload);    
        ret = send_command(buffer,CMD_OK,500);
        ret = send_command(payload,CMD_OK,500);
    }
    return ret;
}


status_t sim7000g_mqtt_subscription(uint8_t* topic){
    status_t ret = STATUS_ERROR;
    uint8_t  buffer[100]={0};
        sprintf(buffer,CMD_MQTT_SUBSCRIBE,topic,2);    
        ret = send_command(buffer,CMD_OK,500);
    return ret;
}


status_t sim7000g_mqtt_unsubscription(uint8_t* topic){
    status_t ret = STATUS_ERROR;
    uint8_t  buffer[100]={0};
        sprintf(buffer,CMD_MQTT_UMSUBSCRIBE,topic);    
        ret = send_command(buffer,CMD_OK,500);
    return ret;
}


status_t sim7000g_mqtt_check(){
    status_t ret = STATUS_ERROR;
    ret = send_command(CMD_MQTT_CHECK_STATUS,CMD_OK,500);
    return ret;
}
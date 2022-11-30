#include "sim7000g.h"
#include "uart.h"
#include "gpio.h"
#include "debug.h"
#include "mem_services.h"
#include "fsm.h"
#include "MQTT.h"

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
#define COMMAND_SIZE                                   30
#define BUFFER_ID_SIZE                                (120)

#define CHAR_END                                        ('x')



PRIVATE uint8_t buffer[SIM_BUFFER_SIZE]={0};
// Buffer donde se almacena string con comando recibido
PRIVATE uint8_t buffer_counter = 0;
PRIVATE uint8_t buffer_cmd[COMMAND_SIZE]={0};
PRIVATE uint8_t _UPDATE_PARAMS_ = 0;
PRIVATE uint8_t interval = 0 , m = 0;
PRIVATE uint8_t flag_cmd = 0;
PRIVATE uint8_t buffer_id[BUFFER_ID_SIZE]={0};



PRIVATE uint8_t __SIM_ONLINE__ = 0;

#define PARSE_CHAR                              ","
#define PARSE_ERROR                             (0)
#define CMD_FORMAT                             ("tag , value")
#define PRINT_FLAG                              (1)
#define SEND_COMMAND(CMD,RET,TIMEOUT)               send_command(CMD,RET,TIMEOUT,PRINT_FLAG);    
// Value sera un valor numerico del 0 al 9

/***
 *              Values Table:
 *                      0           Error en parse
 *                      1           1  minuto
 *                      3           5  minutos
 *                      3           10 minutos
 *                      4           30 minutos
 *                      5           60 miuntos
 *                      6           Calibrar el sensor
 *                      6           x
 *                      7           x
 *                      8           x
 *                      9           x
 * 
 *       
*/


// NUEVA FUNCION PARA PARSEAR COMANDO


// SI NO CONECTO CON SERVIDOR, SE DEBER REINICIAR SISTEMA Y VOLVER A INTENTAR

PRIVATE void __SET_ONLINE__(status_t sim_connect){
    __SIM_ONLINE__ = (sim_connect == STATUS_OK)?1:0;
}

PRIVATE uint8_t __GET_ONLINE_STATUS__(){
    return __SIM_ONLINE__;
}


// SI error retorna 0
static uint8_t get_parse(char* string){
    uint8_t ret = PARSE_ERROR;
   // uint8_t buff[50]={0};

 //   sprintf(buffer,"token: %s \r\n",&(string[17]));
 //   modulo_debug_print(buffer);
    ret = (uint8_t) atoi(&(string[17]));
    return ret;
}



uint8_t sim7000g_get_parse(char* string){
    uint8_t ret = get_parse(string);
    return ret;
}




PRIVATE status_t uart_init(){
    status_t ret = STATUS_OK;
    UART_SIMCOM_init();
    return ret;
}



PRIVATE status_t uart_deinit(){
    status_t ret = STATUS_OK;
    UART_SIMCOM_deinit();
    return ret;
}



PRIVATE void PWRKEY_set(level_t level){
    HAL_GPIO_WritePin(SIM7000G_PWRKEY_GPIO_Port,SIM7000G_PWRKEY_Pin,(GPIO_PinState)level) ;
}


PRIVATE void BAT_ENA_set(level_t level){
    HAL_GPIO_WritePin(SIM7000G_BAT_ENA_GPIO_Port,SIM7000G_BAT_ENA_Pin,(GPIO_PinState)level) ;
}


PRIVATE status_t uart_write(uint8_t* buffer,uint8_t len,uint32_t timeout){
    HAL_UART_Abort_IT(SIM7000G_UART);
    HAL_StatusTypeDef res = HAL_UART_Transmit(SIM7000G_UART,buffer,len,timeout) ;
    status_t ret = (res == HAL_OK)?STATUS_OK:STATUS_ERROR;
    return ret;
}


PRIVATE status_t uart_read(uint8_t* buffer,uint8_t len,uint32_t timeout){
    HAL_UART_Abort_IT(SIM7000G_UART);
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


PRIVATE status_t send_command(uint8_t* string_cmd,uint8_t* response_expected,uint32_t timeout, uint8_t print_flag){
    status_t ret = STATUS_ERROR;
    //Envio comando
   if(print_flag)  modulo_debug_print("cmd enviado:");
   if(print_flag)  modulo_debug_print(string_cmd);
    ret  = uart_write(string_cmd,strlen(string_cmd),timeout);     
   if(print_flag)  modulo_debug_print("\n");
    // leo respuesta y almaceno en buffer SIM7000G
    uart_read(SIM7000G_BUFFER,SIM_BUFFER_SIZE,timeout);
   if(print_flag)  modulo_debug_print("respuesta leida:");
   if(print_flag)  modulo_debug_print(SIM7000G_BUFFER);
   if(print_flag)  modulo_debug_print("\n");
    // checkeo buffer rx con respuesta esperada
    ret  = check_response(response_expected) ;
   if(print_flag)  modulo_debug_print("\n");
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
   // ret = SEND_COMMAND(CMD_AT,CMD_OK,500);
    ret = send_command(CMD_AT,CMD_OK,500,0);
    modulo_debug_print("Check SIMG7000G\r\n");
    while ( ret == STATUS_ERROR){
        //ret = SEND_COMMAND(CMD_VERSION,CMD_OK,500);
        ret =send_command(CMD_VERSION,CMD_OK,500,0);
        delay(1000);
    }
    ret = SEND_COMMAND(CMD_ECHO_OFF,CMD_OK,500);
    ret = SEND_COMMAND(CMD_AT,CMD_OK,500);
    return ret;
}


uint8_t* sim_get_buffer_cmd(){
    return buffer_cmd;
}


void sim_copy_buffer_cmd(uint8_t* buffer_command){
        strcpy(buffer_command,buffer_cmd);

}


uint8_t sim_get_update_params(void){
    return _UPDATE_PARAMS_;
}


void sim_set_update_params(uint8_t value){
    _UPDATE_PARAMS_ = value;
}




 HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    HAL_ResumeTick();
    HAL_GPIO_TogglePin(LED_GPIO_Port, GPIO_PIN_2);
    buffer_cmd[18]=0; // quiero evitar la ultima (comilla)"
    sim_set_update_params(1);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    HAL_ResumeTick();
    memset(buffer_cmd, 0, COMMAND_SIZE);
    HAL_UART_Receive_IT(SIM7000G_UART, buffer_cmd, 19);

}

  
status_t sim7000g_init(){
    uart_init();
    // Necesario para alimentar la placa y encender el sim7000g   
    PWRKEY_set(LEVEL_HIGH);
    BAT_ENA_set(LEVEL_HIGH);
    return STATUS_OK;
}


void sim7000g_deinit(){
     uart_deinit();
    
}



status_t sim7000g_resume(){
    status_t ret = STATUS_ERROR;
      ret = SEND_COMMAND(CMD_LOW_PWR_OFF,CMD_OK,500);
    return ret;
}


status_t sim7000g_sleep(){
    status_t ret = STATUS_ERROR;
     ret = SEND_COMMAND(CMD_LOW_PWR_ON,CMD_OK,500);
    return ret;
}


status_t sim7000g_set_gps(uint8_t value){
    status_t ret = STATUS_ERROR;
    if(value == 0){
     ret = SEND_COMMAND(CMD_GPS_OFF,CMD_OK,500);
    }
    else{
         ret = SEND_COMMAND(CMD_GPS_ON,CMD_OK,500);
    }
    return ret;
}


        //+CGNSINF: eliminado de la string

#define OFFSET_GPS                  12      //(strlen("+CGNSINF:"))
#define OFFSET_GPS_END                  8
status_t sim7000g_get_NMEA( uint8_t* buff, uint8_t len){
    status_t ret = STATUS_OK;
    SEND_COMMAND(CMD_GETGPSINFO,CMD_OK,600);
    uint8_t _len = strlen(SIM7000G_BUFFER +12) - OFFSET_GPS_END;
    //strcpy(buff,SIM7000G_BUFFER + 12);
    strncpy(buff,SIM7000G_BUFFER + OFFSET_GPS,_len);
    return ret;
}


status_t sim7000g_get_operator(){
    return SEND_COMMAND(CMD_GETOPERATOR,CMD_OK,500);
}


status_t sim7000g_get_signal(){
    return SEND_COMMAND(CMD_GET_SIGNAL,CMD_OK,500);
}


status_t sim7000g_open_apn(){
    return SEND_COMMAND(CMD_OPEN_APN_PERSONAL,CMD_OK,500);
}


status_t sim7000g_set_mqtt_config(uint8_t* url, uint8_t* user, uint8_t* password, uint8_t* qos){
    status_t ret = STATUS_ERROR;
    if ((url == NULL) || ( user == NULL) || (password == NULL) || (qos == NULL)) return ret;
    #define MQTT_BUFFER_CONFIG             120
    uint8_t* buffer[MQTT_BUFFER_CONFIG]={0};
    sprintf(buffer,"%s %s,\"%s\" \r\n",CMD_MQTT,CMD_MQTT_URL,url);    
    ret = SEND_COMMAND(buffer,CMD_OK,750);
    delay(500);
    ret = SEND_COMMAND(CMD_MQTT_KEEK_ALIVE,CMD_OK,750);
    delay(500);
    #if NEW_BROKER == 0
        sprintf(buffer,"%s %s,\"%s\" \r\n",CMD_MQTT,CMD_MQTT_USER,user);    
        delay(500);
        ret = SEND_COMMAND(buffer,CMD_OK,750);
        sprintf(buffer,"%s %s,\"%s\" \r\n",CMD_MQTT,CMD_MQTT_PASSWORD,password);    
        delay(500);
        ret= SEND_COMMAND(buffer,CMD_OK,750);
    #endif
    sprintf(buffer,"%s %s,%d \r\n",CMD_MQTT,CMD_MQTT_QOS,qos);    
    delay(500);
    ret = SEND_COMMAND(buffer,CMD_OK,750);
    delay(500);
    ret = SEND_COMMAND(CMD_MQTT_COMMIT,CMD_OK,750);
    __SET_ONLINE__(ret); // IMPORTANTE, asigno a la variable ONLINE el estado de la comunicacion
    delay(500);   
    sprintf(buffer,CMD_MQTT_SUBSCRIBE,"CMD",1);    
    ret = SEND_COMMAND(buffer,CMD_OK,750);
    delay(500);
    // configurar la interrupcion del PIN RI SIM7500G
    ret = SEND_COMMAND("AT+CFGRI=1\r\n",CMD_OK,750); // 
    return ret;
}


status_t sim7000g_mqtt_publish(uint8_t* topic, uint8_t* payload, uint8_t len_payload){
    status_t ret = STATUS_ERROR;
    uint8_t  buffer[100]={0};
    if( (topic != NULL) || (payload != NULL)){
        sprintf(buffer,CMD_MQTT_PUBLISH,topic,len_payload);    
        ret = SEND_COMMAND(buffer,CMD_OK,650);
        ret = SEND_COMMAND(payload,CMD_OK,650);
    }
    return ret;
}


status_t sim7000g_mqtt_subscription(uint8_t* topic){
    status_t ret = STATUS_ERROR;
    uint8_t  buffer[100]={0};
        sprintf(buffer,CMD_MQTT_SUBSCRIBE,topic,2);    
        ret = SEND_COMMAND(buffer,CMD_OK,500);
    return ret;
}


status_t sim7000g_mqtt_unsubscription(uint8_t* topic){
    status_t ret = STATUS_ERROR;
    uint8_t  buffer[100]={0};
        sprintf(buffer,CMD_MQTT_UMSUBSCRIBE,topic);    
        ret = SEND_COMMAND(buffer,CMD_OK,500);
    return ret;
}


status_t sim7000g_mqtt_check(){
    status_t ret = STATUS_ERROR;
    ret = SEND_COMMAND(CMD_MQTT_CHECK_STATUS,CMD_OK,500);
    return ret;
}



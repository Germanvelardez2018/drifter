/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "timer.h"
#include "watchdog.h"
#include "adc.h"
#include "power_save.h"
// Modulos
#include "fsm.h"
#include "debug.h"
#include "mpu6050.h"
#include "sim7000g.h"
#include "AT45DB041.h"
#include "MQTT.h"
#include "battery.h" //
#include "mmap.h"
#include "mem_services.h" // esto es lo que va no AT45
/* USER CODE END Includes */

#define COUNTER (0)

#define MAX_COUNTER (10)

// si esta definido default value, recargamos flash con valores default
//#define DEFAULT_VALUES

#define MSG_INIT "test drifter modular \n"
#define SENSOR_FORMAT "{ gps:\n\t %s\n sensor:\n\t %s }"
#define TAG_INIT "SIMO_INIT"
#define DEVICE_ID "device connected \r\n"
#define ID_FORMAT "Dev: counter:%d, max_counter:%d, interval:%d,last state:%s \r\n"
#define FSM_ONFIELD "FSM: ON FIELD\r\n"
#define FSM_DOWNLOAD "FSM: DOWNLOAD\r\n"
#define FSM_CHANGE1 "ON FIELD => DOWNLOAD \r\n"
#define FSM_CHANGE2 "DOWNLOAD => ON FIELD \r\n"

#define CMD_MSG1 "CMD: calibrando dispositivo\r\n "
#define CMD_MSG2 "CMD: configurar intervalo %d\r\n"
#define CMD_MSG7 "CMD: Forzar download \r\n"
#define CMD_MSG8 "CMD: Contador maximo 20 \r\n"
#define CMD_MSG9 "CMD: Contador maximo 50 \r\n"

#define MQTT_SEND_CMD(msg) sim7000g_mqtt_publish(MQTT_TOPIC, msg, strlen(msg));

/* USER CODE END PM */
/* Private variables ---------------------------------------------------------*/

extern UART_HandleTypeDef huart1;

// en wdt.c
extern IWDG_HandleTypeDef hiwdg;
// en timer.c
extern TIM_HandleTypeDef htim1;
// En sim7000g.c
extern DMA_HandleTypeDef hdma_usart2_tx;

PRIVATE fsm_state_t device;

#define DATAFRAME_SIZE 255
PRIVATE uint8_t data_frame_buffer[DATAFRAME_SIZE];
PRIVATE uint8_t buf[255];
PRIVATE uint8_t counter = 0;
PRIVATE pwr_mode_t modo = RUN;
PRIVATE uint8_t flag_params = 0;
PRIVATE uint8_t max_counter;

void inline get_data_frame_to_save(uint8_t *buffer, uint8_t len, uint8_t counter)
{
  uint8_t gps[100] = {0};
  uint8_t sensor[60] = {0};
  memset(buffer, 0, len);
  mpu6050_get_measure(sensor, 60);
  sim7000g_get_NMEA(gps, 100);
  sprintf(buffer, SENSOR_FORMAT, gps, sensor);
  modulo_debug_print(buffer);
  write_data(buffer, counter);
}

void check_flag_params()
{
  static buffer[35];
  flag_params = sim_get_update_params();
  if (flag_params)
  {
    wdt_reset();
    modo = RUN;
    uint8_t cmd[20] = {0};
    memset(cmd, 0, 20);
    sim_copy_buffer_cmd(cmd);
    modulo_debug_print(cmd);
    uint8_t value = sim7000g_get_parse(cmd);
    sprintf(buffer, "\r\nCMD: valor extraido:%d\r\n", value);
    modulo_debug_print(buffer);
    uint8_t interval = 0;
    uint8_t enable = 0;
    uint8_t reset = 1;

    switch (value)
    {
    case 0:
      // ERROR en parse
      modulo_debug_print("error parse\r\n");
      break;
    case 1:
      interval = SLEEP_1MINUTE;
      enable = 1;
      break;
    case 2:
      interval = SLEEP_5MINUTES;
      enable = 1;
      break;
    case 3:
      interval = SLEEP_10MINUTES;
      enable = 1;
      break;
    case 4:
      interval = SLEEP_30MINUTES;
      enable = 1;
      break;
    case 5:
      interval = SLEEP_1HOUR;
      enable = 1;
      break;
    case 6:
      // Get calibration
      modulo_debug_print(CMD_MSG1);
      sprintf(buffer, CMD_MSG1, interval);
      mpu6050_calibrate_and_save_offset();
      break;
    case 7:
      // Forzar envio de datos a la nube
      modulo_debug_print(CMD_MSG7);
      sprintf(buffer, CMD_MSG7, interval);
      device = FSM_MEMORY_DOWNLOAD;
      fsm_set_state(FSM_MEMORY_DOWNLOAD);
      break; // No hacer nada
    case 8:
      // Almacenar hasta 20 datos
      modulo_debug_print(CMD_MSG8);
      sprintf(buffer, CMD_MSG8, interval);
      max_counter = 20;
      mem_s_set_max_amount_data(&max_counter);
      break;
    case 9:
      // Almacenar hasta 50
      modulo_debug_print(CMD_MSG9);
      sprintf(buffer, CMD_MSG9, interval);
      max_counter = 50;
      mem_s_set_max_amount_data(&max_counter);
      break;

    default:
      break;
    }
    sim_set_update_params(0);
    wdt_reset();

    if (enable)
    {
      sprintf(buffer, CMD_MSG2, interval);
      modulo_debug_print(buffer);
      mem_s_set_interval(&interval);
    }

    MQTT_SEND_CMD(buffer);
    
    while(1);

  }
}

static void inline on_field()
{
  // leo el contador
  modulo_debug_print(FSM_ON_FIELD);
  mem_s_get_counter(&counter);
  get_data_frame_to_save(data_frame_buffer, DATAFRAME_SIZE, counter);
  // Aumento contador de muestras almacenadas
  if (counter >= max_counter)
  {
    device = FSM_MEMORY_DOWNLOAD;
    fsm_set_state(FSM_MEMORY_DOWNLOAD);
    modulo_debug_print(FSM_CHANGE1);
    sim7000g_resume();
    MQTT_SEND_CMD(FSM_CHANGE1);
    sim7000g_sleep();

  }
  else
  {
    counter = counter + 1;
    mem_s_set_counter(&counter);
  }
}

static void inline on_download(void)
{
  modulo_debug_print(FSM_DOWNLOAD);
  counter = (max_counter < counter) ? max_counter : counter;
  sprintf(data_frame_buffer, "Extraer :%d datos\n", counter);
  modulo_debug_print(data_frame_buffer);
  MQTT_SEND_CMD(data_frame_buffer);

  while (counter != 0)
  {
    read_data(data_frame_buffer, counter);
   // sim7000g_mqtt_publish(MQTT_TOPIC, data_frame_buffer, strlen(data_frame_buffer));
    MQTT_SEND_CMD(data_frame_buffer);
   // modulo_debug_print(data_frame_buffer);
    wdt_reset();
    HAL_Delay(750);
    counter = counter - 1;
    mem_s_set_counter(&counter);
  }
  wdt_reset();
  mem_s_set_counter(&counter);
  device = FSM_ON_FIELD;
  fsm_set_state(FSM_ON_FIELD);
  modulo_debug_print(FSM_CHANGE2);
  MQTT_SEND_CMD(FSM_CHANGE2);
}





static void app_init()
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  SystemClock_Config();
  // Memoria
  mem_s_init();
  battery_init();
  MX_DMA_Init();
  MX_RTC_Init();
  MX_TIM1_Init();
  //  Necesario para evitar que el micro se reinicio por WDT
  HAL_TIM_Base_Start_IT(&htim1);
  // Inicio modulos
  modulo_debug_init();
  sim7000g_init();
  pwr_init();
  fsm_init();
  mpu6050_init(); //! Sensor
  
}


static void mqtt_config()
{
  uint8_t id[120];
  uint8_t interval;
  // Cargar parametros desde memoria flash
  mem_s_get_max_amount_data(&max_counter);
  mem_s_get_counter(&counter);
  fsm_state_t state = FSM_ON_FIELD;
  mem_s_get_interval(&interval);
  state = fsm_get_state();
  //---------------------------
  sim7000g_check();
  sim7000g_get_signal();
  sim7000g_open_apn();
  sim7000g_get_operator();
  sim7000g_set_gps(1);
  sim7000g_set_mqtt_config(MQTT_URL, MQTT_ID, MQTT_PASS, MQTT_QOS);
  sim7000g_resume();

  sprintf(id, ID_FORMAT, counter, max_counter, interval, ((state == FSM_ON_FIELD) ? "ON FIELD" : "DOWNLOAD"));
  sim7000g_mqtt_publish(TAG_INIT, id, strlen(id));
  gpio_interruption_init();
  sim7000g_sleep();
}

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{



#define TEST_CONSUMO          1



// ANotaciones....micro run con un toggle cada 1 s                            44mA
//                micro run con toggle 1s y encendido elemon board            68mA









  app_init();
  // Sirve para cargar valores por defecto a memoria flash
  modulo_debug_print("init program \r\n");
  mqtt_config();
  // ! Inicia el WDT
  modulo_debug_print("inicio el programa \r\n");



  sim7000g_sleep();

  MX_IWDG_Init();
  device = fsm_get_state();
  while (1)
  {
    modo = pwr_get_mode(); //! Si te levanto una interrupcion por comandos en vez de una IRQ del RTC, volve a dormir
    wdt_reset();
    if (modo == RUN)
    {
      switch (device)
      {
      case FSM_ON_FIELD:
        on_field();
        break;
      case FSM_MEMORY_DOWNLOAD:
        sim7000g_resume();
        on_download();
        sim7000g_sleep();
        break;
      default:
        break;
      }
    }
    check_flag_params();

    pwr_sleep();
  }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

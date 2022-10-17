#include "core.h"
#include "sim7000g.h"







/**
 * @file MQTT.h
 * @author German Velardez (gvelardez@ginti.gob.ar)
 * @brief  Archivos de configuracion del reloj de tiempo real
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */





/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MQTT_H
#define __MQTT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "core.h"
#include "sim7000g.h"



#define MQTT_TOPIC                                   "X1111"
#define MQTT_URL2                                     "simointi.cloud.shiftr.io"
#define MQTT_URL                                  "lseserver.ddns.net"

#define MQTT_PASS                                    "fdZY5b69OhOVsAns"
#define MQTT_ID                                      "simointi"
#define MQTT_QOS                                      2
#define MQTT_CMD_BROKER                              "SIMO_CONFIG"

/*-----------------------------------------------------------------------------*/






#ifdef __cplusplus
}
#endif

#endif /* __MQTT_H */






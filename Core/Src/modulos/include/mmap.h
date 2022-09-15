/**
 * @file mmap.h
 * @author German Velardez (gvelardez@inti.gob.ar)
 * @brief  Mapa de memoria
 * @version 0.1
 * @date 2022-08-24
 * 
 * @copyright Copyright (c) 2022
 * 
 *
 */




// Notas: La memoria AT45DB041E tiene 2048 paginas de 256/264bytes cada una.



// El buffer 1 se usara en las funciones de escritura
// El contador de datos almacenados se guardara en buffer2

// Guardo los primeros 10 registros para usos en configuraciones en el futuro
#define MMAP_OFFSET_CONFIG                                  (10)

// De 0 A 47   // Registros especiales, No Data

//Intervalo en minutos para mediciones
#define MMAP_INTERVAL                                       (MMAP_OFFSET_CONFIG + 1)
// Numero de datos almacenados antes de enviar a servidor
#define MMAP_MAX_AMOUNT_DATA                                (MMAP_OFFSET_CONFIG + 2)
// Flag de estado de conexion
#define MMAP_F_CONNECTION                                   (MMAP_OFFSET_CONFIG + 3)
// Flag de estado de bateria: 3 Normal, 2 medio, 1 baja
#define MMAP_F_BATTERY_STATE                                (MMAP_OFFSET_CONFIG + 4)



// Almacena los 6 bytes del ofsset  del acelerometro. Eje X, eje y, eje z 
#define MMAP_OFFSET_ACELEROMETER                            (MMAP_OFFSET_CONFIG + 10)




// Formato para almacenar en paginar
#define FORMAT_MSG                                          ("\nx:%.2f, y:%.2f , z:%.2f , t:%.2f\n,\nGPS:%s\n")
// De 48 a 2047 // Datos almacenado
#define MMAP_DATA_OFSSET                                    (48)
#define MMAP_DATA_ADDRESS(pos)                              (pos + MMAP_DATA_OFSSET)


#define MMAP_DEFAULT_INTERVAL                               (1)
#define MMAP_DEFAULT_MAX_AMOUNT_DATA                        (20)
#define MMAP_DEFAULT_BATTERY                                ((battery_state_t )NIVEL_MEDIO)
#define MMAP_DEFAULT_OFFSET_ACCELEROMETER                   ((int16_t)0)
#define MMAP_DEFAULT_COUNTER                                (0)

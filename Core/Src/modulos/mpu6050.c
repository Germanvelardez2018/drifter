#include "i2c.h"
#include "MPU6050.h"
#include "debug.h"

extern I2C_HandleTypeDef hi2c2;


#define MPU6050_INSTANCE                            (&hi2c2)
#define MPU6050_ADDRESS                             (0xD0)
#define MPU6050_AVAILABLE                           (0x68)
#define ACCEL_XOUT_H                                (0x3B)
#define ACCEL_XOUT_L                                (0x3C)
#define ACCEL_YOUT_H                                (0x3D)
#define ACCEL_YOUT_L                                (0x3E)
#define ACCEL_ZOUT_H                                (0x3F)
#define ACCEL_ZOUT_L                                (0x40)
#define ACCEL_TOUT_H                                (0x41)
#define ACCEL_TOUT_L                                (0x42)
#define CONFIG                                      (0x1A)
#define GYRO_CONFIG                                 (0x1B)
#define ACCEL_CONFIG                                (0x1C)
#define USER_CTRL                                   (0x6A)
#define SMPLRT_DIV                                  (0x19)
#define INT_PIN_CFG                                 (0x37)
#define INT_ENABLE                                  (0x38)
#define INT_STATUS                                  (0x3A)
#define FIFO_EN                                     (0x23)
#define FIFO_COUNTER_H                              (0x72)
#define FIFO_COUNTER_L                              (0x73)
#define FIFO_BUFFER                                 (0x74)
#define PWR_MGMT_1                                  (0x6B)
#define PWR_MGMT_2                                  (0x6C)
#define WHO_I_AM                                    (0x75) // Default: 0x68



#define MPU6050_TIMEOUT                             (1000)
// Esta definido en el archivo i2c.c (en el metodo de inicializacion)
#define I2C_CLOCK                                   (100000) 


//Nota, al iniciar el dispositivo se calibra suponiendo posicion x=0G, y=0G ,z1G.

PRIVATE int16_t offset_x,offset_y,offset_z ;

PRIVATE float SCALA_DIV = 0;




PRIVATE void delay(uint32_t time){
        HAL_Delay(time);
}


PRIVATE status_t i2c_write_mem( uint8_t address_reg,uint8_t* buffer,uint32_t len){
    status_t ret = (HAL_I2C_Mem_Write(MPU6050_INSTANCE,MPU6050_ADDRESS,address_reg,1,buffer,len,MPU6050_TIMEOUT) == HAL_OK)?STATUS_OK:STATUS_ERROR;
    return ret;
}
PRIVATE status_t i2c_read_mem( uint8_t address_reg,uint8_t* buffer,uint32_t len){
    
    status_t ret =(HAL_I2C_Mem_Read(MPU6050_INSTANCE,MPU6050_ADDRESS,address_reg,1,buffer,len,MPU6050_TIMEOUT) == HAL_OK ) ?STATUS_OK:STATUS_ERROR;
    return ret;
}

/**
 * @brief Comprueba si el dispositivo esta ready
 * 
 * @return ** PRIVATE 
 */
PRIVATE status_t mpu6050_check(){

    status_t ret = STATUS_ERROR;
    uint8_t value = 0;
    ret = i2c_read_mem(WHO_I_AM,&value,1);
    ret = (value == MPU6050_AVAILABLE) ?STATUS_OK:ret;
    return ret;
}




/**
 * @brief Calibracion con offset en sram
 * 
 * @param x_e 
 * @param y_e 
 * @param z_e 
 * @return ** PRIVATE 
 */
PRIVATE void mpu6050_calibration(int16_t x_e, int16_t y_e, int16_t z_e){

    // x_e, y_e, z_e son los valores esperando. En nuestro caso 0,0,g    
    // para calibracion en superficie horizontal
    #define CALIBRATION_ITER     1000
    int16_t x,y,z ;
    int16_t delta_x = 0;
    int16_t delta_y = 0;
    int16_t delta_z = 0;
    for(int16_t i= 0; i< CALIBRATION_ITER ; i++){
    mpu6050_get_acceleration(&x,&y,&z);
    delta_x = x_e - x;
    delta_y = y_e - y;
    delta_z = z_e - z;
    //corrijo los offset
    if(delta_x !=0) offset_x = (delta_x >0)? (offset_x+1): (offset_x-1);
    if(delta_y !=0) offset_y = (delta_y >0)? (offset_y+1): (offset_y-1);
    if(delta_z !=0) offset_z = (delta_z >0)? (offset_z+1): (offset_z-1);
    //recargo los offset
    //__MPU6050_set_offset(x_offset, y_offset, z_offset);
    }    
}


status_t mpu6050_init(){
    I2C_init();
    status_t ret = STATUS_ERROR;
    uint8_t config = 0;
    ret = i2c_write_mem(PWR_MGMT_1,&config,1);
    ret  = mpu6050_check();
    if( ret == STATUS_ERROR) modulo_debug_print("error en check\n");
    ret = mpu6050_resume();
    if( ret == STATUS_ERROR) modulo_debug_print("error en resumen\n");
    ret = mpu6050_set_scala(  SCALA_2G);
    if( ret == STATUS_ERROR)    modulo_debug_print("error en configuracion de escala\n");
    // Calibramos
    mpu6050_calibration(0, 0, (SCALA_DIV / 2.0));   // ESPERAMOS X=0G , Y=0G y Z=1G
    ret = mpu6050_sleep();
    return ret;
}


status_t mpu6050_set_scala(mpu6050_scala_t scala)
{
    status_t ret = STATUS_ERROR;
    uint8_t config_scala = 0;
    switch (scala)
    {
    case SCALA_2G:
        BIT_SET(config_scala,3);
        SCALA_DIV = 16384.0;
        break;
    case SCALA_4G:
        BIT_SET(config_scala,3);   
        SCALA_DIV = 8192.0;
        break;
    case SCALA_8G:
        BIT_SET(config_scala,4);
        SCALA_DIV = 4096.0;
        break;
    case SCALA_16G:
        BIT_SET(config_scala,4);
        BIT_SET(config_scala,3);
        SCALA_DIV = 2048.0;
        break;
    default:
        break;
    }
    ret = i2c_write_mem( ACCEL_CONFIG,&config_scala,1) ;
    return ret;
}






 status_t mpu6050_resume(){
    status_t ret = STATUS_ERROR;
    uint8_t value = 0;
    ret = i2c_read_mem(PWR_MGMT_1,&value,1);
    if(ret == STATUS_ERROR) return ret;
    BIT_CLEAR(value,3);
    BIT_CLEAR(value,5); // DESHABILITAMOS EL MODO CICLICO
    BIT_CLEAR(value,6);
    ret = i2c_write_mem(PWR_MGMT_1,&value,1);
    
    return ret;

}


status_t mpu6050_sleep(){
    status_t ret = STATUS_ERROR;
    uint8_t value = 0;
    ret = i2c_read_mem(PWR_MGMT_1,&value,1);
    if(ret == STATUS_ERROR) return ret;
    BIT_CLEAR(value,5); // DESHABILITAMOS EL MODO CICLICO
    BIT_SET(value,6);
    ret = i2c_write_mem(PWR_MGMT_1,&value,1);
    return ret;
}


status_t mpu6050_get_acceleration( int16_t* x, int16_t* y, int16_t* z){
    status_t ret = STATUS_ERROR;
    // Leer los 6 registros juntos
    uint8_t address_mem = ACCEL_XOUT_H;  // el primer elemento
    #define MEM_SIZE  (6)
    uint8_t data[MEM_SIZE]={0};
    ret = i2c_read_mem(address_mem,data,MEM_SIZE);
    (*x) = (int16_t)(data[0] << 8 | data [1]);
    (*x) = (*x) + offset_x;
    (*y) = (int16_t)(data[2] << 8 | data [3]);
    (*y) = (*y) + offset_y;
    (*z) = (int16_t)(data[4] << 8 | data [5]);   
    (*z) = (*z) + offset_z;
    return ret;
}




status_t mpu6050_get_temperature( int16_t* temp){
    status_t ret = STATUS_ERROR;
    uint8_t address_mem = ACCEL_TOUT_H;  // el primer elemento
    #define MEM_SIZE  (2)
    uint8_t data[MEM_SIZE]={0};
    ret = i2c_read_mem(address_mem,data,MEM_SIZE);
    (*temp) = (int16_t)((int16_t)(data[0] << 8) | ((int16_t)data[1]));
    return ret;
}




status_t mpu6050_get_measure(uint8_t* buffer, uint8_t len){
    status_t ret = STATUS_OK;
    static uint8_t counter = 0;
    mpu6050_resume();
    int16_t temp =0;
    ret = mpu6050_get_temperature(  &temp);
    if( ret == STATUS_ERROR)    modulo_debug_print("error en leer temp\n");
    float ft = (float)  ((temp/340.0)+ 36.53);
    int16_t x,y,z ;
    ret = mpu6050_get_acceleration(  &x,&y,&z);
    if( ret == STATUS_ERROR)    modulo_debug_print("error en leer acelerometro\n");
    float fx = (float) (x/(SCALA_DIV/2.0)); //      
    float fy = (float) (y/(SCALA_DIV/2.0)); // 
    float fz = (float) (z/(SCALA_DIV/2.0)); // 
    sprintf(buffer,"\ncounter:%d , t:%.2f , x:%.2f , y:%.2f , z:%.2f \n",counter,ft,fx,fy,fz);
    modulo_debug_print(buffer);
    mpu6050_sleep();
    counter ++;
    return ret;
}

























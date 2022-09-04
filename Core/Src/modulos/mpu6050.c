#include "i2c.h"
#include "MPU6050.h"

extern I2C_HandleTypeDef hi2c2;


#define MPU6050_INSTANCE                          (&hi2c2)
#define MPU6050_ADDRESS                           (0xD0)
#define MPU6050_AVAILABLE                         (0x68)

#define ACCEL_XOUT_H                             (0x3B)
#define ACCEL_XOUT_L                             (0x3C)
#define ACCEL_YOUT_H                             (0x3D)
#define ACCEL_YOUT_L                             (0x3E)
#define ACCEL_ZOUT_H                             (0x3F)
#define ACCEL_ZOUT_L                             (0x40)
#define ACCEL_TOUT_H                             (0x41)
#define ACCEL_TOUT_L                             (0x42)




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




PRIVATE status_t i2c_write_mem( uint8_t address_reg,uint8_t* buffer,uint32_t len){
    status_t ret = (HAL_I2C_Mem_Write(MPU6050_INSTANCE,MPU6050_ADDRESS,address_reg,1,buffer,len,MPU6050_TIMEOUT) == HAL_OK)?STATUS_OK:ret;

    return ret;
}
PRIVATE status_t i2c_read_mem( uint8_t address_reg,uint8_t* buffer,uint32_t len){
    
    status_t ret =(HAL_I2C_Mem_Read(MPU6050_INSTANCE,MPU6050_ADDRESS,address_reg,1,buffer,len,MPU6050_TIMEOUT) == HAL_OK ) ?STATUS_OK:ret;
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




status_t mpu6050_init(){
     MX_I2C2_Init();
     status_t ret = STATUS_OK;
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
        break;
    case SCALA_4G:
        BIT_SET(config_scala,3);   
        break;
    case SCALA_8G:
        BIT_SET(config_scala,4);
        break;
    case SCALA_16G:
        BIT_SET(config_scala,4);
        BIT_SET(config_scala,3);
        break;
    default:
       
        break;
    }
    ret = i2c_write_mem( CONFIG,&config_scala,1) ;
    return ret;
}



 status_t mpu6050_resume(){

    status_t ret = STATUS_ERROR;
    uint8_t value = 0;
    ret = i2c_read_mem(PWR_MGMT_1,&value,1);
    if(ret == STATUS_ERROR) return ret;
    BIT_CLEAR(value,5);
    BIT_CLEAR(value,6);
    ret = i2c_write_mem(PWR_MGMT_1,&value,1);
    
    return ret;

}


status_t mpu6050_sleep(){
    status_t ret = STATUS_ERROR;
    uint8_t value = 0;
    ret = i2c_read_mem(PWR_MGMT_1,&value,1);
    if(ret == STATUS_ERROR) return ret;
    BIT_CLEAR(value,5);
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
    i2c_write_mem(address_mem,data,MEM_SIZE);
    // HAL_I2C_Mem_Write(MPU6050_INSTANCE,MPU6050_ADDRESS,address_mem,1,data,MEM_SIZE,MPU6050_TIMEOUT);


    (*x) = (int16_t)(data[0] << 8 | data [1]);
    (*y) = (int16_t)(data[2] << 8 | data [3]);
    (*z) = (int16_t)(data[4] << 8 | data [5]);


    return ret;
}
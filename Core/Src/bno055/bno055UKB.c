/*
 * bno055UKB.c
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#include "main.h"
#include "stm32f4xx_hal.h"
#include "bno055UKB.h"
#include "bno055.h"
#include "kalmanFilter.h"
#include "math.h"

struct bno055_t bno055;
struct bno055_euler_t euler_data;
struct bno055_quaternion_t quat_data;
struct bno055_gyro_t gyro_data;
struct bno055_accel_t acc_data;
struct bno055_mag_t magn_data;

volatile float yaw, pitch, roll;
volatile float yawRaw, pitchRaw, rollRaw;
volatile float accX, accY, accZ;
volatile float accXraw, accYraw, accZraw;
volatile float gyroX, gyroY, gyroZ;
volatile float gyroXraw, gyroYraw, gyroZraw;
float magnX, magnY, magnZ;
float qw, qx, qy, qz;

int8_t i2c_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    return HAL_I2C_Mem_Write(&hi2c2, dev_addr << 1, reg_addr,
                             I2C_MEMADD_SIZE_8BIT, data, len, 100) == HAL_OK ? 0 : -1;
}

int8_t i2c_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    return HAL_I2C_Mem_Read(&hi2c2, dev_addr << 1, reg_addr,
                            I2C_MEMADD_SIZE_8BIT, data, len, 100) == HAL_OK ? 0 : -1;
}

void delay_msec(u32 msec)
{
    HAL_Delay(msec);
}

void bno055_initialize(void)
{
    bno055.bus_write = i2c_bus_write;
    bno055.bus_read = i2c_bus_read;
    bno055.delay_msec = delay_msec;
    bno055.dev_addr = BNO055_I2C_ADDR1;
    bno055_init(&bno055);

    bno055_set_power_mode(BNO055_POWER_MODE_NORMAL);
    bno055.delay_msec(10);
    bno055_set_operation_mode(BNO055_OPERATION_MODE_CONFIG);
    bno055.delay_msec(10);
    bno055_set_axis_remap_value(0x09);
    bno055.delay_msec(10);
    bno055_set_remap_x_sign(BNO055_REMAP_AXIS_POSITIVE);
    bno055.delay_msec(10);
    bno055_set_remap_y_sign(BNO055_REMAP_AXIS_POSITIVE);
    bno055.delay_msec(10);
    bno055_set_remap_z_sign(BNO055_REMAP_AXIS_NEGATIVE);
    bno055.delay_msec(10);
    bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);
    bno055.delay_msec(20);
}

void bno055ReadAngles(void)
{
    bno055_read_gyro_xyz(&gyro_data);
    gyroXraw = (float)gyro_data.x/16.0f*0.017453f;
    gyroYraw = (float)gyro_data.y/16.0f*0.017453f;
    gyroZraw = (float)gyro_data.z/16.0f*0.017453f;

    bno055_read_accel_xyz(&acc_data);
    accXraw = (float)acc_data.x/100.0f/9.81f;
    accYraw = (float)acc_data.y/100.0f/9.81f;
    accZraw = (float)acc_data.z/100.0f/9.81f;

    bno055_read_quaternion_wxyz(&quat_data);
    qw = (float)quat_data.w/pow(2, 14);
    qx = (float)quat_data.x/pow(2, 14);
    qy = (float)quat_data.y/pow(2, 14);
    qz = (float)quat_data.z/pow(2, 14);

    pitchRaw = atan2f(2.0f * (qw*qx + qy*qz), 1.0f - 2.0f * (qx*qx + qy*qy));
    rollRaw = asinf(2.0f * (qw*qy - qz*qx));
    yawRaw = atan2f(2.0f * (qw*qz + qx*qy), 1.0f - 2.0f * (qy*qy + qz*qz));

    rollRaw *= (180.0f / M_PI);
    pitchRaw *= (180.0f / M_PI);
    yawRaw *= (180.0f / M_PI);

    if (pitchRaw > 0) {
    	pitchRaw = pitchRaw - 180;
    } else if (pitchRaw < 0) {
    	pitchRaw = pitchRaw + 180;
    }

    pitch = Kalman_Update(&pitchFilter, pitchRaw);
    roll = Kalman_Update(&rollFilter, rollRaw);
    yaw = Kalman_Update(&yawFilter, yawRaw);
    accX = Kalman_Update(&accXFilter, accXraw);
    accY = Kalman_Update(&accYFilter, accYraw);
    accZ = Kalman_Update(&accZFilter, accZraw);

//    sensorData.Ax = accX;
//    sensorData.Ay = accY;
//    sensorData.Az = accZ;
//    sensorData.Gx = gyroX;
//    sensorData.Gy = gyroY;
//    sensorData.Gz = gyroZ;
//    sensorData.angleX = roll;
//    sensorData.angleY = pitch;
//    sensorData.angleZ = yaw;
}

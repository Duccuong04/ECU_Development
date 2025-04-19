#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f10x.h"

#define MPU6050_ADDR 0x68

#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define SMPLRT_DIV_REG 0x19
#define ACCEL_CONFIG_REG 0x1C
#define TEMP_OUT_H_REG 0x41
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_XOUT_H_REG 0x3B
#define GYRO_XOUT_H_REG 0x43
#define CONFIG_REG          0x1A

typedef struct {
    float Ax, Ay, Az;
    float Gx, Gy, Gz;
} MPU6050_Data;

void MPU6050_Init(void);
void MPU6050_ReadData(MPU6050_Data *data);

#endif


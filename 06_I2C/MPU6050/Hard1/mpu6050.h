#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f10x.h"

typedef struct {
    float Ax, Ay, Az;
    float Gx, Gy, Gz;
} MPU6050_Data;

void MPU6050_Init(void);
void MPU6050_ReadData(MPU6050_Data *data);

#endif

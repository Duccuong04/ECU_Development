#include "mpu6050.h"

#define MPU6050_ADDR           0x68
#define MPU6050_PWR_MGMT_1     0x6B
#define MPU6050_ACCEL_XOUT_H   0x3B
#define MPU6050_GYRO_XOUT_H    0x43

// --------- I2C Low-Level APIs ----------
void I2C_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, devAddr << 1, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, regAddr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(I2C1, data);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C1, ENABLE);
}

uint8_t I2C_ReadByte(uint8_t devAddr, uint8_t regAddr) {
    uint8_t data;

    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, devAddr << 1, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, regAddr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTART(I2C1, ENABLE); // repeated start
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, devAddr << 1, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    data = I2C_ReceiveData(I2C1);

    I2C_GenerateSTOP(I2C1, ENABLE);
    return data;
}

int16_t I2C_ReadWord(uint8_t devAddr, uint8_t regAddr) {
    uint8_t high = I2C_ReadByte(devAddr, regAddr);
    uint8_t low  = I2C_ReadByte(devAddr, regAddr + 1);
    return (int16_t)((high << 8) | low);
}

// --------- MPU6050 Functions ----------
void MPU6050_Init(void) {
    I2C_WriteByte(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x00); // Wake up
}

void MPU6050_ReadData(MPU6050_Data *data) {
    data->Ax = (float)I2C_ReadWord(MPU6050_ADDR, MPU6050_ACCEL_XOUT_H) / 16384.0f;
    data->Ay = (float)I2C_ReadWord(MPU6050_ADDR, MPU6050_ACCEL_XOUT_H + 2) / 16384.0f;
    data->Az = (float)I2C_ReadWord(MPU6050_ADDR, MPU6050_ACCEL_XOUT_H + 4) / 16384.0f;

    data->Gx = (float)I2C_ReadWord(MPU6050_ADDR, MPU6050_GYRO_XOUT_H) / 131.0f;
    data->Gy = (float)I2C_ReadWord(MPU6050_ADDR, MPU6050_GYRO_XOUT_H + 2) / 131.0f;
    data->Gz = (float)I2C_ReadWord(MPU6050_ADDR, MPU6050_GYRO_XOUT_H + 4) / 131.0f;
}

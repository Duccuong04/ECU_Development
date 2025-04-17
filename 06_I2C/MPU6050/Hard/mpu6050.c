#include "mpu6050.h"

static int16_t MPU6050_ReadWord(uint8_t reg) {
    uint8_t high = 0, low = 0;

    // Start and Address of register need to read - Write
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, MPU6050_ADDR << 1, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, reg);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Repeated Start + Read
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, MPU6050_ADDR << 1, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    high = I2C_ReceiveData(I2C1);

    I2C_AcknowledgeConfig(I2C1, DISABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    low = I2C_ReceiveData(I2C1);
    I2C_GenerateSTOP(I2C1, ENABLE);
    I2C_AcknowledgeConfig(I2C1, ENABLE);

    return (int16_t)((high << 8) | low);
}

void I2C_WriteRegister(uint8_t reg_addr, uint8_t data) {
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, MPU6050_ADDR << 1, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, reg_addr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(I2C1, data);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C1, ENABLE);
}

void MPU6050_Init(void) {
    I2C_WriteRegister(PWR_MGMT_1_REG, 0x00);      // Wake up MPU6050
    I2C_WriteRegister(SMPLRT_DIV_REG, 0x07);      // Sample rate = 1kHz / (1 + 7) = 125Hz
    I2C_WriteRegister(CONFIG_REG, 0x01);          // Low pass filter ~188Hz
    I2C_WriteRegister(GYRO_CONFIG_REG, 0x00);     // ±250 °/s
    I2C_WriteRegister(ACCEL_CONFIG_REG, 0x00);    // ±2g
}

void MPU6050_ReadData(MPU6050_Data *data) {
    // Ð?c gia t?c
    data->Ax = (float)MPU6050_ReadWord(ACCEL_XOUT_H_REG)     / 16384.0f;
    data->Ay = (float)MPU6050_ReadWord(ACCEL_XOUT_H_REG + 2) / 16384.0f;
    data->Az = (float)MPU6050_ReadWord(ACCEL_XOUT_H_REG + 4) / 16384.0f;

    // Ð?c gyro
    data->Gx = (float)MPU6050_ReadWord(GYRO_XOUT_H_REG)     / 131.0f;
    data->Gy = (float)MPU6050_ReadWord(GYRO_XOUT_H_REG + 2) / 131.0f;
    data->Gz = (float)MPU6050_ReadWord(GYRO_XOUT_H_REG + 4) / 131.0f;
}

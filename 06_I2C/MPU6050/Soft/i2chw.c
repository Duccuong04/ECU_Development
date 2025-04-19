#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

// I2C Pin d?nh nghia
#define I2C_SCL     GPIO_Pin_6
#define I2C_SDA     GPIO_Pin_7
#define I2C_GPIO    GPIOB

// I2C macro di?u khi?n
#define WRITE_SCL_1 GPIO_SetBits(I2C_GPIO, I2C_SCL)
#define WRITE_SCL_0 GPIO_ResetBits(I2C_GPIO, I2C_SCL)
#define WRITE_SDA_1 GPIO_SetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SDA_0 GPIO_ResetBits(I2C_GPIO, I2C_SDA)
#define READ_SDA_VAL GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA)

typedef enum { NOT_OK, OK } status;
typedef enum { NACK, ACK } ACK_Bit;

// C?u hình clock
void RCC_Config() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

// C?u hình timer delay us
void TIM_Config() {
    TIM_TimeBaseInitTypeDef timer;
    timer.TIM_ClockDivision = TIM_CKD_DIV1;
    timer.TIM_CounterMode = TIM_CounterMode_Up;
    timer.TIM_Period = 0xFFFF;
    timer.TIM_Prescaler = 72 - 1; // 1us n?u 72MHz
    TIM_TimeBaseInit(TIM2, &timer);
    TIM_Cmd(TIM2, ENABLE);
}

void delay_us(uint32_t time) {
    TIM_SetCounter(TIM2, 0);
    while (TIM_GetCounter(TIM2) < time);
}

// C?u hình GPIO ? ch? d? open-drain
void GPIO_Config() {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = I2C_SCL | I2C_SDA;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}

// Set SDA/SCL ban d?u
void I2C_Config() {
    WRITE_SCL_1;
    WRITE_SDA_1;
    delay_us(5);
}

// T?o tín hi?u START
void I2C_Start() {
    WRITE_SDA_1;
    WRITE_SCL_1;
    delay_us(5);
    WRITE_SDA_0;
    delay_us(5);
    WRITE_SCL_0;
}

// T?o tín hi?u STOP
void I2C_Stop() {
    WRITE_SDA_0;
    delay_us(5);
    WRITE_SCL_1;
    delay_us(5);
    WRITE_SDA_1;
    delay_us(5);
}

// G?i 1 byte (g?m c? d?a ch? thi?t b? và d? li?u)
status I2C_Master_Write(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        if (data & 0x80) WRITE_SDA_1;
        else WRITE_SDA_0;

        data <<= 1;
        delay_us(2);
        WRITE_SCL_1;
        delay_us(5);
        WRITE_SCL_0;
        delay_us(2);
    }

    // Ð?c ACK
    WRITE_SDA_1;  // Nh? SDA
    delay_us(2);
    WRITE_SCL_1;
    delay_us(2);

    status ret = (READ_SDA_VAL == 0) ? OK : NOT_OK;

    WRITE_SCL_0;
    delay_us(2);
    return ret;
}

// Ð?c 1 byte t? Slave
uint8_t I2C_Master_Read(ACK_Bit ack) {
    uint8_t data = 0;

    WRITE_SDA_1; // Nh? SDA d? d?c
    for (int i = 0; i < 8; i++) {
        data <<= 1;

        WRITE_SCL_1;
        delay_us(3);
        if (READ_SDA_VAL) data |= 1;
        WRITE_SCL_0;
        delay_us(3);
    }

    // G?i ACK ho?c NACK
    if (ack == ACK) WRITE_SDA_0;
    else WRITE_SDA_1;

    delay_us(2);
    WRITE_SCL_1;
    delay_us(3);
    WRITE_SCL_0;
    delay_us(2);
    WRITE_SDA_1; // Nh? SDA

    return data;
}

// Ð?c WHO_AM_I (0x75) c?a MPU6050
uint8_t MPU6050_ReadWhoAmI() {
    uint8_t data = 0xFF;

    I2C_Start();
    if (I2C_Master_Write(0xD0) != OK) return 0xEE; // 0x68 << 1 | 0
    I2C_Master_Write(0x75); // WHO_AM_I register
    I2C_Stop();

    delay_us(10);

    I2C_Start();
    if (I2C_Master_Write(0xD1) != OK) return 0xEF; // 0x68 << 1 | 1
    data = I2C_Master_Read(NACK);
    I2C_Stop();

    return data;
}

uint8_t mpu_id;
int main() {


    RCC_Config();
    TIM_Config();
    GPIO_Config();
    I2C_Config();

    while (1) {
        mpu_id = MPU6050_ReadWhoAmI();
        delay_us(1000000); // 1s
    }
}

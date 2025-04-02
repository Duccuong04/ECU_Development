// Slave
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

// define SPI pin through GPIO Pin
#define SPI_SCK_Pin GPIO_Pin_0
#define SPI_MISO_Pin GPIO_Pin_1
#define SPI_MOSI_Pin GPIO_Pin_2
#define SPI_CS_Pin GPIO_Pin_3
#define SPI_GPIO GPIOA
#define SPI_RCC RCC_APB2Periph_GPIOA

// cap clock GPIOA & TIM2
void RCC_Config(){
	RCC_APB2PeriphClockCmd(SPI_RCC, ENABLE);              
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

// tao ham delay = timer
void TIM_Config(){
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_Prescaler = 7200 - 1;
	TIM_InitStruct.TIM_Period = 0xFFFF;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	TIM_Cmd(TIM2, ENABLE);
}

void delay_ms(uint32_t time){
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < time*10){}
}

// cau hinh ngo vao ngo ra Slave
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_CS_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
}

uint8_t SPI_Slave_Receive(void) {
    uint8_t dataReceive = 0x00;
    
    // Ch? d?n khi CS xu?ng m?c th?p (Master b?t d?u g?i)
    while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));

    // Nh?n d? li?u 8 bit
    for (int i = 0; i < 8; i++) {
        // Ch? SCK lên m?c cao
        while (!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
        
        // Ð?c bit t? MOSI
        dataReceive = (dataReceive << 1) | GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin);
        
        // Ch? SCK xu?ng m?c th?p tru?c khi d?c bit ti?p theo
        while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
    }
    
    // Ch? CS lên m?c cao (Master k?t thúc truy?n)
    while (!GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));

    return dataReceive;
}

uint8_t DataReceive;
int main()
{
	RCC_Config();
	TIM_Config();
	GPIO_Config();
	
	while(1)
	{
		DataReceive = SPI_Slave_Receive();
	}
}

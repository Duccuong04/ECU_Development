// Master
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI



// define SPI pin of STM32F103
#define SPI1_NSS 	GPIO_Pin_4
#define SPI1_SCK	GPIO_Pin_5
#define SPI1_MISO 	GPIO_Pin_6
#define SPI1_MOSI 	GPIO_Pin_7
#define SPI1_GPIO 	GPIOA
#define SPI1_RCC  RCC_APB2Periph_SPI1

// provide clock, khoi SPI se tu dong noi voi GPIO
void RCC_Config()
{
	RCC_APB2PeriphClockCmd(SPI1_RCC | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void TIM_Config()
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;

	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // 72MHzz
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = 0xFFFF;
	TIM_InitStruct.TIM_Prescaler = 7200 - 1;
	
	TIM_TimeBaseInit (TIM2, &TIM_InitStruct);
	TIM_Cmd(TIM2, ENABLE);
}

void delay(uint32_t time)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < time*10)
	{}		
}

	
// configurating GPIO pin as SPI function
void GPIO_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SPI1_NSS | SPI1_SCK | SPI1_MISO | SPI1_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // afternation push-pull, connect to SPI block
	
	GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);
}


// SPI configuration
void SPI_Config()
{
	SPI_InitTypeDef SPI_InitStructure;
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7; // check sum CRC bac 7
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// kiem soat chan NSS/CS bang software
	
	SPI_Init(SPI1, &SPI_InitStructure); // save to register
	SPI_Cmd(SPI1, ENABLE);	
}

void SPI_Send1Byte(uint8_t data)
{
		GPIO_ResetBits(GPIOA, SPI1_NSS);
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
    SPI_I2S_SendData(SPI1, (uint16_t)data);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET){}
		GPIO_SetBits(GPIOA, SPI1_NSS);
}

uint8_t Data_Transmitter[] = {1, 3, 5, 9, 10, 18};
int main()
{
	RCC_Config();
	TIM_Config();
	SPI_Config();
	GPIO_config();
	
	while(1)
	{
		for(int i = 0; i < sizeof(Data_Transmitter)/sizeof(uint8_t); i++)
		{
			SPI_Send1Byte(Data_Transmitter[i]);
			delay(1000);
		}
	}
}
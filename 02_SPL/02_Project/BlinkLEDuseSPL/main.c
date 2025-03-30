#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

void delay(uint32_t time)
{
	for(uint32_t i = 0; i < time; i++)
	{
	}
}

void RCC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}

void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;           // PC13
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;     // Mode output push pull
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;    // Speed	
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);               // save setting at register
}

int main()
{
	RCC_Config();
	GPIO_Config();
	while(1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		delay(10000000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		delay(10000000);
	}
		
}


#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

void RCC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
}

void GPIO_Config()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // C?u hình GPIOC Pin 13 (Output Push-Pull)
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // C?u hình GPIOA Pin 0 (Input Pull-Up)
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

int main()
{
	RCC_Config();
	GPIO_Config();
	while(1)
{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
		if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))
			{
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		}
		else 
			{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
			}		
		}
}
}







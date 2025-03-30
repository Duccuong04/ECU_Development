/*
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

void RCC_Config()
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}	

void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void TIM_Config()
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_Prescaler = 7200 - 1;  // Ttimer = 0.1ms -> counter ++ 
	TIM_InitStruct.TIM_Period = 0xFFFF;       // Period (ARR), max
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // Tck_psc = 72MHZ
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_Cmd(TIM2, ENABLE); // allow TIMER, counter
}

void delay(uint32_t time)
{
	TIM_SetCounter(TIM2, 0);  // set counter = 0 - Mode Counter up
	while(TIM_GetCounter(TIM2) < time*10); // delay 1s, Counter ll count 0 -> 10000
}

int main()
{
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	
	while(1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		delay(1000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		delay(1000);		
	}
}
*/

// use timer interrupt
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

void TIM2_IRQHandler()									// process IRQ function
{
	while(TIM_GetITStatus(TIM2, TIM_IT_Update))  // check flag interrupt TIM2
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
		GPIOC->ODR ^= GPIO_Pin_13;
	}
}

void RCC_Config()
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}	

void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void TIM_Config()
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_Prescaler = 7200 - 1;  // Ttimer = 0.1ms -> counter ++ 
	TIM_InitStruct.TIM_Period = 10000 - 1;       // Period (ARR) = 10000, 1s trafn 1 time
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // Tck_psc = 72MHZ
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  // kich hoat ngat update event
	TIM_Cmd(TIM2, ENABLE); // allow TIMER, counter
  
	// Setting NVIC, 
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void delay(uint32_t time)
{
	TIM_SetCounter(TIM2, 0);  // set counter = 0 - Mode Counter up
	while(TIM_GetCounter(TIM2) < time*10); // delay 1s, Counter ll count 0 -> 10000
}

int main()
{
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	
	while(1)
	{
	}
}



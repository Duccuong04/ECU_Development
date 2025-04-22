#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI

uint16_t count = 0;
uint8_t data;

// cap clock Button A0, LED PC13, TIM2, AFIO cho EXTI
void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_USART1, ENABLE); // cap clock cho button A0 & LED C13
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

// Config PA0, PC13, RX, TX
void GPIO_Config(){
	GPIO_InitTypeDef GPIOInitStruct;
	
	//EXTI button PA0
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOInitStruct);
	
	// LED PC13
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIOInitStruct);
	
	//Tx
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOInitStruct);
	
	//Rx
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOInitStruct);
}

// Config TIM2 Interrupt
void TIM_Config()
{
	TIM_TimeBaseInitTypeDef TIMInitStruct;
	
	TIMInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIMInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIMInitStruct.TIM_Period = 10000 - 1;                 // 1s tran TIMER -> Interrupt
	TIMInitStruct.TIM_Prescaler = 7200 - 1;            // 72Mhz/7200
	
	TIM_TimeBaseInit(TIM2, &TIMInitStruct);
	// Khoi dong TIM2 Interrupt
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

// Config USART
void UART_Config(){
	USART_InitTypeDef UART_InitStructure;
	
	UART_InitStructure.USART_BaudRate = 9600;
	UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	UART_InitStructure.USART_Parity = USART_Parity_No;
	UART_InitStructure.USART_StopBits = USART_StopBits_1;
	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1, &UART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

// Config EXTI
void EXTI_Config(){
	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
}

// Config NVIC cho EXTI, Timer2 Interrupt, USART Interrupt
void NVIC_Config(){
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	// NVIC for EXTI
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	// NVIC for TIM2 Interrupt
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	// NVIC for USART Interrupt
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}
	
void EXTI0_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) // chac chan la line 0 bao ngat
	{
		count++;
		EXTI_ClearITPendingBit(EXTI_Line0);      // reset co ngat = 0 de tranh bao lien tuc
	}
}

void TIM2_IRQHandler(){
	if(TIM_GetITStatus(TIM2, TIM_IT_Update))  // chac chan la TIM2 bao ngat
	{
		GPIOC->ODR ^= (1 << 13);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

void USART1_IRQHandler()
{
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE));
		data = USART_ReceiveData(USART1);
	/*	if(USART_GetITStatus(USART1, USART_IT_TXE) == RESET){
			USART_SendData(USART1, data);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	*/
		}
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}

int main(void){
	RCC_Config();
	GPIO_Config();
	EXTI_Config();
	NVIC_Config();
	TIM_Config();
	UART_Config();
	while(1){
	}
	
}






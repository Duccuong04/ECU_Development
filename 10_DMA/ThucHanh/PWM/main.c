#include "stm32f10x.h"


volatile uint32_t timer_ms = 0;

// TIM2 (PWM), và TIM3 (delay), GPIOA: output servo
void RCC_Config(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}

// Pin A0 output servo
void GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// C?u hình TIM2 t?o PWM 20ms, pulse ban d?u 0.5ms 
void TIM2_Config(void) {
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_OCInitTypeDef TIM_OC;

	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Prescaler = 72 - 1; // 72MHz / 72 = 1MHz (1us/tick)
	TIM_InitStructure.TIM_Period = 20000 - 1; // 20ms period
	TIM_TimeBaseInit(TIM2, &TIM_InitStructure);

	TIM_OC.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC.TIM_Pulse = 1500; // 0.5ms = 0° (servo g?c)
	TIM_OC1Init(TIM2, &TIM_OC);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_Cmd(TIM2, ENABLE);
}

// C?u hình TIM3 t?o ng?t m?i 1ms d? dùng cho delay_ms()
void TIM3_Int_Init(void) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;        // Ð?m 1000 tick ? 1ms
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;       // 72MHz / 72 = 1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	// C?u hình ng?t TIM3
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3_IRQHandler(void) {
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		if (timer_ms > 0) {
			timer_ms--;
		}
	}
}

void delay_ms(uint32_t ms) {
	timer_ms = ms;
	while (timer_ms != 0); // Ch? d?n khi timer_ms = 0
}

int main(void) {
	RCC_Config();
	GPIO_Config();
	TIM2_Config();
	TIM3_Int_Init();

	while (1) {

	}
}

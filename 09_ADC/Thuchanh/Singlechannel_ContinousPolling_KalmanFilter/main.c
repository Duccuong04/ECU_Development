#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "kalman.h"

float val = 0;
float final = 0;
// Config clock
void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

// Config GPIO
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

// Config ADC
void ADC_Config(){
	ADC_InitTypeDef ADC_InitStruct;
	
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	
	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE); // end of conversion mask int
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVICInitStruct;
	NVICInitStruct.NVIC_IRQChannel = ADC1_2_IRQn;
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVICInitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVICInitStruct);
	
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*void ADC1_2_IRQHandler(){
	if(ADC_GetITStatus(ADC1, ADC_IT_EOC)){
		val = ADC_GetConversionValue(ADC1); // 1 ADC chi co 1 thanh ghi DR
		final = updateEstimate(val);
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
	}
}
*/

int main(void){
	RCC_Config();
	ADC_Config();
	GPIO_Config();

	SimpleKalmanFilter(0.1, 0.05, 0.1);
	while(1){
	val = ADC_GetConversionValue(ADC1);
		final = updateEstimate(val);
	}
}


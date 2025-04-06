// Slave
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

// define SPI pin through GPIO Pin
#define SPI_SCK GPIO_Pin_4
#define SPI_MISO GPIO_Pin_5
#define SPI_MOSI GPIO_Pin_6
#define SPI_CS  GPIO_Pin_7
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
	GPIO_InitStructure.GPIO_Pin = SPI_SCK | SPI_MOSI | SPI_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
}

uint8_t SPI_Slave_TransmitReceive(uint8_t u8Data) {
	  uint8_t u8Mask = 0x80;	// 0b10000000
    uint8_t DataReceiveSlave = 0x00;
	  uint8_t tempData;
		
    while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS));
			for (int i = 0; i < 8; i++) 
			{
				tempData = u8Data & u8Mask;
				if(tempData)
				{
					GPIO_WriteBit(SPI_GPIO, SPI_MISO, Bit_SET);
					delay_ms(1);
				} 
				else
				{
					GPIO_WriteBit(SPI_GPIO, SPI_MISO, Bit_RESET);
					delay_ms(1);
				}
			u8Data = u8Data << 1;
      while (!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK)){}
      DataReceiveSlave = (DataReceiveSlave << 1) | GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI);
      while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK));
    }
    while (!GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS));
    return DataReceiveSlave;
}

uint8_t DataReceive_Slave;
uint8_t DataTransmit_Slave[] = {1, 3, 5, 7, 10, 100};
uint8_t indexx = 0;

int main(void)
{
	RCC_Config();
	TIM_Config();
	GPIO_Config();
	
	while(1)
	{
        DataReceive_Slave = SPI_Slave_TransmitReceive(DataTransmit_Slave[indexx]);
        indexx++;
        if (indexx >= 6) indexx = 0;
        delay_ms(10);  
  }
}
	

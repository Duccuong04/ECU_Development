#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_dma.h"              // Keil::Device:StdPeriph Drivers:DMA

#define SPI1_NSS   GPIO_Pin_4
#define SPI1_SCK   GPIO_Pin_5
#define SPI1_MISO  GPIO_Pin_6
#define SPI1_MOSI  GPIO_Pin_7
#define SPI1_GPIO  GPIOA

uint8_t rxbuffer[100];

void RCC_Config(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

void GPIO_Config(){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = SPI1_NSS | SPI1_SCK | SPI1_MISO| SPI1_MOSI;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    
    GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);
}

void DMA_Config(){
    DMA_InitTypeDef DMA_InitStruct;
    
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStruct.DMA_BufferSize = 20;   // S? lu?ng byte mu?n truy?n
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)rxbuffer;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;	//Data Register
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    
    DMA_Init(DMA1_Channel2, &DMA_InitStruct);
    DMA_Cmd(DMA1_Channel2, ENABLE);    
}

void SPI_Config(){
    SPI_InitTypeDef SPI_InitStruct;
    
    SPI_InitStruct.SPI_Mode = SPI_Mode_Slave;
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Hard;
    
    SPI_Init(SPI1, &SPI_InitStruct);
	  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
    SPI_Cmd(SPI1, ENABLE);
}

int main(){
    RCC_Config();
    GPIO_Config();
	  DMA_Config();
	  SPI_Config();
    
    
    
    while(1){
        // Chuong tr?nh ch?nh
    }
}
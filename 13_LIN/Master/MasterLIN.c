#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

// Khai báo cac ham
void LIN_UART_Init(void);
void LIN_MasterSend(uint8_t id, uint8_t *data, uint8_t length);
void LIN_SendBreak(void);
void LIN_SendSync(void);
uint8_t LIN_CalculateParity(uint8_t id);
void LIN_SendID(uint8_t id);
void LIN_SendData(uint8_t *data, uint8_t length);
uint8_t LIN_CalculateChecksum(uint8_t *data, uint8_t length);
void LIN_SendChecksum(uint8_t *data, uint8_t length);

int main(void)
{
    // Data Send from Master
    uint8_t data_to_send[4] = {0x12, 0x34, 0x56, 0x78};
    
    // UART for LIN
    LIN_UART_Init();

    while (1)
    {
        // Master send 0x10, request respone from Slave
        LIN_MasterSend(0x10, data_to_send, 4);
        
        // Dely avoid overload LIN bus
        for (int i = 0; i < 1000000; i++);
    }
}

void LIN_UART_Init(void)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // clock cho GPIOA và UART1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    // chân TX (PA9) và RX (PA10)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // C?u hình UART1 cho LIN
	USART_InitStructure.USART_BaudRate = 19200;		//Standard Baudrate in LIN
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);

    // B?t UART1
    USART_Cmd(USART1, ENABLE);
		USART_LINCmd(USART1, ENABLE);
}

void LIN_MasterSend(uint8_t id, uint8_t *data, uint8_t length)
{
    // G?i Break, Sync, ID và d? li?u
    LIN_SendBreak();
    LIN_SendSync();
    LIN_SendID(id);
    LIN_SendData(data, length);
    LIN_SendChecksum(data, length);
}

void LIN_SendBreak(void)
{
    USART_SendBreak(USART1);
}

// T?o Sync
void LIN_SendSync(void)
{
    USART_SendData(USART1, 0x55);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

// T?o ID và tính toán parity
uint8_t LIN_CalculateParity(uint8_t id)
{
    uint8_t p0 = ((id >> 0) & 0x01) ^ ((id >> 1) & 0x01) ^ ((id >> 2) & 0x01);
    uint8_t p1 = ~(((id >> 1) & 0x01) ^ ((id >> 3) & 0x01) ^ ((id >> 4) & 0x01) ^ ((id >> 5) & 0x01));
    return (p0 | (p1 << 1)) << 6;
}

void LIN_SendID(uint8_t id)
{
    uint8_t id_with_parity = id | LIN_CalculateParity(id);
    USART_SendData(USART1, id_with_parity);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

// G?i d? li?u
void LIN_SendData(uint8_t *data, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        USART_SendData(USART1, data[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
}

uint8_t LIN_CalculateChecksum(uint8_t *data, uint8_t length)
{
    uint16_t checksum = 0;
    for (uint8_t i = 0; i < length; i++)
    {
        checksum += data[i];
        if (checksum > 0xFF)
        {
            checksum -= 0xFF;
        }
    }
    return ~checksum; // Bù 1 c?a checksum
}

void LIN_SendChecksum(uint8_t *data, uint8_t length)
{
    uint8_t checksum = LIN_CalculateChecksum(data, length);
    USART_SendData(USART1, checksum);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}
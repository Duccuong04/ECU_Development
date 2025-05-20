#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART


void LIN_UART_Init(void);
void LIN_SlaveReceive(uint8_t id, uint8_t *buffer, uint8_t length);

int main(void)
{
    // Data buffer Receive
    uint8_t data_received[4];
    
    // UART cho LIN
    LIN_UART_Init();

    while (1)
    {
        // Slave receive data from master (ID 0x10)
        LIN_SlaveReceive(0x10, data_received, 4);
        
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
    USART_InitStructure.USART_BaudRate = 19200;
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

uint8_t LIN_CalculateParity(uint8_t id)
{
    uint8_t p0 = ((id >> 0) & 0x01) ^ ((id >> 1) & 0x01) ^ ((id >> 2) & 0x01);
    uint8_t p1 = ~(((id >> 1) & 0x01) ^ ((id >> 3) & 0x01) ^ ((id >> 4) & 0x01) ^ ((id >> 5) & 0x01));
    return (p0 | (p1 << 1)) << 6;
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

uint8_t LIN_ReceiveData(uint8_t *buffer, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
        buffer[i] = USART_ReceiveData(USART1);
    }
    return LIN_CalculateChecksum(buffer, length);
}

void LIN_SlaveReceive(uint8_t id, uint8_t *buffer, uint8_t length)
{
    uint8_t received_id;
    
    // Ch? nh?n ID t? master
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    received_id = USART_ReceiveData(USART1);
    
    // Ki?m tra ID, n?u kh?p v?i ID yêu c?u thì ti?p t?c nh?n d? li?u
    if (received_id == (id | LIN_CalculateParity(id)))
    {
        // Nh?n d? li?u t? master
        LIN_ReceiveData(buffer, length);
        
        // Ki?m tra checksum
        uint8_t received_checksum = USART_ReceiveData(USART1);
        if (received_checksum == LIN_CalculateChecksum(buffer, length))
        {
            // D? li?u h?p l?, x? lý d? li?u nh?n du?c trong buffer
        }
        else
        {
            // X? lý l?i checksum
        }
    }
}
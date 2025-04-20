#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM


// define UART pin
#define TX_Pin		GPIO_Pin_9
#define RX_Pin    GPIO_Pin_10
#define UART_GPIO GPIOA
#define bit_time  104

// Clock
void RCC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void TIM_Config()
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = 0xFFFF;
	TIM_InitStructure.TIM_Prescaler = 72 - 1;
	
	TIM_TimeBaseInit(TIM2, &TIM_InitStructure);
	TIM_Cmd(TIM2, ENABLE);
}

void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// TX
	GPIO_InitStructure.GPIO_Pin = TX_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART_GPIO, &GPIO_InitStructure);
	
	// RX
	GPIO_InitStructure.GPIO_Pin = RX_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART_GPIO, &GPIO_InitStructure);
}

void delay_us(uint32_t time)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < time) {}
}

void delay_s(uint32_t time)
{
	for(int i = 0; i < time*1000; i++)
	{
		delay_us(1000);
	}
}

// Init UART
void UART_Init()
{
	// Set TX = 1 (idle status)
	GPIO_SetBits(UART_GPIO, TX_Pin);
	delay_us(1);	
}

// creta UART clock
void clock()
{
	delay_us(bit_time);
}

// Transmit function
void UARTSoftware_Transmit(char c)
{
	// Start bit
	GPIO_ResetBits(UART_GPIO, TX_Pin);
	clock();
	
	// Transmit bits (LSB first)
	for(int i = 0; i < 8; i++)
	{
		if(c & (1 << i))
		{
			GPIO_SetBits (UART_GPIO, TX_Pin);
		}
		else
		{
			GPIO_ResetBits(UART_GPIO, TX_Pin); 
		}
		clock();
	}
	
	// Stop bit
	GPIO_SetBits(GPIOA, TX_Pin);
	clock();
}

// Receive function
char UARTSoftware_Receive()
{
	char c = 0;
	
	// Wait start bit
	while(GPIO_ReadInputDataBit(UART_GPIO, RX_Pin) == 1);
	
	// delay 1,5 period to read at the middle of (1,5 bit time)
	delay_us(bit_time + bit_time/2);

	// read data (MSB first)
	for(int i = 0; i < 8; i++)
	{
		if(GPIO_ReadInputDataBit(GPIOA, RX_Pin))
		{
			c = c | (1 << i);
		}
		delay_us(bit_time); // wait for next bit
	}
	
	// wait stop bit
	delay_us(bit_time/2);
	
	return c;
}

void UARTSoftware_TransmitString(const char* string)
{
	while(*string != '\0')
	{
		UARTSoftware_Transmit(*string);
		string ++;
	}
}

void UARTSoftware_ReceiveString(char* buffer, uint16_t maxLength)
{
	int i = 0;
	char c;
	
	while(i < maxLength - 1)
	{
		c = UARTSoftware_Receive();
		if(c == '\n' || c == '\r')
		{
			break;
		}
			buffer[i++] = c;
	}
	buffer[i] = '\0';
}

char data_transmit[5] = {'C', 'U', 'O', 'N', 'G'};
char *str = "duccuong\n";
char rx_buffer[50] = {0};
char rx_receive[50] = {0};
char data;

int main(void)
{
	RCC_Config();
	TIM_Config();
	GPIO_Config();
	UART_Init();

//	UARTSoftware_TransmitString("Bat dau nhan UART...\n");

while (1)
{
    UARTSoftware_ReceiveString(rx_buffer, 50);

    if(strcmp(rx_buffer, "led") == 0)
    {
        UARTSoftware_TransmitString("Bat LED\n");
    }

}
	
}

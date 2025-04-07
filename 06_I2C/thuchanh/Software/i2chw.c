#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

#define I2C_SCL		GPIO_Pin_6
#define I2C_SDA		GPIO_Pin_7

#define I2C_GPIO  GPIOB

#define WRITE_SDA_0 GPIO_ResetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SDA_1 GPIO_SetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SCL_0 GPIO_ResetBits(I2C_GPIO, I2C_SCL)
#define WRITE_SCL_1 GPIO_SetBits(I2C_GPIO, I2C_SCL)
#define READ_SDA_VAL 	GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA)

typedef enum {
	NOT_OK, OK
} status;

typedef enum{
	NACK, ACK
} ACK_Bit;

void RCC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void TIM_Config()
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 72Mhz
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = 0xFFFF;
	TIM_InitStructure.TIM_Prescaler = 72 - 1; //1uS
  
  TIM_TimeBaseInit(TIM2, &TIM_InitStructure);
	TIM_Cmd(TIM2, ENABLE);
}

void delay_us(uint32_t time)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < time){}
}

void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // mode open-drain for I2C
	GPIO_InitStructure.GPIO_Pin = I2C_SDA | I2C_SCL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}

void I2C_Config()
{
	WRITE_SDA_1;
	delay_us(1);
	WRITE_SCL_1;
	delay_us(1);
}

void I2C_Start()
{
	WRITE_SDA_0; // SDA reset 0 before SCL
	delay_us(3);
	WRITE_SCL_0;
	delay_us(3);
}

void I2C_Stop()
{
	WRITE_SCL_1; // SCL set 1 before SDA
	delay_us(3);
	WRITE_SDA_1;
	delay_us(3);
}
	
status I2C_Master_Write(uint8_t u8Data) // Send Address/ Data to Slave, Slave receive -> send ACK
{
	status stRet;
	for(int i = 0; i < 8; i++)
	{
		if(u8Data & 0x80)
		{
			WRITE_SDA_1;
		}
		else
		{
			WRITE_SDA_0;
		}
		delay_us(3);
		
		// Clock
		WRITE_SCL_1;
		delay_us(5);
		WRITE_SCL_0;
		delay_us(5);
		
		// Shift 1 bit to the left
		u8Data <<= 1;
	}
	
	WRITE_SDA_1;        // Set SDA = 1 to read ACK from Slave (if Slave send ACK, SDA = 0)
	delay_us(3);
	WRITE_SCL_1;
	delay_us(3);
	
	if(READ_SDA_VAL)    // SDA = 0 -> ACK, SDA = 1 -> NACK
	{ 
		stRet = NOT_OK;
	}
	else
	{
		stRet = OK;
	}
	
	delay_us(2);        // Clock SCK = 5uS -> Read at the middle to avoid bouncing
	WRITE_SCL_0;
	delay_us(5);
	
	return stRet;
}
	
uint8_t I2C_Master_Read	(ACK_Bit _ACK)
{
	uint8_t i;
	uint8_t u8Ret = 0x00;
	WRITE_SDA_1;
	delay_us(3);
	for(i = 0; i < 8; ++i)
	{
		u8Ret <<= 1;
		delay_us(3);
		if(READ_SDA_VAL)  // read register of Slave on SDA bus
		{
			u8Ret |= 0x01;
		}
		delay_us(2);
		WRITE_SCL_0;
		delay_us(5);
	}
	if(_ACK)
	{
		WRITE_SDA_0;
	}
	else
	{
		WRITE_SDA_1;
	}
	delay_us(3);
	
	WRITE_SCL_1;
	delay_us(5);
	WRITE_SCL_0;
	delay_us(5);

	return u8Ret;
}

void I2C_Config(){
	I2C_InitTypeDef I2C_InitStruct;
	
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_ClockSpeed = 400000;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2; //(Low/ High)
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	
	I2C_Init(I2C1, &I2C_InitStruct);
	I2C_Cmd(I2C1, ENABLE);
}

int main(void)
{

}
	
	
	

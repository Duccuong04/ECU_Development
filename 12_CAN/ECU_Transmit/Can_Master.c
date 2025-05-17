#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_can.h"              // Keil::Device:StdPeriph Drivers:CAN

uint8_t dataTransmit[10] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0};

void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
}
	
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// C?u hình PA11 (CAN RX) là Input Pull-up
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// C?u hình PA12 (CAN TX) là Alternate Function Push-Pull
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
	
void CAN_Config(){
	CAN_InitTypeDef CAN_InitStructure;

	// C?u hình CAN
	CAN_InitStructure.CAN_TTCM = DISABLE;  // Time Triggered Communication Mode
	CAN_InitStructure.CAN_ABOM = ENABLE;   // Automatic Bus-Off Management
	CAN_InitStructure.CAN_AWUM = ENABLE;   // Automatic Wake-Up Mode
	CAN_InitStructure.CAN_NART = DISABLE;  // No Automatic Retransmission
	CAN_InitStructure.CAN_RFLM = DISABLE;  // Receive FIFO Locked Mode
	CAN_InitStructure.CAN_TXFP = ENABLE;   // Transmit FIFO Priority
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  // Ch? d? ho?t d?ng bình thu?ng

	// C?u hình th?i gian truy?n (bit timing)
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  // Synchronization Jump Width = 1 time quantum
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;  // Bit Segment 1 = 6 time quanta
	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;  // Bit Segment 2 = 8 time quanta
	CAN_InitStructure.CAN_Prescaler = 6;  // T?c d? baudrate = 36 MHz / (Prescaler * 12) = 500 Kbps
	CAN_Init(CAN1, &CAN_InitStructure);
}

void CAN_TransmitData(uint8_t* data, uint8_t length){
	CanTxMsg TxMessage;

	TxMessage.StdId = 0x125;  // S? d?ng d?nh danh 11-bit
	TxMessage.RTR = CAN_RTR_Data;  // Data Frame
	TxMessage.IDE = CAN_Id_Standard;  // Standard ID
	TxMessage.DLC = length;  // S? byte d? li?u

	// Copy d? li?u vào tru?ng d? li?u c?a khung
	for (int i = 0; i < length; i++) {
		TxMessage.Data[i] = data[i];
	}
	
	//Transmit Message
	uint8_t mailbox = CAN_Transmit(CAN1, &TxMessage);

	// Ch? cho d?n khi khung du?c truy?n thành công
	while (CAN_TransmitStatus(CAN1, mailbox) != CAN_TxStatus_Ok);
}

int main(){
	RCC_Config();
	GPIO_Config();
	CAN_Config();
	CAN_TransmitData(dataTransmit, 7);
	while(1){
	
	}
}
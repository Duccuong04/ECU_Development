#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_can.h"              // Keil::Device:StdPeriph Drivers:CAN

uint8_t dataReceive[8] = {0};

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
	CAN_InitStructure.CAN_TXFP = DISABLE;   // Transmit FIFO Priority
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  // Ch? d? ho?t d?ng bình thu?ng

	// C?u hình th?i gian truy?n (bit timing)
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  // Synchronization Jump Width = 1 time quantum
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;  // Bit Segment 1 = 6 time quanta
	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;  // Bit Segment 2 = 8 time quanta
	CAN_InitStructure.CAN_Prescaler = 6;  // T?c d? baudrate = 36 MHz / (Prescaler * 12) = 500 Kbps
	CAN_Init(CAN1, &CAN_InitStructure);
}

void CAN_FilterConfig(void) {
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	// S? d?ng b? l?c s? 0, ch? d? ID Mask, d?nh danh 11-bit
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x125 << 5;  // Ð?nh danh Standard ID 0x321
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFE0;  // M?t n? ch? so sánh 11-bit ID
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;  // Ðua khung vào FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

	// Kh?i t?o b? l?c
	CAN_FilterInit(&CAN_FilterInitStructure);
}

void CAN_ReceiveData(uint8_t* data) {
	while(CAN_MessagePending(CAN1, CAN_FIFO0) < 1);
	CanRxMsg RxMessage;

	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);  // Nh?n t? FIFO 0

	// Copy d? li?u nh?n du?c
	for (int i = 0; i < RxMessage.DLC; i++) {
		data[i] = RxMessage.Data[i];
	}
	CAN_FIFORelease(CAN1, CAN_FIFO0);
}

int main(){
	RCC_Config();
	GPIO_Config();
	CAN_Config();
	CAN_FilterConfig();
	while(1){
		CAN_ReceiveData(dataReceive);
	}
}
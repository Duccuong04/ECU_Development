# Làm quen với thư viện SPL (Standard Peripheral Library)

## Ví dụ GPIO

### 1. Cấp clock cho ngoại vi

Ví dụ hàm cấu hình xung clock

```c
void RCC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}

```
2 tham số:

- Ngoại vi muốn cấp clock

- ENABLE/ DISABLE

### 2. Cấu hình cho GPIO

```c
typedef struct
{
  uint16_t GPIO_Pin;             /*!< Specifies the GPIO pins to be configured.
                                      This parameter can be any value of @ref GPIO_pins_define */

  GPIOSpeed_TypeDef GPIO_Speed;  /*!< Specifies the speed for the selected pins.
                                      This parameter can be a value of @ref GPIOSpeed_TypeDef */

  GPIOMode_TypeDef GPIO_Mode;    /*!< Specifies the operating mode for the selected pins.
                                      This parameter can be a value of @ref GPIOMode_TypeDef */
}GPIO_InitTypeDef;

```

- Pin sử dụng, tốc độ & mode

```c
void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;           // PC13
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;     // Mode output push pull
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;    // Speed	
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);               // save setting at register
}
```

```c
typedef enum
{ GPIO_Mode_AIN = 0x0,              // analog input
  GPIO_Mode_IN_FLOATING = 0x04,     // input floating
  GPIO_Mode_IPD = 0x28,             // input pull up
  GPIO_Mode_IPU = 0x48,             // input pull down
  GPIO_Mode_Out_OD = 0x14,          // open drain
  GPIO_Mode_Out_PP = 0x10,          // output push-pull
  GPIO_Mode_AF_OD = 0x1C,           
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;
```
### 3. Các hàm sử dụng ngoại vi GPIO

```c
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
\\Đọc giá trị 1 chân trong GPIO được cấu hình là INPUT
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
\\Đọc giá trị nguyên GPIO được cấu hình là INPUT
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
\\Đọc giá trị 1 chân trong GPIO được cấu hình là OUTPUT
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
\\Đọc giá trị nguyên GPIO được cấu hình là OUTPUT
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
\\Cho giá trị điện áp của 1 chân trong GPIO = 1
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
\\Cho giá trị điện áp của 1 chân trong GPIO = 0
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
\\Ghi giá trị "BitVal" vào 1 chân trong GPIO
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
\\Ghi giá trị "PortVal" vào nguyên GPIO
```

## Ví dụ Timer - Interrupt

- Cấp clock cho TIM2

```c
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
```

- Cấu hình TIMER - interrupt

```c
void TIM_Config()
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_Prescaler = 7200 - 1;  // Ttimer = 0.1ms -> counter ++ 
	TIM_InitStruct.TIM_Period = 10000 - 1;       // Period (ARR) = 10000, 1s trafn 1 time
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // Tck_psc = 72MHZ
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  // kich hoat ngat update event
	TIM_Cmd(TIM2, ENABLE); // allow TIMER, counter
  
	// Setting NVIC, 
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}
```

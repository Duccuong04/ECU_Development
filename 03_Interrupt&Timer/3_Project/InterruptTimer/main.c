#define TIM2_BASE			0x40000000
#define TIM2_CR1			(*(volatile unsigned int*)(TIM2_BASE + 0x00))
#define TIM2_DIER			(*(volatile unsigned int*)(TIM2_BASE + 0x0C))
#define TIM2_SR			  (*(volatile unsigned int*)(TIM2_BASE + 0x10))
#define TIM2_PSC			(*(volatile unsigned int*)(TIM2_BASE + 0x28))
#define TIM2_ARR			(*(volatile unsigned int*)(TIM2_BASE + 0x2C))
#define TIM2_CNT			(*(volatile unsigned int*)(TIM2_BASE + 0x24))

#define RCC_BASE		  0x40021000
#define RCC_APB1ENR   (*(volatile unsigned int*)(RCC_BASE + 0x1C))
#define RCC_ABP2ENR   (*(volatile unsigned int*)(RCC_BASE + 0x18))
	
#define GPIOC_BASE    0x40011000
#define GPIO_CRH      (*(volatile unsigned int*)(GPIOC_BASE + 0x04))
#define GPIO_ODR      (*(volatile unsigned int*)(GPIOC_BASE + 0x0C))
	
#define NVIC_ISER0    (*(volatile unsigned int*)0xE000E100)
	
void TIM2_IRQHandler(void)
{
	if(TIM2_SR & 1)    // check UIF (Update Interrupt Flag)
	{
		TIM2_SR &= ~1;   // delete flag interrupt UIF = 0
		
		GPIO_ODR ^= (1 << 13); // Toggle PIN PC13 by XOR
		
	}	
}

void RCC_Config()
{
	RCC_APB1ENR |= 1;  // provide clock to GPIOC
	RCC_ABP2ENR |= 1 << 4; // provide clock to TIM2
}

void TIM_Config()
{
	TIM2_PSC = 7200 -1; // Prescaler = 7200 -> Timer will run on 1kHz
	TIM2_ARR = 10000 -1;// Auto - reload
	
	TIM2_DIER |= (1 << 0); // enable update event
	TIM2_CR1 = (1 << 0);   // enable counter
	
	NVIC_ISER0 |= (1 << 28);	
}

void GPIO_Config()
{
	GPIO_CRH &= ~((1 << 23) | (1 << 22));
	GPIO_CRH |= (1 << 21) | (1 << 20);
}

int main()
{
	RCC_Config();
	GPIO_Config();
	TIM_Config();
	while(1)
	{
		// do nothing
	}
}


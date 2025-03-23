#define RCC ((RCC_TypeDef *)0x40021000)    // define first address of element in Typedef
#define GPIOC ((GPIO_TypeDef *)0x40011000)
#define GPIOA ((GPIO_TypeDef *)0x40010800)

/*
* Use struct to define shortly
*/
	
typedef struct
{
  unsigned int CRL;
  unsigned int CRH;
  unsigned int IDR;
  unsigned int ODR;
  unsigned int BSRR;
  unsigned int BRR;
  unsigned int LCKR;
} GPIO_TypeDef;


typedef struct
{
  unsigned int CR;
  unsigned int CFGR;
  unsigned int CIR;
  unsigned int APB2RSTR;
  unsigned int APB1RSTR;
  unsigned int AHBENR;
  unsigned int APB2ENR;
  unsigned int APB1ENR;
  unsigned int BDCR;
  unsigned int CSR;
} RCC_TypeDef;


void delay(unsigned int time) {
    for (unsigned int i = 0; i < time; i++);
}

void WritePin(GPIO_TypeDef *GPIO_Port, unsigned char Pin, unsigned char state)
{
	if(state == 1)
	GPIO_Port->ODR |= (1 << Pin);
	else
	GPIO_Port->ODR &= ~(1 << Pin);
}

void GPIO_Config()
{
	// enable RCC for GPIOA & GPIOC
		RCC->APB2ENR |= ((1 << 4) | (1 << 2)); 
	// PC13 Configuration
    GPIOC->CRH &= ~((1 << 23) | (1 << 22));  
    GPIOC->CRH |= ((1 << 21) | (1 << 20));  
	// PA0 Configuration
		GPIOA->CRL |= (1 << 3);
		GPIOA->CRL &= ~(7);
		GPIOA->ODR |= (1 << 0);
}

int main() {
	
	GPIO_Config();

	while(1){
		if((GPIOA->IDR & (1 << 0)) == 1) // Ð?c tr?ng thái nút nh?n
		{
		GPIOC->ODR = 0 << 13; // N?u PA0 = 0 -> PC13 = 0
		}
		else
		{
		GPIOC->ODR = 1 << 13; // N?u PA0 = 1 -> PC13 = 1
		}
}
}

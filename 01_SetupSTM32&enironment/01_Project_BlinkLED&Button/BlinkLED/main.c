	
#define RCC ((RCC_TypeDef *)0x40021000)    // define first address of element in Typedef
#define GPIOC ((GPIO_TypeDef *)0x40011000)

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

int main() {
	
	  RCC->APB2ENR |= (1 << 4);  
    GPIOC->CRH &= ~((1 << 23) | (1 << 22));  
    GPIOC->CRH |= ((1 << 21) | (1 << 20));  

    while (1) {
        GPIOC->ODR |= (1 << 13);  
        delay(10000000);
        GPIOC->ODR &= ~(1 << 13);  
        delay(10000000);
    }
}

/*

 Define Registers
#define RCC_APB2ENR  *((unsigned int *)0x40021018)
#define GPIOC_CRH    *((unsigned int *)0x40011004)
#define GPIOC_ODR    *((unsigned int *)0x4001100C)


#define RCC ((RCC_TypeDef *)0x40021000)    // define first address of element in Typedef
#define GPIOC ((GPIO_TypeDef *)0x40011000)

/*
* Use struct to define shortly

	
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

int main() {
	
/*
    RCC_APB2ENR |= (1 << 4);  
    GPIOC_CRH &= ~((1 << 23) | (1 << 22));  
    GPIOC_CRH |= ((1 << 21) | (1 << 20));  

    while (1) {
        GPIOC_ODR |= (1 << 13);  
        delay(10000000);
        GPIOC_ODR &= ~(1 << 13);  
        delay(10000000);
    }
}

	RCC->APB2ENR |= (1 << 4);  
    GPIOC->CRH &= ~((1 << 23) | (1 << 22));  
    GPIOC->CRH |= ((1 << 21) | (1 << 20));  

    while (1) {
        GPIOC->ODR |= (1 << 13);  
        delay(10000000);
        GPIOC->ODR &= ~(1 << 13);  
        delay(10000000);
    }
}
*/


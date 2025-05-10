#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_flash.h"            // Keil::Device:StdPeriph Drivers:Flash

#define ADDR_STR_BLINK 0x08000800

void Boot() {
    // Thiet lap lai he thong clock RCC
    RCC_DeInit();
    
    // Vo hieu hoa cac ngat tranh loi trong qua trinh
    SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);
    
    // Cap nhat Stack Pointer tu bang VTOR ngat chuong trinh chinh
    __set_MSP(*(__IO uint32_t*)(ADDR_STR_BLINK));
    
    // C?p nh?t thanh ghi SCB->VTOR d? tr? d?n b?ng vector ng?t c?a chuong trình chính
    SCB->VTOR = ADDR_STR_BLINK;

    // L?y d?a ch? Reset Handler c?a chuong trình chính
    uint32_t JumpAddress = *(__IO uint32_t*)(ADDR_STR_BLINK + 4);
    
    // T?o con tr? hàm d?n Reset Handler
    void (*reset_handler)(void) = (void (*)(void)) JumpAddress;
    
    // Nh?y vào Reset Handler c?a chuong trình chính
    reset_handler();
}

void Flash_Erase1Page(uint32_t PageAddress)
{
	FLASH_Unlock();
	FLASH->CR |= FLASH_CR_PER;   // bat bit CR PER
	FLASH->AR = PageAddress;     
	FLASH->CR |= FLASH_CR_STRT;
	while((FLASH->SR & FLASH_SR_BSY) == SET);
	FLASH->CR &= ~FLASH_CR_PER;  // tat bit CR_PER
	FLASH_Lock();	
}


void Flash_Programming_HalfWord(uint32_t Address, uint16_t data)
{
    FLASH_Unlock();
    FLASH->CR |= FLASH_CR_PG;  // B?t ch? d? l?p trình (Program)

    *(uint16_t*)Address = data;  // Ghi 16-bit
    while (FLASH->SR & FLASH_SR_BSY);  // Ch? ghi xong

    FLASH->CR &= ~FLASH_CR_PG;  // T?t ch? d? l?p trình
    FLASH_Lock();
}


int main(void)
{
	Flash_Erase1Page(0x08000800);
	Flash_Programming_HalfWord(0x08000820, 0x1234);
	while(1){}
}


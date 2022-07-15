#include <stdint.h>
#include "stm32f10x.h"
#include "Initialize.h"



void clockInit(void)
{
  
//* enable HSI and wait for it to be ready
    RCC->CR |= 0x00000001;
    while (((RCC->CR) & 0x00000003) == 0);
			
//* enable HSE with Bypass and wait for it to be ready
    RCC->CR |= 0x00050000;
    while (((RCC->CR) & 0x0007000) == 0);
				
//* enable flash prefetch buffer */
//FLASH->ACR = 0x00000012;
    
////SET HSI as SYSCLK and wait for it to be recognized
//    RCC->CFGR = 0x04000000;
//    while (((RCC->CFGR) & 0x0000000C) != 0);
			
//SET HSE as SYSCLK and wait for it to be recognized
    RCC->CFGR = 0x04000001;
    while (((RCC->CFGR) & 0x00000005) == 0);
			
// Disable PLL. 
		
		RCC->CR &= ~RCC_CR_PLLON;
			
//Change PLL source and set the PLL multiplier
			
//These are the SYSCLK values when using the PLL with HSI/2 as the input. The max value is 64 MHz
		//RCC->CFGR = 0x00000000;// 8MHz
		//RCC->CFGR = 0x00040000;// 12 MHz
		//RCC->CFGR = 0x00080000;// 16 MHz
		//RCC->CFGR = 0x000c0000;// 20 MHz			
		RCC->CFGR = 0x00100000;// 24 MHz **** MAX SPEED for VLDISCOVERY *****
		//RCC->CFGR = 0x00140000;//	28 MHz
		//RCC->CFGR = 0x00180000;// 32 MHz
		//RCC->CFGR = 0x001C0000;// 36 MHz			
		//RCC->CFGR = 0x00200000;// 40 MHz
		//RCC->CFGR = 0x00240000;//	44 MHz
		//RCC->CFGR = 0x00280000;// 48 MHz
		//RCC->CFGR = 0x002C0000;// 52 MHz
		//RCC->CFGR = 0x003C0000;// 64 MHz			

//ENABLE PLL and wait for it to be ready

    RCC->CR |= RCC_CR_PLLON;
    while (((RCC->CR) & 0x03000000) == 0);
   
// Set PLL as SYSCLK and wait for it to be ready
			
    RCC->CFGR |= 0x00000002;
    while (((RCC->CFGR) & 0x00000008) == 0);
			
// Enable clock visibity on PA8, select PLL/2 as output source
			
		RCC->CFGR |= RCC_CFGR_MCO_PLL;

	
//Enable peripheral clocks for various ports and subsystems
//Bit 4: Port C Bit3: Port B Bit 2: Port A
			
    RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_ADC1EN;
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
		
// Write a 0xB ( 1011b ) into the configuration and mode bits for PA8 (AFIO)
    GPIOA->CRH |= GPIO_CRH_CNF8_1 | GPIO_CRH_MODE8_1 | GPIO_CRH_MODE8_0 ;
    GPIOA->CRH &= ~GPIO_CRH_CNF8_0 ;
		
		//setting up bits for PA6
		GPIOA->CRL |= GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6_1 | GPIO_CRL_MODE6_0 ;
		GPIOA->CRL &= ~GPIO_CRL_CNF6_0 ;
		//RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
		
		//Set the config and mode bits for Port A bit 5 so it will be a push-pull output (up to 50 MHz)
    GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->CRL &= ~GPIO_CRL_CNF5;
		
		
		GPIOA->CRL |= GPIO_CRL_MODE7; //GPIOA -> CR (low), PA7
    GPIOA->CRL &= ~GPIO_CRL_CNF7;
		
		
		//PA8 to PA11 are pins that will be used to output the LEDS and the motor driving signals
		GPIOA->CRH |= GPIO_CRH_MODE8; //GPIOA -> CR (high), PA8
    GPIOA->CRH &= ~GPIO_CRH_CNF8;
		
		GPIOA->CRH |= GPIO_CRH_MODE9; //GPIOA -> CR (high), PA9
    GPIOA->CRH &= ~GPIO_CRH_CNF9;
		
		GPIOA->CRH |= GPIO_CRH_MODE10; //GPIOA -> CR (high), PA10
    GPIOA->CRH &= ~GPIO_CRH_CNF10;
		
		GPIOA->CRH |= GPIO_CRH_MODE11; //GPIOA -> CR (high), PA11
    GPIOA->CRH &= ~GPIO_CRH_CNF11;
		
		GPIOA->CRL &=  ~GPIO_CRL_CNF4 & ~GPIO_CRL_MODE4; //PA4 for ADC12_IN4
		GPIOA->CRL &=  ~GPIO_CRL_CNF4 & ~GPIO_CRL_MODE4; //PA5 for ADC12_IN4
				
		ADC1->CR2 |= (ADC_CR2_ADON); //Turning on the ADC system
   // ADC1->CR2 &= ~(ADC_CR2_ADON);
	  
}

//**************************** I/O ************************************************************

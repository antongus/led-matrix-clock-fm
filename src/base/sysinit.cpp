#include "stm32f10x.h"

static inline void init_clocks()
{
	// enable HSI
	RCC->CR |= RCC_CR_HSION;
	while (!(RCC->CR & RCC_CR_HSIRDY)) ;

	// turn off all other clocks
	RCC->CR &= RCC_CR_HSITRIM | RCC_CR_HSION;
	RCC->CFGR = 0;                                            // CFGR reset value.
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) ;  // Wait until HSI is selected.

    // HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK/2
    RCC->CFGR = RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_ADCPRE_DIV6;

    //  PLL configuration:  = (HSI / 2) * 6 = 24 MHz
    RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLMULL6;

	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY)) ;

	// Select PLL as system clock source
	RCC->CFGR |= RCC_CFGR_SW_PLL;

    // Wait till PLL is used as system clock source
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) ;
}

extern "C" void init_HW(void);

void init_HW(void)
{
	// RCC system reset(for debug purpose)
	// Set HSION bit
	RCC->CR |= RCC_CR_HSION;
	// Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], ADCPRE[1:0] and MCO[2:0] bits
#ifndef STM32F10X_CL
	RCC->CFGR &= (uint32_t)0xF8FF0000;
#else
	RCC->CFGR &= (uint32_t)0xF0FF0000;
#endif
	// Reset HSEON, CSSON and PLLON bits
	RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON);
	// Reset HSEBYP bit
	RCC->CR &= ~RCC_CR_HSEBYP;
	// Reset PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE bits
	RCC->CFGR &= (uint32_t)0xFF80FFFF;

#ifdef STM32F10X_CL
	// Reset PLL2ON and PLL3ON bits
	RCC->CR &= (uint32_t)0xEBFFFFFF;

	// Disable all interrupts and clear pending bits
	RCC->CIR = 0x00FF0000;

	// Reset CFGR2 register
	RCC->CFGR2 = 0x00000000;
#elif defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) 
	// Disable all interrupts and clear pending bits
	RCC->CIR = 0x009F0000;

	// Reset CFGR2 register
	RCC->CFGR2 = 0x00000000;      
#else
	// Disable all interrupts and clear pending bits
	RCC->CIR = 0x009F0000;
#endif

	init_clocks();

	// enable IOPx periph
	RCC->APB2ENR |=
			RCC_APB2ENR_IOPAEN |
			RCC_APB2ENR_IOPBEN |
			RCC_APB2ENR_IOPCEN |
			RCC_APB2ENR_IOPDEN |
#ifdef RCC_APB2ENR_IOPEEN
			RCC_APB2ENR_IOPEEN |
#endif
			RCC_APB2ENR_AFIOEN;

//	NVIC_SetPriorityGrouping(7);	// no preemption,           4 bit of subprio (0..15)
//	NVIC_SetPriorityGrouping(6);	// 1 bit preemption (0..1), 3 bit of subprio (0..7)
	NVIC_SetPriorityGrouping(5);	// 2 bit preemption (0..3), 2 bit of subprio (0..3)
//	NVIC_SetPriorityGrouping(4);	// 3 bit preemption (0..3), 1 bit of subprio (0..1)
//	NVIC_SetPriorityGrouping(3);	// 4 bit preemption(0..15), no subprio
}


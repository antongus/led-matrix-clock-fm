#include "hw.h"

console_uart_t& uart = singleton<console_uart_t>::Instance();

Rtc rtc;
kbd_t kbd;
SpiType spi;
TMax Max;
FmTuner tuner;

extern "C" OS_INTERRUPT void USART1_IRQHandler()
{
	OS::TISRW ISR;
	uart.irq_handler();
}

extern "C" OS_INTERRUPT void USART2_IRQHandler()
{
	OS::TISRW ISR;
	uart.irq_handler();
}



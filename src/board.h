/**
*  @file board.h
*
*  Created on: 23.11.2012
*  Copyright (c) Anton Gusev aka AHTOXA
**/

#ifndef BOARD_H_
#define BOARD_H_

#include "pin.h"
#include "stm32_spi.h"

struct BoardDiscovery
{
	typedef Spi<SPI_1, SPI_DIV_2> SpiType;
	typedef Pin<'C', 1, 'L'> LATCH;
	typedef uart_t<UART_1, 115200> console_uart_t;
	typedef dbg_uart_t<UART_1, 115200> debug_uart_t;
	typedef Pin<'C', 9, 'H'> GreenLed;
	typedef Pin<'C', 8, 'H'> BlueLed;
	typedef Pin<'A', 0, 'H'> Button;

};

struct BoardGreen
{
	typedef Spi<SPI_1, SPI_DIV_16> SpiType;
	typedef Pin<'C', 4, 'L'> LATCH;
	typedef uart_t<UART_2, 115200> console_uart_t;
	typedef dbg_uart_t<UART_2, 115200> debug_uart_t;
};

#ifdef STM32F10X_MD
typedef BoardGreen BoardType;
#else
typedef BoardDiscovery BoardType;
#endif



#endif /* BOARD_H_ */

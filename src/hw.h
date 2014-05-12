#ifndef HW_H_INCLUDED
#define HW_H_INCLUDED

#include "stm32.h"
#include "pin.h"
#include "singleton.h"
#include "uart.h"
#include "dbg_uart.h"
#include "STM32-VLD.h"
#include "board.h"

typedef BoardType::console_uart_t console_uart_t;
typedef BoardType::debug_uart_t debug_uart_t;
typedef BoardType::SpiType SpiType;

extern console_uart_t& uart;
extern SpiType spi;

/**
 * RTC
 */
#include "rtc.h"
typedef rtc_t<> Rtc;
extern Rtc rtc;

/**
 * Keyboard stuff
 */
typedef Pin<'B', 9, 'L'> PinUp;
typedef Pin<'B', 8, 'L'> PinDown;

#include "kbd.h"
typedef keyboard_t<8> kbd_t;
extern kbd_t kbd;

enum
{
	BUTTON_UP = 0x01,
	BUTTON_DOWN = 0x02
};

template <int buf_size>
uint16_t keyboard_t<buf_size>::read_input()
{
	uint16_t ret = 0;
	if (PinUp::Signalled()) ret |= BUTTON_UP;
	if (PinDown::Signalled()) ret |= BUTTON_DOWN;
	return ret;
}

/**
 * LED matrix
 */
#include "max7219_soft.h"

struct Max7219Props
{
	enum
	{
		CHIP_COUNT = 3,
		DEFAULT_BRIGHTNESS = 7,
		NOPS_AFTER_SELECT = 1,
		NOPS_BEFORE_DESELECT = 8
	};
	typedef Pin<'A', 3, 'H'> CLK;
	typedef Pin<'A', 4, 'L'> LATCH;
	typedef Pin<'A', 5, 'H'> DATA;
};

typedef TMax7219Chain<Max7219Props> TMax;
extern TMax Max;

/**
 * FM tuner
 */
#include "tea5767.h"
typedef Tea5767<> FmTuner;
extern FmTuner tuner;

/**
 * Command-line interface
 */
#include "commands2.h"
#include "vars2.h"

#endif // HW_H_INCLUDED

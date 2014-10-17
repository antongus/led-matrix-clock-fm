/**
*  @file hw.h
*
*  Project-level hardware declarations.
*
*  Created on: 20.04.2014
*  Copyright (c) 2014 by Anton Gusev aka AHTOXA
**/
#ifndef HW_H_INCLUDED
#define HW_H_INCLUDED

/*
 * Firmware version signature.
 */
#define xstr(s) str(s)
#define str(s) #s
#define FW_VERSION_STR         "Версия" xstr(VER_MAJOR) "." xstr(VER_MINOR)
#define FW_VERSION             ((VER_MAJOR * 10.0) + VER_MINOR) / 10.0

#include "stm32.h"

/**
 * RTC module
 */
#include "rtc.h"
extern RtcModuleLSE rtc;

/**
 * Keyboard stuff
 */
typedef Pin<'B', 9, 'L'> PinUp;
typedef Pin<'B', 8, 'L'> PinDown;

#include "kbd.h"
typedef Keyboard<8> KeyboardType;
extern KeyboardType kbd;

enum
{
	BUTTON_UP = 0x01,
	BUTTON_DOWN = 0x02
};

template <int buf_size>
uint16_t Keyboard<buf_size>::ReadInput()
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

#endif // HW_H_INCLUDED

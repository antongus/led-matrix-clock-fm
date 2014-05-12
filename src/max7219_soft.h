/**
*  @file max7219.h
*  LED driver MAX7219 class template
*
*  Copyright (c) 2011 by Anton Gusev aka AHTOXA
*
*/
#ifndef MAX7219_SOFT_INCLUDED
#define MAX7219_SOFT_INCLUDED
#include <stdint.h>
#include "pin.h"

struct SampleMax7219Props
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

template<typename props = SampleMax7219Props>
class TMax7219Chain
{
public:
	enum
	{
		CHIP_COUNT = props::CHIP_COUNT,
		DIGITS_PER_CHIP = 8,
		DIGIT_COUNT = CHIP_COUNT * DIGITS_PER_CHIP,
		DEFAULT_BRIGHTNESS = props::DEFAULT_BRIGHTNESS,
		NOPS_AFTER_SELECT = props::NOPS_AFTER_SELECT,
		NOPS_BEFORE_DESELECT = props::NOPS_BEFORE_DESELECT
	};
	typedef typename props::CLK CLK;
	typedef typename props::LATCH LATCH;
	typedef  typename props::DATA DATA;

	TMax7219Chain()
		: brightness_(DEFAULT_BRIGHTNESS)
		{
		}
	void Init();
	void SetSleep(bool value) { WriteAll(value ? 0x0C00 : 0x0C01); }
	void SetTest(bool value) { WriteAll(value ? 0x0F01 : 0x0F00); }
	void SetDecodeMode(uint8_t flags);
	void SetUsedDigits(uint8_t value);
	void Cls(void);
	void Test(int delay = 2);
	void SetBrightness(uint8_t value);
	void SetBrightnessVal(uint8_t value) { brightness_ = value; }
	uint8_t GetBrightness(void) { return brightness_; };
	void SetDigit(int digit, int value);
private:
	uint8_t brightness_;
	void Select(void);
	void Deselect(void);
	void Write(uint16_t w);
	void WriteAll(uint16_t w);
};

template<typename props>
void TMax7219Chain<props>::Init(void)
{
	CLK::Mode(OUTPUT); CLK::Off();
	LATCH::Mode(OUTPUT); LATCH::On();
	DATA::Mode(OUTPUT); DATA::Off();
	SetSleep(false);
	SetTest(false);
	SetDecodeMode(0x00);
	SetUsedDigits(0xFF);
	SetDecodeMode(0x00);
	SetBrightness(brightness_);
}

template<typename props>
void TMax7219Chain<props>::Select()
{
	LATCH::On();
	for (int i = 0; i < NOPS_AFTER_SELECT; i++)
		__asm__ __volatile__ ("nop");
}

template<typename props>
void TMax7219Chain<props>::Deselect()
{
	for (int i = 0; i < NOPS_BEFORE_DESELECT; i++)
		__asm__ __volatile__ ("nop");
	LATCH::Off();
}

template<typename props>
void TMax7219Chain<props>::Write(uint16_t w)
{
	for (int i = 0; i < 16; i++)
	{
		if (w & 0x8000)
			DATA::On();
		else
			DATA::Off();
		CLK::Off();
		w <<= 1;
		CLK::On();
	}
}

template<typename props>
void TMax7219Chain<props>::WriteAll(uint16_t w)
{
	Select();
	for (int i = CHIP_COUNT; i; i--)
		Write(w);
	Deselect();
}

template<typename props>
void TMax7219Chain<props>::SetDecodeMode(uint8_t flags)
{
	uint16_t w = 0x0900 | flags;
	WriteAll(w);
}

template<typename props>
void TMax7219Chain<props>::SetUsedDigits(uint8_t value)
{
	uint16_t w = 0x0B00 | value;
	WriteAll(w);
}

template<typename props>
void TMax7219Chain<props>::Cls(void)
{
	for (uint16_t digit = 1; digit < 9; digit++)
		WriteAll(digit << 8);
}

template<typename props>
void TMax7219Chain<props>::Test(int delay)
{
	for (int digit = 1; digit <= DIGIT_COUNT; digit++)
	{
		uint8_t mask = 1;
		for (int i = 0; i < DIGITS_PER_CHIP; i++)
		{
			SetDigit(digit, mask);
			mask <<= 1;
			mask |= 1;
			OS::sleep(delay);
		}
	}
}

template<typename props>
void TMax7219Chain<props>::SetBrightness(uint8_t value)
{
	uint16_t w;
	value &= 0x0F;
	brightness_ = value;
	w = 0x0A00 | value;
	WriteAll(w);
}

template<typename props>
void TMax7219Chain<props>::SetDigit(int digit, int value)
{
	digit -= 1;
	int targetChip = digit / DIGITS_PER_CHIP;
	digit %= DIGITS_PER_CHIP;
	digit += 1;
	uint16_t w = (digit << 8) | value;

	Select();

	for (int i = CHIP_COUNT - 1; i >= 0; i--)
		Write(i == targetChip ? w : 0x0000);

	Deselect();
}


#endif // MAX7219_SOFT_INCLUDED


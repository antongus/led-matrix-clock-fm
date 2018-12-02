/**
*  @file display.cpp
*
*  Created on: 20.04.2014
*  Copyright (c) 2014 by Anton Gusev aka AHTOXA
**/

#include "hw.h"
#include "display.h"
#include "textbuf.h"

Display display;

void Display::DrawFrame(Coord x, Coord y)
{
	drawBuf_.setReadOrigin(x, y);
	for (int digit = 1; digit <= Max.DIGIT_COUNT; digit++)
		Max.SetDigit(XlatCol(digit), XlatRow(drawBuf_.getByte(digit-1, 0)));
}

void Display::ScrollLine(char const* line, bool scrollOut)
{
	Coord x = 24, y = 0;
	drawBuf_.clear();
	drawBuf_.puts(x, y, line);
	drawBuf_.setReadOrigin(0, 0);
	int width = scrollOut ? x : x - 24;	for (int i = 0; i < width; i++)
	{
		DrawFrame(i, 0);
		if (kbd.Keypressed())
			break;
		OS::sleep(40);
	}
}

/**
 * Display static text.
 */
void Display::StaticText(char const* line)
{
	Coord x = 0, y = 0;
	drawBuf_.clear();
	drawBuf_.puts(x, y, line);
	drawBuf_.setReadOrigin(0, 0);
	DrawFrame(0, 0);
}

/**
 * Translate column number.
 * (Columns are connected to the driver not in sequence, to simplify board routing).
 * Argument is 1-based.
 */
uint8_t Display::XlatCol(uint8_t col)
{
	static uint8_t columnsXlat[] = {7, 0, 4, 2, 6, 3, 1, 5};
	--col;
	col = (col / 8) * 8 + columnsXlat[col % 8];
	return ++col;
}

uint8_t Display::XlatRow(uint8_t row)
{
	uint8_t res = 0;
	if (row & (1 << 7)) res |= (1 << 7);
	if (row & (1 << 6)) res |= (1 << 2);
	if (row & (1 << 5)) res |= (1 << 0);
	if (row & (1 << 4)) res |= (1 << 4);
	if (row & (1 << 3)) res |= (1 << 1);
	if (row & (1 << 2)) res |= (1 << 3);
	if (row & (1 << 1)) res |= (1 << 5);
	if (row & (1 << 0)) res |= (1 << 6);
	return res;
}

void Display::AnimateChange(char* bufOld, char* bufNew)
{
	for (int step = 0; step < 8; step++)
	{
		Coord x = 0, y = 0;
		for (int i = 0; i < 4; i++)
		{
			char ch1 = bufOld[i];
			char ch2 = bufNew[i];
			if (ch1 == ch2)
				drawBuf_.putch(x, y, ch1);
			else
				drawBuf_.put2ch(x, y, ch1, ch2, step);
			if (i == 1)
				drawBuf_.putColumn(x++, y, 0);
			drawBuf_.setPixel(11, 7, now_ & 1);
			drawBuf_.setPixel(12, 7, !(now_ & 1));
			DrawFrame(0, 0);
			if (kbd.Keypressed())
				return;
			OS::sleep(20);
		}
	}
}

#ifdef USE_RTC
void Display::TimeToBuf(char* buf, time_t t)
{
	struct tm stm;
	TimeUtil::localtime(t, &stm);

	TextBuffer<5> tmpBuf;

	if (stm.tm_hour < 10) tmpBuf << '0';
	tmpBuf << stm.tm_hour;
	if (stm.tm_min < 10) tmpBuf << '0';
	tmpBuf << stm.tm_min;
	*buf++ = tmpBuf[0];
	*buf++ = tmpBuf[1];
	*buf++ = tmpBuf[2];
	*buf++ = tmpBuf[3];
	*buf=0;
}
#endif

#ifdef USE_RTC
void Display::DrawTime(Coord x, Coord y)
{
	char buf[8];

	now_ = rtc.ReadTime();
	TimeToBuf(buf, now_);

	Coord drawX = x;
	Coord drawY = y;

	drawBuf_.putch(drawX, drawY, buf[0]);
	drawBuf_.putch(drawX, drawY, buf[1]);
	drawBuf_.putColumn(drawX++, y, 0);
	drawBuf_.putch(drawX, drawY, buf[2]);
	drawBuf_.putch(drawX, drawY, buf[3]);

	drawBuf_.setPixel(x + 11, y + 7, now_ & 1);
	drawBuf_.setPixel(x + 12, y + 7, !(now_ & 1));
}

void Display::AnimateTime(Coord x, Coord y)
{
	char buf[8];

	now_ = rtc.ReadTime();
	TimeToBuf(buf, now_);

	if ((now_ % 60 == 0) && (now_ / 60) != (lastTime_ / 60)) // minute change
	{
		char bufNew[8];
		TimeToBuf(bufNew, lastTime_);
		lastTime_ = now_;
		AnimateChange(buf, bufNew);
	}

	Coord drawX = x;
	Coord drawY = y;

	drawBuf_.putch(drawX, drawY, buf[0]);
	drawBuf_.putch(drawX, drawY, buf[1]);
	drawBuf_.putColumn(drawX++, y, 0);
	drawBuf_.putch(drawX, drawY, buf[2]);
	drawBuf_.putch(drawX, drawY, buf[3]);

	drawBuf_.setPixel(x + 11, y + 7, now_ & 1);
	drawBuf_.setPixel(x + 12, y + 7, !(now_ & 1));
	DrawFrame(x, y);
}

/**
 * Scroll time vertically (in or out)
 */
void Display::ScrollTime(bool out)
{
	Coord y = out ? 0 : 8;
	drawBuf_.clear();
	DrawTime(0, y);
	for (int i = 0; i < 8; i++)
	{
		DrawFrame(0, i);
		if (kbd.Keypressed())
			break;
		OS::sleep(40);
	}
}

#endif

/**
*  @file display.h
*
*  Created on: 19.04.2014
*  Copyright (c) 2014 by Anton Gusev aka AHTOXA
**/

#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED


#include <scmRTOS.h>
#include "drawbuf.h"
#include "rtc.h"

class Display
{
public:
	Display()
		: drawBuf_()
	{}

	void ScrollLine(char const* line, bool scrollOut = true);
	void ScrollTime(bool out);
	void StaticText(char const* line);
	void DrawTime(Coord x, Coord y);
	void AnimateTime(Coord x, Coord y);
private:
	enum { TX_BUF_SIZE = 4 };

	DrawBuffer<256, 16> drawBuf_;
	Coord x_;
	Coord y_;
	Coord width_;
	time_t now_;
	time_t lastTime_;

	uint8_t XlatCol(uint8_t val);
	uint8_t XlatRow(uint8_t val);

	void DrawFrame(Coord x, Coord y);
	void PrintTime(char* buf);
	void TimeToBuf(char* buf, time_t t);
	void AnimateChange(char* bufOld, char* bufNew);
};

extern Display display;

#endif // DISPLAY_H_INCLUDED

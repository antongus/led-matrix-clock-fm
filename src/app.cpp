/**
*  @file app.cpp
*
*  Created on: 27.04.2014
*  Copyright (c) 2014 by Anton Gusev aka AHTOXA
**/

#include "app.h"

#include "hw.h"
#include <cstdlib>
#include "stations.h"
#include "display.h"
#include "textbuf.h"
#include <atomic>

App app;

void App::InitHardware()
{
	PinUp::Mode(INPUTPULLED);
	PinUp::PullUp();
	PinDown::Mode(INPUTPULLED);
	PinDown::PullUp();

	Max.SetBrightnessVal(BKP->DR3);
	Max.Init();
	Max.Cls();
}

OS_PROCESS void App::UserInterfaceLoop()
{
	OS::sleep(800);
	SelectStation(BKP->DR2);   // load station number from battery-backed RAM

	for (;;)
	{
		OS::sleep(20);

		Max.Init();

		if (kbd.Keypressed())
		{
			int ret = kbd.GetChar();
			if (ret == BUTTON_UP)
				SelectStation(station_ + 1);
			else if (ret == BUTTON_DOWN)
				SelectStation(station_ - 1);
			else if (ret == (BUTTON_UP | BUTTON_DOWN))
				EditConfig();
			continue;
		}

#ifdef USE_RTC
		time_t t = rtc.ReadTime();
		struct tm stm;
		TimeUtil::localtime(t, &stm);
		if (stm.tm_sec == 5)
		{
			display.ScrollTime(true);
			ScrollDate();
			display.ScrollTime(false);
		}
		else if (stm.tm_sec == 45)
		{
			display.ScrollTime(true);
			display.ScrollLine(stations[station_].Name);
			display.ScrollTime(false);
		}
		else
			display.AnimateTime(0, 0);
#else
		if ((OS::get_tick_count() % 10000) < 1000)
			display.ScrollLine(stations[station_].Name);
		else
			display.StaticText(stations[station_].freqString);
#endif
	}
}

void App::SetBrightness(int br)
{
	Max.SetBrightness(br);

	// save brightness into battery backed RAM
	PWR->CR |= PWR_CR_DBP;
	BKP->DR3 = Max.GetBrightness();
	PWR->CR &= ~PWR_CR_DBP;
}

void App::SelectStation(int st)
{
	if (st >= stationCount)     // check that value in range
		st = 0;
	else if (st < 0)
		st = stationCount - 1;

	station_ = st;

	tuner.setFreq(stations[station_].freq);
	tuner.writeData();

	display.ScrollLine(stations[station_].Name);

	// save station into battery backed RAM
	PWR->CR |= PWR_CR_DBP;
	BKP->DR2 = station_;
	PWR->CR &= ~PWR_CR_DBP;
}

#ifdef USE_RTC
static const char  *const monthsLong[] =
	{"января","февраля","марта","апреля","мая","июня",
	"июля","августа","сентября","октября","ноября","декабря"};

static const char  *const monthsShort[] =
	{"янв","фев","мар","апр","май","июн",
	"июл","авг","сен","окт","ноя","дек"};

static const char  *const dayNames[] =
	{"Воскресенье", "Понедельник","Вторник","Среда","Четверг","Пятница","Суббота"};


void App::ScrollDate()
{
//                   0        1         2         3
//                   12345678901234567890123456789012
// The longest case: Воскресенье, 22 сентября 2014 г.
// 33 chars.
	struct tm stm;
	time_t t = rtc.ReadTime();
	TimeUtil::localtime(t, &stm);

	TextBuffer<40> buf;

	buf
		<< dayNames[stm.tm_wday]
		<< ", "
		<< stm.tm_mday
		<< ' '
		<< monthsLong[stm.tm_mon]
		<< ' '
		<< stm.tm_year + 1900
		<< "г. ";

	display.ScrollLine(buf);
}
bool App::GetMonth(int* val)
{
	int i = *val;

	for (;;)
	{
		display.StaticText(monthsShort[i]);
		int ch = kbd.GetChar(30000); // 30 sec timeout
		switch (ch)
		{
		case -1:
			return false;

		case BUTTON_UP:
		case BUTTON_UP | BUTTON_REPEAT:
			if (++i > 11) i = 0;
			break;

		case BUTTON_DOWN:
		case BUTTON_DOWN | BUTTON_REPEAT:
			if (--i < 0) i = 11;
			break;

		case BUTTON_UP | BUTTON_DOWN:
			*val = i;
			return true;
		}
	}
}

bool App::EditTime()
{
	struct tm stm;
	int i;
	time_t t = rtc.ReadTime();
	TimeUtil::localtime(t, &stm);

	i = stm.tm_hour;
	if (!GetInt("Ч:", 0, 23, &i))
		return false;
	stm.tm_hour = i;

	i = stm.tm_min;
	if (!GetInt("М:", 0, 59, &i))
		return false;
	stm.tm_min = i;

	stm.tm_sec = 0;

	rtc.WriteTime(TimeUtil::mktime(&stm));
	return true;
}

bool App::EditDate()
{
	struct tm stm;
	int i;
	time_t t = rtc.ReadTime();
	TimeUtil::localtime(t, &stm);

	i = stm.tm_mday;
	if (!GetInt("Д:", 0, 31, &i))
		return false;
	stm.tm_mday = i;

	i = stm.tm_mon;
	if (!GetMonth(&i))
		return false;
	stm.tm_mon = i;

	i = stm.tm_year + 1900;
	if (!GetInt("", 2014, 3014, &i, false))
		return false;
	stm.tm_year = i - 1900;

	rtc.WriteTime(TimeUtil::mktime(&stm));
	return true;
}

bool App::EditCorr()
{
	int i = rtc.GetCorrection();
	if (!GetInt("К", 0, 255, &i))
		return false;
	rtc.SetCorrection(i);
	return true;
}

#endif

bool App::GetInt(char const* prompt, int min, int max, int* val, bool wrap)
{
	int i = *val;
	if (i > max) i = max;
	else if (i < min) i = min;

	for (;;)
	{
		TextBuffer<5> buf;
		buf << prompt << i;
		display.StaticText(buf);
		int ch = kbd.GetChar(30000); // 30 sec timeout
		switch (ch)
		{
		case -1:
			return false;

		case BUTTON_UP:
		case BUTTON_UP | BUTTON_REPEAT:
			if (++i > max) i = wrap ? min : max;
			break;

		case BUTTON_DOWN:
		case BUTTON_DOWN | BUTTON_REPEAT:
			if (--i < min) i = wrap ? max : min;
			break;

		case BUTTON_UP | BUTTON_DOWN:
			*val = i;
			return true;
		}
	}
}

bool App::EditBrightness()
{
	for (;;)
	{
		TextBuffer<5> buf;
		buf << "Я:" << Max.GetBrightness();
		display.StaticText(buf);
		int ch = kbd.GetChar(30000); // 30 sec timeout
		switch (ch)
		{
		case -1:
			return false;

		case BUTTON_UP:
			if (Max.GetBrightness() < 15)
				SetBrightness(Max.GetBrightness()+1);
			break;

		case BUTTON_DOWN:
			if (Max.GetBrightness() > 0)
				SetBrightness(Max.GetBrightness()-1);
			break;

		case BUTTON_UP | BUTTON_DOWN:
			return true;
		}
	}
	return true;
}


static const char  *const configMenu[] =
	{
		"Яркость",
#ifdef USE_RTC
		"Время",
		"Дата",
		"Коррекция",
#endif
		"Выход"
	};

static const int configMenuSize = sizeof(configMenu)/sizeof(configMenu[0]);

void App::EditConfig()
{
	int pos = 0;

	for (;;)
	{
		tick_count_t start = OS::get_tick_count();
		for (;;)
		{
			display.ScrollLine(configMenu[pos]);
			if (kbd.Keypressed())
				break;
			if (OS::get_tick_count() - start > 30000)
				return;
		}
		int ch = kbd.GetChar();
		switch (ch)
		{
		case BUTTON_UP:
			if (++pos >= configMenuSize) pos = 0;
			break;

		case BUTTON_DOWN:
			if (--pos < 0) pos = configMenuSize-1;
			break;

		case BUTTON_UP | BUTTON_DOWN:
			switch (pos)
			{
			case 0 :
				EditBrightness();
				break;

#ifdef USE_RTC
			case 1 :
				EditTime();
				break;

			case 2 :
				EditDate();
				break;

			case 3 :
				EditCorr();
				break;

			case 4 :
				return;
#else
			case 1 :
				return;
#endif
			}
		}
	}
}


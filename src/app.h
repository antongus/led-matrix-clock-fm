/**
*  @file app.h
*
*  Created on: 27.04.2014
*  Copyright (c) 2014 by Anton Gusev aka AHTOXA
**/

#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include <scmRTOS.h>

class App
{
public:
	App()
	{}
	OS_PROCESS void UserInterfaceLoop();
	void InitHardware();
private:
	int station_;
	void SelectStation(int st);
	void SetBrightness(int br);
	void ScrollDate();
	bool EditTime();
	bool EditDate();
	bool EditBrightness();
	bool EditCorr();
	void EditConfig();
	bool GetInt(char const* prompt, int min, int max, int* val);
	bool GetMonth(int* val);
};

extern App app;

#endif // APP_H_INCLUDED

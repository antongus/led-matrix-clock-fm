/**
*  @file time.cpp
*  "TIME" and "DATE" variables.
*
*  Created on: 02.06.2010
*  Copyright (c) 2009 by Anton Gusev aka AHTOXA
**/

#include "util.h"
#include "hw.h"

DEFINE_VARIABLE(DATE);

VARIABLE_GET_HANDLER(DATE)
{
	char buf[30];
	u << rtc.get_date(buf);
	return true;
}

VARIABLE_SET_HANDLER(DATE)
{
	rtc.set_date(args);
	return true;
}

DEFINE_VARIABLE(TIME);

VARIABLE_GET_HANDLER(TIME)
{
	char buf[30];
	u << rtc.get_time(buf);
	return true;
}

VARIABLE_SET_HANDLER(TIME)
{
	rtc.set_time(args);
	return true;
}

DEFINE_VARIABLE(CORR);

VARIABLE_GET_HANDLER(CORR)
{
	u << rtc.getCorrection();
	return true;
}

VARIABLE_SET_HANDLER(CORR)
{
	rtc.setCorrection(small_atoi(args));
	return true;
}


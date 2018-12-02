/**
*  @file hw.h
*
*  Project-level hardware definitions.
*
*  Created on: 20.04.2014
*  Copyright (c) 2014 by Anton Gusev aka AHTOXA
**/
#include "hw.h"

#ifdef USE_RTC
RtcModuleLSE rtc;
#endif

KeyboardType kbd;
TMax Max;
FmTuner tuner;

/**
*  @file stations.h
*
*  Created on: 05.04.2014
*  Copyright (c) 2014 by Anton Gusev aka AHTOXA
**/

#ifndef STATIONS_H_
#define STATIONS_H_

#include <stdint.h>

struct Station
{
	uint32_t freq;
	const char * Name;
	const char * freqString;
};

extern const Station stations[];
extern const int stationCount;

#endif /* STATIONS_H_ */

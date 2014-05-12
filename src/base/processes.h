/**
*  @file processes.h
*
*  Declarations for all processes in project
*
*  Created on: 11.04.2011
*  Copyright (c) 2010 by Anton Gusev aka AHTOXA
**/

#ifndef PROCESSES_H_INCLUDED
#define PROCESSES_H_INCLUDED

#include <scmRTOS.h>

typedef OS::process<OS::pr0, 200> TRefreshProcess;
typedef OS::process<OS::pr1, 200> TKbdProcess;
typedef OS::process<OS::pr2, 1200> TUserInterfaceProcess;

extern TRefreshProcess RefreshProcess;
extern TKbdProcess KbdProcess;
extern TUserInterfaceProcess LedProcess;

#endif // PROCESSES_H_INCLUDED

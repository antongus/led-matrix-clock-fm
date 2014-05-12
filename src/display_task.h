/**
*  @file display_task.h
*
*  Created on: 20.04.2014
*  Copyright (c) 2014 by Anton Gusev aka AHTOXA
**/

#ifndef DISPLAY_TASK_H_INCLUDED
#define DISPLAY_TASK_H_INCLUDED

#include <scmRTOS.h>

class DisplayTask
{
public:
	enum TaskKind
	{
		tkScrollText,
		tkStaticText,
		tkGoIdle
	};

	DisplayTask(TaskKind knd, char const* txt)
		: kind(knd)
		, text(txt)
		, flag_()
	{}
	bool Wait(timeout_t timeout = 1000) { return flag_.wait(timeout); 	}
	void Done() { flag_.signal(); 	}
	TaskKind kind;
	char const* text;
private:
	OS::TEventFlag flag_;
};


#endif // DISPLAY_TASK_H_INCLUDED

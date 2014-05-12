#include "hw.h"

INTERPRETER_COMMAND(FM)
{
	char* lt = args;
	char* token = GetToken(0, &lt);
	if (!strcmp(token, "MUTE"))
	{
		token = GetToken(0, &lt);
		if (!strcmp(token, "ON"))
		{
			tuner.setMute(true);
			tuner.writeData();
			return true;
		}
		if (!strcmp(token, "OFF"))
		{
			tuner.setMute(false);
			tuner.writeData();
			return true;
		}
	}
	if (!strcmp(token, "HLSI"))
	{
		token = GetToken(0, &lt);
		if (!strcmp(token, "ON"))
		{
			tuner.setHLSI(true);
			tuner.writeData();
			return true;
		}
		if (!strcmp(token, "OFF"))
		{
			tuner.setHLSI(false);
			tuner.writeData();
			return true;
		}
	}
	if (!strcmp(token, "HCC"))
	{
		token = GetToken(0, &lt);
		if (!strcmp(token, "ON"))
		{
			tuner.setHCC(true);
			tuner.writeData();
			return true;
		}
		if (!strcmp(token, "OFF"))
		{
			tuner.setHCC(false);
			tuner.writeData();
			return true;
		}
	}
	if (!strcmp(token, "DTC"))
	{
		token = GetToken(0, &lt);
		if (!strcmp(token, "ON"))
		{
			tuner.setDTC(true);
			tuner.writeData();
			return true;
		}
		if (!strcmp(token, "OFF"))
		{
			tuner.setDTC(false);
			tuner.writeData();
			return true;
		}
	}
	if (!strcmp(token, "TUNE"))
	{
		token = GetToken(0, &lt);
		uint32_t freq = small_atoi(token);
		uart << "setting FM freq to " << freq;
		tuner.setFreq(freq);
		tuner.writeData();
		return true;
    }
	tuner.readData();
	return true;
}

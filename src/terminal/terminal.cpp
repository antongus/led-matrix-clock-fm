#include "hw.h"
#include "processes.h"
#include "vars2.h"

namespace OS
{
	template <>
	OS_PROCESS void TTerminalProcess::exec()
	{
		char buf[61];
		console_uart_t& u = singleton<console_uart_t>::Instance();
		for (;;)
		{
			u << "\r\n=>";
			u.gets(buf, 60);
			if (!interpreter_t::parse(buf, u))
				u << "ERR";
		}
	}
}

TTerminalProcess TerminalProcess;

INTERPRETER_COMMAND(GET)
{
	char *lt = args;
	char *token = GetToken(0, &lt);

	if (token)
	{
		var_t* v = var_list_t::find(token);
		if (v)
			return v->get(lt, u);
	}
	return false;
}

INTERPRETER_COMMAND(SET)
{
	char *lt = args;
	char *token = GetToken(0, &lt);

	if (token)
	{
		var_t* v = var_list_t::find(token);
		if (v)
			return v->set(lt, u);
	}
	return false;
}

INTERPRETER_COMMAND(VARS)
{
	var_t* v = var_list_t::first();
	while (v)
	{
		uart << v->get_name() << "\r\n";
		v = var_list_t::next(v);
	}
	return true;
}


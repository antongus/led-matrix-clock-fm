#include "hw.h"
#include "processes.h"
#include "display.h"
#include "app.h"

TRefreshProcess RefreshProcess;
TKbdProcess KbdProcess;
TUserInterfaceProcess UserInterfaceProcess;

OS::TEventFlag startEvent;

int main()
{
	OS::run();
}

namespace OS
{
	template<>
	OS_PROCESS void TRefreshProcess::exec()
	{
		OS::sleep(2);

		app.InitHardware();

		startEvent.signal();

		for (;;)
		{
			sleep(1000);
		}

	}

	template <>
	OS_PROCESS void TKbdProcess::exec()
	{
        startEvent.wait();
		for (;;)
		{
			sleep(20);
			kbd.Loop();
		}
    }

	template <>
	OS_PROCESS void TUserInterfaceProcess::exec()
	{
        startEvent.wait();
        app.UserInterfaceLoop();
	}
}


#include <tasks/HttpTask.h>
extern "C"
{
#include "lwip/tcpip.h"
#include "netif/ppp/ppp.h"
}

namespace tasks {

HttpTask::HttpTask() : Thread(config::HttpTask::NAME,
		                      config::HttpTask::STACK_SIZE,
		                      config::HttpTask::PRIORITY),
						_led(hw::config::LED_GREEN),
						_gsm(new net::GsmModule())
{
	Start();
}

void HttpTask::Run()
{
	TickType_t delay = rtos::Ticks::MsToTicks(config::HttpTask::SLEEP_TIME_MS);

	while (not _gsm->turn_on(hw::SimSwitcher::SIM_CHIP));

	int setup = 0;
    const char *username = "";
    const char *password = "";


	tcpip_init(tcpip_init_done, &setup);
	while (!setup) {
		rtos::Thread::Delay(1);
	}

	pppInit();

	pppSetAuth(PPPAUTHTYPE_ANY, username, password);

	while (true)
	{
		volatile int connected = 0;

		int pd = pppOverSerialOpen(0, linkStatusCB, (int*)&connected);

		if (pd >= 0) {
			while (!connected)
				rtos::Thread::Delay(10);
		}

//		// Wait for initial connection
//		int timeout = 0;
//		while (connected < 1) {
//			rtos::Thread::Delay(30);
//			if(timeout++ > 5) {  // If we waited too long restart connection
//				pppClose(pd);
//				continue;
//			}
//		}
//
//		// Make sure connection is stable
//		while (connected < 5) {
//			rtos::Thread::Delay(10);
//			if (connected == 0) { // reset by pppThread while waiting for stable connection
//				pppClose(pd);
//				continue;
//			}
//			connected++;
//		}

		while (connected > 0) {
			_led.toggle();
			rtos::Thread::Delay(delay);
		}
	}
}

void HttpTask::tcpip_init_done(void *arg)
{
	if (arg) {
		*((bool *)arg) = 1;
	}
}

} /* namespace tasks */

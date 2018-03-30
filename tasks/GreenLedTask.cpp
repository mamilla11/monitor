#include "GreenLedTask.h"

namespace tasks {

GreenLedTask::GreenLedTask() : Thread(config::GreenLedTask::NAME,
		                              config::GreenLedTask::STACK_SIZE,
		                              config::GreenLedTask::PRIORITY),
							   _led(hw::config::LED_GREEN)
{
	Start();
}

void GreenLedTask::Run()
{
	TickType_t delay = rtos::Ticks::MsToTicks(config::GreenLedTask::SLEEP_TIME_MS);

	while (true)
	{
		_led.toggle();
		rtos::Thread::Delay(delay);
	}
}

} /* namespace tasks */

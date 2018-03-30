#include "RedLedTask.h"

namespace tasks {

RedLedTask::RedLedTask() : Thread(config::RedLedTask::NAME,
		                          config::RedLedTask::STACK_SIZE,
		                          config::RedLedTask::PRIORITY),
						   _led(hw::config::LED_RED)
{
	Start();
}

void RedLedTask::Run()
{
	TickType_t delay = rtos::Ticks::MsToTicks(config::RedLedTask::SLEEP_TIME_MS);

	while (true)
	{
		_led.toggle();
		rtos::Thread::Delay(delay);
	}
}

} /* namespace tasks */

#include "YellowLedTask.h"

namespace tasks {

YellowLedTask::YellowLedTask() : Thread(config::YellowLedTask::NAME,
		                                config::YellowLedTask::STACK_SIZE,
		                                config::YellowLedTask::PRIORITY),
								 _led(hw::config::LED_YELLOW)
{
	Start();
}

void YellowLedTask::Run()
{
	TickType_t delay = rtos::Ticks::MsToTicks(config::YellowLedTask::SLEEP_TIME_MS);

	while (true)
	{
		_led.toggle();
		rtos::Thread::Delay(delay);
	}
}

} /* namespace tasks */

#include "RedLedTask.h"

namespace tasks {

RedLedTask::RedLedTask() : Thread(config::RedLedTask::NAME,
		                          config::RedLedTask::STACK_SIZE,
		                          config::RedLedTask::PRIORITY),
						   _logger("RED"),
						   _led(hw::config::LED_RED)
{
	//_logger.set_enable_flags(utils::Logger::Flag::DEBG);
	Start();
}

void RedLedTask::Run()
{
	TickType_t delay = rtos::Ticks::MsToTicks(config::RedLedTask::SLEEP_TIME_MS);

	while (true)
	{
		_led.toggle();
		_logger.add_str(utils::Logger::Flag::DEBG, "[DEBG] Toggle");
		rtos::Thread::Delay(delay);
	}
}

} /* namespace tasks */

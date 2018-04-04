#include "GreenLedTask.h"

namespace tasks {

GreenLedTask::GreenLedTask() : Thread(config::GreenLedTask::NAME,
		                              config::GreenLedTask::STACK_SIZE,
		                              config::GreenLedTask::PRIORITY),
							   _logger("GREEN"),
							   _led(hw::config::LED_GREEN)
{
	//_logger.set_enable_flags(utils::Logger::Flag::DEBG);
	Start();
}

void GreenLedTask::Run()
{
	TickType_t delay = rtos::Ticks::MsToTicks(config::GreenLedTask::SLEEP_TIME_MS);

	while (true)
	{
		_led.toggle();
		_logger.add_str(utils::Logger::Flag::DEBG, "[DEBG] Toggle");
		rtos::Thread::Delay(delay);
	}
}

} /* namespace tasks */

#include "YellowLedTask.h"

namespace tasks {

YellowLedTask::YellowLedTask() : Thread(config::YellowLedTask::NAME,
		                                config::YellowLedTask::STACK_SIZE,
		                                config::YellowLedTask::PRIORITY),
								 _logger("YELLOW"),
								 _led(hw::config::LED_YELLOW)
{
	//_logger.set_enable_flags(utils::Logger::Flag::DEBG);
	Start();
}

void YellowLedTask::Run()
{
	TickType_t delay = rtos::Ticks::MsToTicks(config::YellowLedTask::SLEEP_TIME_MS);

	while (true)
	{
		_led.toggle();
		_logger.add_str(utils::Logger::Flag::DEBG, "[DEBG] Toggle");
		rtos::Thread::Delay(delay);
	}
}

} /* namespace tasks */

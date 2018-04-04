#include <tasks/LoggerTask.h>
#include <stdio.h>

namespace tasks {

using Usart = cm3cpp::usart::Usart;

LoggerTask::LoggerTask() : Thread(config::LoggerTask::NAME,
		                          config::LoggerTask::STACK_SIZE,
		                          config::LoggerTask::PRIORITY),
						   _interrupt(this),
						   _usart(hw::config::LOGGER_UART, hw::config::LOGGER_UART_SETTINGS),
						   _selA(hw::config::LOGGER_SEL_A),
						   _selB(hw::config::LOGGER_SEL_B)
{
	_selA.mode_setup(Gpio::Mode::OUTPUT, Gpio::PullMode::NO_PULL);
	_selA.set_output_options(Gpio::OutputType::PUSH_PULL, Gpio::Speed::MEDIUM_25MHz);
	_selA.set();

	_selB.mode_setup(Gpio::Mode::OUTPUT, Gpio::PullMode::NO_PULL);
	_selB.set_output_options(Gpio::OutputType::PUSH_PULL, Gpio::Speed::MEDIUM_25MHz);
	_selB.clear();

	this->Start();
}

void LoggerTask::Run()
{
	TickType_t delay = rtos::Ticks::MsToTicks(config::LoggerTask::SLEEP_TIME_MS);

	while(true)
	{
		_usart.enable_tx_interrupt();
		vTaskDelay(delay);
	}
}

void LoggerInterrupt::call()
{
	uint16_t data;

	if (_owner->_usart.interrupt_source_tx())
	{
		if (not utils::Logger::queue.empty())
		{
			_owner->_usart.write(utils::Logger::queue.front());
			utils::Logger::queue.pop();
		}

		else
		{
			_owner->_usart.disable_tx_interrupt();
		}
	}
}

LoggerInterrupt::LoggerInterrupt(LoggerTask* owner)
{
	_owner = owner;
	this->register_isr(hw::config::LOGGER_ISR, this);
}

} /* namespace tasks */

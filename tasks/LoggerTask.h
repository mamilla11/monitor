#pragma once

#include <thread.hpp>
#include <ticks.hpp>

#include <config/HwConfig.h>
#include <config/TasksConfig.h>
#include <utils/Logger.h>

namespace tasks {

namespace rtos = cpp_freertos;

class LoggerTask;

class LoggerInterrupt : public cm3cpp::IInterruptable
{
public:
	LoggerInterrupt(LoggerTask* owner);
	virtual void call(void);

private:
	LoggerTask* _owner;
};

class LoggerTask final : rtos::Thread
{
public:
	friend class LoggerInterrupt;

	using Gpio    = cm3cpp::gpio::Gpio;
	using Usart   = cm3cpp::usart::Usart;

	LoggerTask();
	~LoggerTask() = default;

	void Run();

private:
	LoggerInterrupt _interrupt;
	Usart           _usart;
	Gpio            _selA;
	Gpio            _selB;

    void _send();
};

} /* namespace tasks */

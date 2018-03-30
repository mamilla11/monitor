#pragma once

#include <thread.hpp>
#include <ticks.hpp>

#include <config/HwConfig.h>
#include <config/TasksConfig.h>
#include <hw/Led.hpp>

namespace tasks {

namespace rtos = cpp_freertos;

class YellowLedTask : public rtos::Thread
{
public:
	YellowLedTask();
	~YellowLedTask() = default;

	void Run();

private:
	hw::Led _led;
};

} /* namespace tasks */

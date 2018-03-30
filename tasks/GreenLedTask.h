#pragma once

#include <thread.hpp>
#include <ticks.hpp>

#include <config/HwConfig.h>
#include <config/TasksConfig.h>
#include <hw/Led.hpp>

namespace tasks {

namespace rtos = cpp_freertos;

class GreenLedTask : public rtos::Thread
{
public:
	GreenLedTask();
	~GreenLedTask() = default;

	void Run();

private:
	hw::Led _led;
};

} /* namespace tasks */

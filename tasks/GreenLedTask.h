#pragma once

#include <thread.hpp>
#include <ticks.hpp>

#include <config/HwConfig.h>
#include <config/TasksConfig.h>
#include <utils/Logger.h>
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
	utils::Logger _logger;
	hw::Led _led;
};

} /* namespace tasks */

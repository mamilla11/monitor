#pragma once

#include <string.h>
#include <thread.hpp>
#include <ticks.hpp>
#include <config/TasksConfig.h>
#include <utils/Logger.h>
#include <lib/libopencm3_cpp_extensions/cm3cpp_timer.h>
#include <config/HwConfig.h>

namespace tasks {

namespace rtos = cpp_freertos;

class MonitorTask final : public rtos::Thread
{
public:
	MonitorTask();
	~MonitorTask() = default;

	void Run();
	static void config_timer();
	static unsigned long get_counter_value();

private:
	utils::Logger              _logger;
	static TaskStatus_t        _tasks_status_array[config::MonitorTask::MAX_TASKS_MONITOR];
	static cm3cpp::tim::Timer* _timer;

	char _task_state_to_char(eTaskState state);
	void _sort_tasks_by_number(TaskStatus_t *status, UBaseType_t count);
};

} /* namespace tasks */

#include <tasks/MonitorTask.h>
#include <libopencm3/include/libopencm3/stm32/iwdg.h>

namespace tasks {

TaskStatus_t MonitorTask::_tasks_status_array[config::MonitorTask::MAX_TASKS_MONITOR];
cm3cpp::tim::Timer* MonitorTask::_timer;

MonitorTask::MonitorTask() : Thread(config::MonitorTask::NAME,
			                        config::MonitorTask::STACK_SIZE,
			                        config::MonitorTask::PRIORITY),
			                 _logger("MONITOR")

{
	_logger.set_enable_flags(utils::Logger::Flag::DEBG);
	_timer = new cm3cpp::tim::Timer(hw::config::MONITOR_TIMER_NUMBER);
	Start();
}


void MonitorTask::config_timer()
{
	_timer->disable_counter();
	_timer->set_counter_direction(cm3cpp::tim::Timer::CounterDirection::UP);
	_timer->set_alignment(cm3cpp::tim::Timer::Alignment::EDGE);
	_timer->set_clock_division(cm3cpp::tim::Timer::ClockDivision::TIMER_CLOCK_MUL_1);
	_timer->set_prescaler_value(hw::config::MONITOR_TIMER_PRESQ);
	_timer->set_autoreload_value(hw::config::MONITOR_AUTORELOAD);
	_timer->enable_counter();
	_timer->set_counter_value(0);
	for (int i = 0; i < 10000; i++);
}

unsigned long MonitorTask::get_counter_value()
{
	static unsigned long _counter = 0;

	_counter += _timer->get_counter_value();
	_timer->set_counter_value(0);
	return (_counter);
}

void MonitorTask::Run()
{
	TickType_t delay_time =
			rtos::Ticks::MsToTicks(config::MonitorTask::SLEEP_TIME_MS);

	while(true)
	{
		UBaseType_t task_count = uxTaskGetNumberOfTasks();

		if (task_count <= config::MonitorTask::MAX_TASKS_MONITOR)
		{
			char cStatus;
			unsigned long _total_runtime;

			TaskStatus_t _buffer[config::MonitorTask::MAX_TASKS_MONITOR];
			task_count = uxTaskGetSystemState(_buffer, task_count, &_total_runtime);
			_sort_tasks_by_number(_buffer, task_count);

			for (int task = 0; task < task_count; task++)
			{
				_logger.add_str(utils::Logger::Flag::DEBG, "[DEBG] %20s: %c, %u, %6u, %u",
						              _tasks_status_array[task].pcTaskName,
						              _task_state_to_char(_tasks_status_array[task].eCurrentState),
									  (unsigned long)_tasks_status_array[task].uxCurrentPriority,
									  (unsigned long)_tasks_status_array[task].usStackHighWaterMark,
									  (unsigned long)(_tasks_status_array[task].ulRunTimeCounter / 1000));
			}

			_logger.add_str(utils::Logger::Flag::DEBG, " ");

			_logger.add_str(utils::Logger::Flag::DEBG, "[DEBG] Current Heap Free Size: %u",
					                                   (unsigned long)xPortGetFreeHeapSize());

			_logger.add_str(utils::Logger::Flag::DEBG, "[DEBG] Minimal Heap Free Size: %u",
					                                   (unsigned long)xPortGetMinimumEverFreeHeapSize());

			_logger.add_str(utils::Logger::Flag::DEBG, "[DEBG] Total RunTime:  %u ms", _total_runtime / 1000);

			_logger.add_str(utils::Logger::Flag::DEBG, "[DEBG] System Uptime:  %u ms\r\n",
					                                   (unsigned long)xTaskGetTickCount() * portTICK_PERIOD_MS);
		}

		rtos::Thread::Delay(delay_time);
	}
}

char MonitorTask::_task_state_to_char(eTaskState state)
{
	switch (state)
	{
		case eReady:		return 'R';
		case eBlocked:		return 'B';
		case eSuspended:	return 'S';
		case eDeleted:		return 'D';
		default:			return 0x00;
	}
}

void MonitorTask::_sort_tasks_by_number(TaskStatus_t *status, UBaseType_t count)
{
	for (int task = 0; task < count; task++) {
		_tasks_status_array[status[task].xTaskNumber - 1] = status[task];
  	}
}

} /* namespace tasks */

#pragma once

#include <cstdint>

#include "FreeRTOSConfig.h"

namespace tasks {

namespace config {

static constexpr uint32_t MIN_TASK_STACK_SIZE 				= configMINIMAL_STACK_SIZE;
static constexpr uint32_t MIN_TASK_PRIORITY   				= 1;
static constexpr uint32_t MAX_TASK_PRIORITY   				= configMAX_PRIORITIES;

struct GreenLedTask {
	static constexpr uint32_t STACK_SIZE 					= MIN_TASK_STACK_SIZE*2;
	static constexpr const char NAME[]   					= "Green Led Task";
	static constexpr uint32_t PRIORITY   					= MIN_TASK_PRIORITY;
	static constexpr uint32_t SLEEP_TIME_MS 			    = 1000;
};

struct RedLedTask {
	static constexpr uint32_t STACK_SIZE 					= MIN_TASK_STACK_SIZE*2;
	static constexpr const char NAME[]   					= "Red Led Task";
	static constexpr uint32_t PRIORITY   					= MIN_TASK_PRIORITY;
	static constexpr uint32_t SLEEP_TIME_MS 			    = 1000;
};

struct YellowLedTask {
	static constexpr uint32_t STACK_SIZE 					= MIN_TASK_STACK_SIZE*2;
	static constexpr const char NAME[]   					= "Yellow Led Task";
	static constexpr uint32_t PRIORITY   					= MIN_TASK_PRIORITY;
	static constexpr uint32_t SLEEP_TIME_MS 			    = 1000;
};

} /* namespace config */

} /* namespace tasks */

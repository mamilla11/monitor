#pragma once

#include <thread.hpp>
#include <ticks.hpp>

#include <config/HwConfig.h>
#include <config/TasksConfig.h>
#include <hw/Led.hpp>
#include <net/GsmModule.h>

namespace tasks {

namespace rtos = cpp_freertos;

class HttpTask : public rtos::Thread
{
public:
	HttpTask();
	~HttpTask() = default;

	void Run();

private:
	hw::Led _led;
	net::GsmModule* _gsm;

	static void tcpip_init_done(void *arg);
};

} /* namespace tasks */

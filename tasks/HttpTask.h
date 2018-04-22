#pragma once

#include <thread.hpp>
#include <ticks.hpp>

#include <config/HwConfig.h>
#include <config/TasksConfig.h>
#include <utils/Logger.h>
#include <hw/Led.hpp>
#include <net/GsmModule.h>

BEGIN_DECLS
#include "lwip/tcpip.h"
#include "netif/ppp/ppp.h"
#include  "tcp.h"
END_DECLS

namespace tasks {

namespace rtos = cpp_freertos;

class HttpTask : public rtos::Thread
{
public:
	HttpTask();
	~HttpTask() = default;

	void Run();

private:
	static utils::Logger   _logger;
	hw::Led         _led;
	net::GsmModule* _gsm;

	static void tcpip_init_done(void *arg);
	static err_t tcp_receive(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
	static void tcp_client_close(struct tcp_pcb *pcb);
	static err_t tcp_connected(void *arg, struct tcp_pcb *pcb, err_t err);
	static err_t tcp_client_sent(void *arg, struct tcp_pcb *pcb, u16_t len);
	static void tcp_error(void *arg, err_t err);

	void tcpip_setup();
};

} /* namespace tasks */

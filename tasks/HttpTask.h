#pragma once

#include <thread.hpp>
#include <ticks.hpp>

#include <config/HwConfig.h>
#include <config/TasksConfig.h>
#include <utils/Logger.h>
#include <hw/Led.hpp>
#include <net/GsmModule.h>

BEGIN_DECLS
#include "netif/ppp/pppos.h"
#include "lwip/tcpip.h"
#include  <lib/lwip/src/include/lwip/tcp.h>
#include <lib/lwip/contrib/ping.h>

void ppp_link_status_cb(ppp_pcb *pcb, int err_code, void *ctx);
u32_t ppp_output_cb(ppp_pcb *pcb, u8_t *data, u32_t len, void *ctx);
u32_t ppp_input_cb(u8_t *data, int len);
END_DECLS

namespace tasks {

namespace rtos = cpp_freertos;

class HttpTask : public rtos::Thread
{
public:
	static utils::Logger logger;
	static bool connected;
	static ppp_pcb *ppp;
	static struct netif ppp_netif;

	HttpTask();
	~HttpTask() = default;

	void Run();

private:
	hw::Led         _led;
	net::GsmModule* _gsm;

	static void ppp_input_thread(void *arg);
	static void tcpip_init_done(void *arg);
	static err_t tcp_receive(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
	static void tcp_client_close(struct tcp_pcb *pcb);
	static err_t tcp_connected(void *arg, struct tcp_pcb *pcb, err_t err);
	static err_t tcp_client_sent(void *arg, struct tcp_pcb *pcb, u16_t len);
	static void tcp_error(void *arg, err_t err);

	void tcpip_setup();
};

} /* namespace tasks */

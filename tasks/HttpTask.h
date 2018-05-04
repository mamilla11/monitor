#pragma once

#include <thread.hpp>
#include <ticks.hpp>

#include <config/HwConfig.h>
#include <config/TasksConfig.h>
#include <utils/Logger.h>
#include <hw/Led.hpp>
#include <net/GsmModule.h>
#include <mbedtls/ssl.h>
#include <mbedtls/ctr_drbg.h>
#include "mbedtls/entropy.h"
#include "mbedtls/debug.h"

BEGIN_DECLS
#include <lwip/contrib/ping.h>
#include "netif/ppp/pppos.h"
#include "lwip/tcpip.h"
#include <lwip/sockets.h>

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
    static constexpr const char _USERNAME[]          = "beeline";
    static constexpr const char _PASSWORD[]          = "beeline";
    static constexpr const char _DRBG_PERS[]         = "mbed TLS helloword client";
    static constexpr const char _HTTPS_SERVER_NAME[] = "developer.mbed.org";
    static constexpr const char _HTTPS_PATH[]        = "/media/uploads/mbed_official/hello.txt";

	hw::Led                  _led;
	net::GsmModule*          _gsm;
	mbedtls_entropy_context  _entropy;
	mbedtls_ctr_drbg_context _ctr_drbg;
	mbedtls_x509_crt         _cacert;
	mbedtls_ssl_context      _ssl;
	mbedtls_ssl_config       _ssl_conf;


	static void my_debug(void *ctx, int level, const char *file, int line, const char *str);
	static void ppp_input_thread(void *arg);
	static int ssl_send(void *ctx, const unsigned char *buf, size_t len);
	static int ssl_recv(void *ctx, unsigned char *buf, size_t len);
	static err_t tcp_receive(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
	static void tcp_client_close(struct tcp_pcb *pcb);
	static err_t tcp_connected(void *arg, struct tcp_pcb *pcb, err_t err);
	static err_t tcp_client_sent(void *arg, struct tcp_pcb *pcb, u16_t len);
	static void tcp_error(void *arg, err_t err);

	void _init_gsm();
	void _init_tcpip();
	void _init_ssl();
	void _open_ppp_connection();
	int  _tcp_socket_setup();

	// CALLBACKS ################################################################################
	static void _tcpip_init_done_cb(void *arg);

};

} /* namespace tasks */

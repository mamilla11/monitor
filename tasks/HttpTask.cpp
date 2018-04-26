#include <tasks/HttpTask.h>

void ppp_link_status_cb(ppp_pcb *pcb, int err_code, void *ctx)
{
	 LWIP_UNUSED_ARG(ctx);
	struct netif *pppif = ppp_netif(pcb);

	switch(err_code)
	{
	case PPPERR_NONE:
	{
#if LWIP_DNS
		const ip_addr_t *ns;
#endif /* LWIP_DNS */
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "status_cb: Connected\n");
#if PPP_IPV4_SUPPORT
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "our_ipaddr = %s\n", ipaddr_ntoa(&pppif->ip_addr));
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "his_ipaddr = %s\n", ipaddr_ntoa(&pppif->gw));
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "netmask    = %s\n", ipaddr_ntoa(&pppif->netmask));
#if LWIP_DNS
		ns = dns_getserver(0);
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "dns1 = %s\n", ipaddr_ntoa(ns));
		ns = dns_getserver(1);
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "dns2 = %s\n", ipaddr_ntoa(ns));
#endif /* LWIP_DNS */
#endif /* PPP_IPV4_SUPPORT */
#if PPP_IPV6_SUPPORT
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "our6_ipaddr = %s\n", ip6addr_ntoa(netif_ip6_addr(pppif, 0)));
#endif /* PPP_IPV6_SUPPORT */
		break;
	}
	case PPPERR_PARAM:
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "status_cb: Invalid parameter\n");
		break;
	case PPPERR_OPEN:
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "status_cb: Unable to open PPP session\n");
		break;
	case PPPERR_DEVICE:
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "status_cb: Invalid I/O device for PPP\n");
		break;
	case PPPERR_ALLOC:
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "status_cb: Unable to allocate resources\n");
		break;
	case PPPERR_USER:
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "status_cb: User interrupt\n");
		break;
	case PPPERR_CONNECT:
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "status_cb: Connection lost\n");
		break;
	case PPPERR_AUTHFAIL:
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "status_cb: Failed authentication challenge\n");
		break;
	case PPPERR_PROTOCOL:
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "status_cb: Failed to meet protocol\n");
		break;
	case PPPERR_PEERDEAD:
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "status_cb: Connection timeout\n");
		break;
	case PPPERR_IDLETIMEOUT:
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "status_cb: Idle Timeout\n");
		break;
	case PPPERR_CONNECTTIME:
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "status_cb: Max connect time reached\n");
		break;
	case PPPERR_LOOPBACK:
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "status_cb: Loopback detected\n");
		break;
	default:
		tasks::HttpTask::logger.add_str(utils::Logger::Flag::DEBG, "status_cb: Unknown error code %d\n", err_code);
		break;
	}

	/*
	 * This should be in the switch case, this is put outside of the switch
	 * case for example readability.
	 */

	if (err_code == PPPERR_NONE) {
		tasks::HttpTask::connected = true;
		return;
	}

	/* ppp_close() was previously called, don't reconnect */
	if (err_code == PPPERR_USER) {
		tasks::HttpTask::connected = false;
		/* ppp_free(); -- can be called here */
		return;
	}

	tasks::HttpTask::connected = false;

	/*
	 * Try to reconnect in 30 seconds, if you need a modem chatscript you have
	 * to do a much better signaling here ;-)
	 */
	//ppp_connect(pcb, 30);
	/* OR ppp_listen(pcb); */
}

u32_t ppp_output_cb(ppp_pcb *pcb, u8_t *data, u32_t len, void *ctx)
{
	//net::base::sim800::Sim800Base::_logger.add_str(utils::Logger::Flag::DEBG, "[OUT]");
	//net::base::sim800::Sim800Base::_logger.add_hex(utils::Logger::Flag::DEBG, data, len);
	net::base::sim800::Sim800Base::send_raw_hex(data, len);
	return len;
}

u32_t ppp_input_cb(u8_t *data, int len)
{
	int i    = 0;
	int byte = 0;

	while (true)
	{
		byte = net::base::sim800::Sim800Base::recv_byte();

		if (byte == -1) {
			break;
		}

		data[i++] = (u8_t)byte;

		if (i == len)
			break;
	}

	//if (i > 0) {

		//net::base::sim800::Sim800Base::_logger.add_str(utils::Logger::Flag::DEBG, "[IN]");
		//net::base::sim800::Sim800Base::_logger.add_hex(utils::Logger::Flag::DEBG, data, i);
	//}

	return i;
}

namespace tasks {

utils::Logger HttpTask::logger("HTTP");
bool HttpTask::connected(false);
ppp_pcb *HttpTask::ppp;
struct netif HttpTask::ppp_netif;

HttpTask::HttpTask() : Thread(config::HttpTask::NAME,
		                      config::HttpTask::STACK_SIZE,
		                      config::HttpTask::PRIORITY),
						_led(hw::config::LED_GREEN),
						_gsm(new net::GsmModule())
{
	logger.set_enable_flags(utils::Logger::Flag::DEBG);
	Start();
}
void HttpTask::Run()
{
	TickType_t delay = rtos::Ticks::MsToTicks(config::HttpTask::SLEEP_TIME_MS);

	while (not _gsm->turn_on(hw::SimSwitcher::SIM_CHIP));

	int setup = 0;
    const char *username = "beeline";
    const char *password = "beeline";

	tcpip_init(tcpip_init_done, &setup);
	while (!setup) { rtos::Thread::Delay(rtos::Ticks::MsToTicks(10)); }

	sys_thread_new(PPP_THREAD_NAME, ppp_input_thread, NULL, PPP_THREAD_STACKSIZE, PPP_THREAD_PRIO);

	ppp = pppos_create(&ppp_netif, ppp_output_cb, ppp_link_status_cb, NULL);
	ppp_set_default(ppp);
	ppp_set_auth(ppp, PPPAUTHTYPE_PAP, username, password);
	ppp_connect(ppp, 0);
	while (not connected)
		rtos::Thread::Delay(10);

	tcpip_setup();

	//ping_init();

	while (true)
	{
		//ping_send_now();
		_led.toggle();
		rtos::Thread::Delay(delay);
	}
}

void HttpTask::ppp_input_thread(void *arg)
{
	int count;
	uint8_t data[1504] = {0};

	while (ppp->phase != PPP_PHASE_DEAD)
	{
		count = ppp_input_cb(data, 1504);
		if(count > 0)
			pppos_input_tcpip(ppp, data, count);
		else
			sys_msleep(100);
	}
}

void HttpTask::tcpip_setup()
{
	ip4_addr_t server_ip;
	IP4_ADDR(&server_ip, 93,92,200,31);
	struct tcp_pcb *testpcb;

	testpcb = tcp_new();
	tcp_err(testpcb, tcp_error);
	tcp_recv(testpcb, tcp_receive);
	tcp_sent(testpcb, tcp_client_sent);
	tcp_connect(testpcb, &server_ip, 80, tcp_connected);
}

void HttpTask::tcpip_init_done(void *arg)
{
	if (arg) {
		*((bool *)arg) = 1;
	}
}

err_t HttpTask::tcp_receive(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	logger.add_str(utils::Logger::Flag::DEBG, "Data Received");

	LWIP_UNUSED_ARG(arg);

	if(err == ERR_OK && (char*)p != NULL)
	{
		char *string = (char*)p->payload;
		string[p->len] = 0;

		logger.add_str(utils::Logger::Flag::DEBG, "Number of pbufs %d\n", pbuf_clen(p));
		logger.add_str(utils::Logger::Flag::DEBG, "Contents of pbuf %s\n", string);

		tcp_recved(pcb, p->tot_len);
		pbuf_free(p);
	}
	else
	{
		logger.add_str(utils::Logger::Flag::DEBG, "The remote host closed the connection.\n");
		logger.add_str(utils::Logger::Flag::DEBG, "Now I'm closing the connection.\n");
		tcp_client_close(pcb);
	}
	return ERR_OK;
}

void HttpTask::tcp_client_close(struct tcp_pcb *pcb)
{
	tcp_arg(pcb, NULL);
	tcp_sent(pcb, NULL);
	tcp_close(pcb);
}

err_t HttpTask::tcp_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
	logger.add_str(utils::Logger::Flag::DEBG, "Connection Established");

	char string[150] = "GET /about/ HTTP/1.1\r\nHost: weather.thirdpin.ru\r\n\r\n";
	LWIP_UNUSED_ARG(arg);
	if(err == ERR_OK)
	{
		err = tcp_write(pcb, string, strlen(string), TCP_WRITE_FLAG_COPY);
		if (err)
		{
			logger.add_str(utils::Logger::Flag::DEBG, "TCP_WRITE ERROR");
			return err;
		}
		err = tcp_output(pcb);
		if (err)
		{
			logger.add_str(utils::Logger::Flag::DEBG, "TCP_OUTPUT ERROR");
			return err;
		}
	}

	logger.add_str(utils::Logger::Flag::DEBG, "TCP_DATA_SENT OK");
	return err;
}

err_t HttpTask::tcp_client_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
	LWIP_UNUSED_ARG(arg);
	logger.add_str(utils::Logger::Flag::DEBG, "TCP_CLIENT_SENT OK");
	return ERR_OK;
}

void HttpTask::tcp_error(void *arg, err_t err)
{
	logger.add_str(utils::Logger::Flag::DEBG, "ERROR %d", err);
}

} /* namespace tasks */

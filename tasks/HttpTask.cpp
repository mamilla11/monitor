#include <tasks/HttpTask.h>

namespace tasks {

utils::Logger HttpTask::_logger("HTTP");

HttpTask::HttpTask() : Thread(config::HttpTask::NAME,
		                      config::HttpTask::STACK_SIZE,
		                      config::HttpTask::PRIORITY),
						_led(hw::config::LED_GREEN),
						_gsm(new net::GsmModule())
{
	_logger.set_enable_flags(utils::Logger::Flag::DEBG);
	Start();
}

void HttpTask::Run()
{
	TickType_t delay = rtos::Ticks::MsToTicks(config::HttpTask::SLEEP_TIME_MS);

	while (not _gsm->turn_on(hw::SimSwitcher::SIM_CHIP));

	int setup = 0;
    const char *username = "";
    const char *password = "";


	tcpip_init(tcpip_init_done, &setup);
	while (!setup) {
		rtos::Thread::Delay(1);
	}

	pppInit();

	pppSetAuth(PPPAUTHTYPE_ANY, username, password);

	while (true)
	{
		volatile int connected = 0;

		int pd = pppOverSerialOpen(0, linkStatusCB, (int*)&connected);

		if (pd >= 0) {
			while (!connected)
				rtos::Thread::Delay(10);
		}

//		// Wait for initial connection
//		int timeout = 0;
//		while (connected < 1) {
//			rtos::Thread::Delay(30);
//			if(timeout++ > 5) {  // If we waited too long restart connection
//				pppClose(pd);
//				continue;
//			}
//		}
//
//		// Make sure connection is stable
//		while (connected < 5) {
//			rtos::Thread::Delay(10);
//			if (connected == 0) { // reset by pppThread while waiting for stable connection
//				pppClose(pd);
//				continue;
//			}
//			connected++;
//		}

		while (connected > 0) {
			tcpip_setup();
			_led.toggle();
			rtos::Thread::Delay(delay);
		}
	}
}

void HttpTask::tcpip_setup()
{
	struct ip_addr server_ip;
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
	_logger.add_str(utils::Logger::Flag::DEBG, "Data Received");

	LWIP_UNUSED_ARG(arg);

	if(err == ERR_OK && (char*)p != NULL)
	{
		char *string = (char*)p->payload;
		string[p->len] = 0;

		_logger.add_str(utils::Logger::Flag::DEBG, "Number of pbufs %d\n", pbuf_clen(p));
		_logger.add_str(utils::Logger::Flag::DEBG, "Contents of pbuf %s\n", string);

		tcp_recved(pcb, p->tot_len);
		pbuf_free(p);
	}
	else
	{
		_logger.add_str(utils::Logger::Flag::DEBG, "The remote host closed the connection.\n");
		_logger.add_str(utils::Logger::Flag::DEBG, "Now I'm closing the connection.\n");
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
	_logger.add_str(utils::Logger::Flag::DEBG, "Connection Established");

	char string[50] = "{\"hop\": 0}";
	LWIP_UNUSED_ARG(arg);
	if(err == ERR_OK)
	{
		tcp_write(pcb, string, strlen(string), 0);
		tcp_output(pcb);
	}
	return err;
}

err_t HttpTask::tcp_client_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
	LWIP_UNUSED_ARG(arg);
	return ERR_OK;
}

void HttpTask::tcp_error(void *arg, err_t err)
{
	_logger.add_str(utils::Logger::Flag::DEBG, "ERROR %d", err);
}

} /* namespace tasks */

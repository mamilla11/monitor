#include <tasks/HttpTask.h>
#include "mbedtls/memory_buffer_alloc.h"

const char SSL_CA_PEM[] =
		/* GlobalSign Root certificate */
		"-----BEGIN CERTIFICATE-----\n"
		"MIIEaTCCA1GgAwIBAgILBAAAAAABRE7wQkcwDQYJKoZIhvcNAQELBQAwVzELMAkG\n"
		"A1UEBhMCQkUxGTAXBgNVBAoTEEdsb2JhbFNpZ24gbnYtc2ExEDAOBgNVBAsTB1Jv\n"
		"b3QgQ0ExGzAZBgNVBAMTEkdsb2JhbFNpZ24gUm9vdCBDQTAeFw0xNDAyMjAxMDAw\n"
		"MDBaFw0yNDAyMjAxMDAwMDBaMGYxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9i\n"
		"YWxTaWduIG52LXNhMTwwOgYDVQQDEzNHbG9iYWxTaWduIE9yZ2FuaXphdGlvbiBW\n"
		"YWxpZGF0aW9uIENBIC0gU0hBMjU2IC0gRzIwggEiMA0GCSqGSIb3DQEBAQUAA4IB\n"
		"DwAwggEKAoIBAQDHDmw/I5N/zHClnSDDDlM/fsBOwphJykfVI+8DNIV0yKMCLkZc\n"
		"C33JiJ1Pi/D4nGyMVTXbv/Kz6vvjVudKRtkTIso21ZvBqOOWQ5PyDLzm+ebomchj\n"
		"SHh/VzZpGhkdWtHUfcKc1H/hgBKueuqI6lfYygoKOhJJomIZeg0k9zfrtHOSewUj\n"
		"mxK1zusp36QUArkBpdSmnENkiN74fv7j9R7l/tyjqORmMdlMJekYuYlZCa7pnRxt\n"
		"Nw9KHjUgKOKv1CGLAcRFrW4rY6uSa2EKTSDtc7p8zv4WtdufgPDWi2zZCHlKT3hl\n"
		"2pK8vjX5s8T5J4BO/5ZS5gIg4Qdz6V0rvbLxAgMBAAGjggElMIIBITAOBgNVHQ8B\n"
		"Af8EBAMCAQYwEgYDVR0TAQH/BAgwBgEB/wIBADAdBgNVHQ4EFgQUlt5h8b0cFilT\n"
		"HMDMfTuDAEDmGnwwRwYDVR0gBEAwPjA8BgRVHSAAMDQwMgYIKwYBBQUHAgEWJmh0\n"
		"dHBzOi8vd3d3Lmdsb2JhbHNpZ24uY29tL3JlcG9zaXRvcnkvMDMGA1UdHwQsMCow\n"
		"KKAmoCSGImh0dHA6Ly9jcmwuZ2xvYmFsc2lnbi5uZXQvcm9vdC5jcmwwPQYIKwYB\n"
		"BQUHAQEEMTAvMC0GCCsGAQUFBzABhiFodHRwOi8vb2NzcC5nbG9iYWxzaWduLmNv\n"
		"bS9yb290cjEwHwYDVR0jBBgwFoAUYHtmGkUNl8qJUC99BM00qP/8/UswDQYJKoZI\n"
		"hvcNAQELBQADggEBAEYq7l69rgFgNzERhnF0tkZJyBAW/i9iIxerH4f4gu3K3w4s\n"
		"32R1juUYcqeMOovJrKV3UPfvnqTgoI8UV6MqX+x+bRDmuo2wCId2Dkyy2VG7EQLy\n"
		"XN0cvfNVlg/UBsD84iOKJHDTu/B5GqdhcIOKrwbFINihY9Bsrk8y1658GEV1BSl3\n"
		"30JAZGSGvip2CTFvHST0mdCF/vIhCPnG9vHQWe3WVjwIKANnuvD58ZAWR65n5ryA\n"
		"SOlCdjSXVWkkDoPWoC209fN5ikkodBpBocLTJIg1MGCUF7ThBCIxPTsvFwayuJ2G\n"
		"K1pp74P1S8SqtCr4fKGxhZSM9AyHDPSsQPhZSZg=\n"
		"-----END CERTIFICATE-----\n";

#if OLD_CERTIFICATE
/* GlobalSign Root R1 SHA1/RSA/2048
 *   Serial no.  04 00 00 00 00 01 15 4b 5a c3 94 */
"-----BEGIN CERTIFICATE-----\n"
"MIIDdTCCAl2gAwIBAgILBAAAAAABFUtaw5QwDQYJKoZIhvcNAQEFBQAwVzELMAkG\n"
"A1UEBhMCQkUxGTAXBgNVBAoTEEdsb2JhbFNpZ24gbnYtc2ExEDAOBgNVBAsTB1Jv\n"
"b3QgQ0ExGzAZBgNVBAMTEkdsb2JhbFNpZ24gUm9vdCBDQTAeFw05ODA5MDExMjAw\n"
"MDBaFw0yODAxMjgxMjAwMDBaMFcxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9i\n"
"YWxTaWduIG52LXNhMRAwDgYDVQQLEwdSb290IENBMRswGQYDVQQDExJHbG9iYWxT\n"
"aWduIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDaDuaZ\n"
"jc6j40+Kfvvxi4Mla+pIH/EqsLmVEQS98GPR4mdmzxzdzxtIK+6NiY6arymAZavp\n"
"xy0Sy6scTHAHoT0KMM0VjU/43dSMUBUc71DuxC73/OlS8pF94G3VNTCOXkNz8kHp\n"
"1Wrjsok6Vjk4bwY8iGlbKk3Fp1S4bInMm/k8yuX9ifUSPJJ4ltbcdG6TRGHRjcdG\n"
"snUOhugZitVtbNV4FpWi6cgKOOvyJBNPc1STE4U6G7weNLWLBYy5d4ux2x8gkasJ\n"
"U26Qzns3dLlwR5EiUWMWea6xrkEmCMgZK9FGqkjWZCrXgzT/LCrBbBlDSgeF59N8\n"
"9iFo7+ryUp9/k5DPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8E\n"
"BTADAQH/MB0GA1UdDgQWBBRge2YaRQ2XyolQL30EzTSo//z9SzANBgkqhkiG9w0B\n"
"AQUFAAOCAQEA1nPnfE920I2/7LqivjTFKDK1fPxsnCwrvQmeU79rXqoRSLblCKOz\n"
"yj1hTdNGCbM+w6DjY1Ub8rrvrTnhQ7k4o+YviiY776BQVvnGCv04zcQLcFGUl5gE\n"
"38NflNUVyRRBnMRddWQVDf9VMOyGj/8N7yy5Y0b2qvzfvGn9LhJIZJrglfCm7ymP\n"
"AbEVtQwdpf5pLGkkeB6zpxxxYu7KyJesF12KwvhHhm4qxFYxldBniYUr+WymXUad\n"
"DKqC5JlR3XC321Y9YeRq4VzW9v493kHMB65jUr9TU/Qr6cf9tveCX4XSQRjbgbME\n"
"HMUfpIBvFSDJ3gyICh3WZlXi/EjJKSZp4A==\n"
"-----END CERTIFICATE-----\n";
#endif

namespace tasks {

utils::Logger            HttpTask::logger("HTTP");
constexpr const char     HttpTask::_USERNAME[];
constexpr const char     HttpTask::_PASSWORD[];
constexpr const char     HttpTask::_DRBG_PERS[];
constexpr const char     HttpTask::_HTTPS_SERVER_NAME[];
constexpr const char     HttpTask::_HTTPS_PATH[];
bool                     HttpTask::connected(false);
ppp_pcb*                 HttpTask::ppp;
struct netif             HttpTask::ppp_netif;

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

	_init_gsm();

	_init_tcpip();

	_init_ssl();

	sys_thread_new(PPP_THREAD_NAME, ppp_input_thread, NULL, PPP_THREAD_STACKSIZE, PPP_THREAD_PRIO);

	_open_ppp_connection();

	int socket = _tcp_socket_setup();

	static uint8_t  memory_tls[40000];
	mbedtls_memory_buffer_alloc_init( memory_tls, sizeof(memory_tls) );

	int ret;

	ret = mbedtls_ctr_drbg_seed(&_ctr_drbg, mbedtls_entropy_func, &_entropy,
	                               (const unsigned char *)_DRBG_PERS, sizeof(_DRBG_PERS));

	if (ret != 0)
		logger.add_str(utils::Logger::Flag::DEBG, "[FAIL] mbedtls_ctr_drbg_seed returned %d", ret);


	ret = mbedtls_x509_crt_parse(&_cacert, (const unsigned char *) SSL_CA_PEM, sizeof(SSL_CA_PEM));

	if (ret != 0)
		logger.add_str(utils::Logger::Flag::DEBG, "[FAIL] mbedtls_x509_crt_parse returned %d", ret);

	ret = mbedtls_ssl_config_defaults(&_ssl_conf, MBEDTLS_SSL_IS_CLIENT,
			                                      MBEDTLS_SSL_TRANSPORT_STREAM,
                                                  MBEDTLS_SSL_PRESET_DEFAULT);
	if (ret != 0 )
		logger.add_str(utils::Logger::Flag::DEBG, "[FAIL] mbedtls_ssl_config_defaults returned %d", ret);


	mbedtls_ssl_conf_ca_chain(&_ssl_conf, &_cacert, NULL);
	mbedtls_ssl_conf_rng(&_ssl_conf, mbedtls_ctr_drbg_random, &_ctr_drbg);
	//mbedtls_ssl_conf_authmode(&_ssl_conf, MBEDTLS_SSL_VERIFY_OPTIONAL);

	mbedtls_ssl_conf_dbg(&_ssl_conf, my_debug, NULL);
	mbedtls_debug_set_threshold(3);

	ret = mbedtls_ssl_setup(&_ssl, &_ssl_conf);

	if (ret != 0)
		logger.add_str(utils::Logger::Flag::DEBG, "[FAIL] mbedtls_ssl_setup returned %d", ret);

	ret = mbedtls_ssl_set_hostname(&_ssl, _HTTPS_SERVER_NAME);

	if (ret != 0 )
		logger.add_str(utils::Logger::Flag::DEBG, "[FAIL] mbedtls_ssl_set_hostname returned %d", ret);

	mbedtls_ssl_set_bio(&_ssl, static_cast<void *>(&socket), ssl_send, ssl_recv, NULL);

	ret = mbedtls_ssl_handshake(&_ssl);

	if (ret < 0)
		logger.add_str(utils::Logger::Flag::DEBG, "[FAIL] mbedtls_ssl_handshake returned %d", ret);

	char string[150] = {0};
	snprintf(string, sizeof(string) - 1, "GET %s HTTP/1.1\nHost: %s\n\n", _HTTPS_PATH, _HTTPS_SERVER_NAME);
//
	ret = mbedtls_ssl_write(&_ssl, (const unsigned char*)string, strlen(string));
//
	if (ret < 0)
		logger.add_str(utils::Logger::Flag::DEBG, "[FAIL] mbedtls_ssl_write returned %d", ret);

	uint8_t buffer[1000];
	ret = mbedtls_ssl_read( &_ssl, buffer, 1000 );

	//ping_init();

	while (true)
	{
		//ping_send_now();
		_led.toggle();
		rtos::Thread::Delay(delay);
	}
}

void HttpTask::my_debug(void *ctx, int level, const char *file, int line,
                     const char *str)
{
    const char *p, *basename;
    (void) ctx;

    /* Extract basename from file */
    for(p = basename = file; *p != '\0'; p++) {
        if(*p == '/' || *p == '\\') {
            basename = p + 1;
        }
    }

    logger.add_str(utils::Logger::Flag::DEBG, "[DEBG] %s:%04d: |%d| %s", basename, line, level, str);
}

void  HttpTask::_init_gsm()
{
	TickType_t delay = rtos::Ticks::MsToTicks(10);
	bool inited = false;

	while (not inited) {
		inited = _gsm->turn_on(hw::SimSwitcher::SIM_CHIP);
		rtos::Thread::Delay(delay);
	}
}

void HttpTask::_init_tcpip()
{
	TickType_t delay = rtos::Ticks::MsToTicks(10);
	int inited = 0;

	tcpip_init(_tcpip_init_done_cb, &inited);

	while (not inited)
		rtos::Thread::Delay(delay);
}

void HttpTask::_init_ssl()
{
	mbedtls_entropy_init(&_entropy);
	mbedtls_ctr_drbg_init(&_ctr_drbg);
	mbedtls_x509_crt_init(&_cacert);
	mbedtls_ssl_init(&_ssl);
	mbedtls_ssl_config_init(&_ssl_conf);
}

void HttpTask::_open_ppp_connection()
{
	TickType_t delay = rtos::Ticks::MsToTicks(10);

	ppp = pppos_create(&ppp_netif, ppp_output_cb, ppp_link_status_cb, NULL);
	ppp_set_default(ppp);
	ppp_set_auth(ppp, PPPAUTHTYPE_PAP, _USERNAME, _PASSWORD);
	ppp_connect(ppp, 0);

	while (not connected)
		rtos::Thread::Delay(delay);
}

int HttpTask::_tcp_socket_setup()
{
	int socket_fd;
	struct sockaddr_in sa,ra;
	char data_buffer[80] = {0};

	socket_fd = socket(PF_INET, SOCK_STREAM, 0);

	if ( socket_fd < 0 )
	{
		logger.add_str(utils::Logger::Flag::DEBG, "socket call failed");
		return 0;
	}

	memset(&ra, 0, sizeof(struct sockaddr_in));
	ra.sin_family = AF_INET;
	ra.sin_addr.s_addr = inet_addr("217.140.101.30");
	ra.sin_port = htons(443);


	if(connect(socket_fd, (struct sockaddr*)&ra, sizeof(struct sockaddr_in)) < 0)
	{
		logger.add_str(utils::Logger::Flag::DEBG, "connect failed \n");
		close(socket_fd);
		return 0;
	}

	return socket_fd;
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

void HttpTask::_tcpip_init_done_cb(void *arg)
{
	if (arg) {
		*((bool *)arg) = 1;
	}
}

int HttpTask::ssl_send(void *ctx, const unsigned char *buf, size_t len)
{
	int send_data = send(*((int*)ctx),(void*)buf,len,0);
	return send_data;
}

int HttpTask::ssl_recv(void *ctx, unsigned char *buf, size_t len)
{
	int recv_data = recv(*((int*)ctx),(void*)buf,len,0);
	return recv_data;
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

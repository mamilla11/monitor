#include "Sim800Base.h"

#include "lwip/sio.h"
#include "lwip/inet.h"
#include "netif/ppp/ppp.h"

static volatile bool stop = false;

extern "C" {
void LWIP_PLATFORM_DIAG_(const char* msg, ...)
{
	va_list arglist;
	va_start(arglist, msg);
	static char temp[1000];
	vsnprintf (temp,1000,msg, arglist);
	//net::base::sim800::Sim800Base::_logger.add_str(1, temp);
	va_end(arglist);
}


u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
	uint32_t i = 0;
	int byte   = 0;

	stop = false;
	int n = 0;

	while ((not stop) && (n < 1))
	{
		byte = net::base::sim800::Sim800Base::recv_byte();
		if (byte == -1)
			n++;
		else
			data[i++] = byte;
	}

	if(i > 0) {

		//net::base::sim800::Sim800Base::_logger.add_str(utils::Logger::Flag::DEBG, "[ IN]");
		//net::base::sim800::Sim800Base::_logger.add_hex(utils::Logger::Flag::DEBG, data, i);
	}

	return i;

//	while (true)
//	{
//		byte = net::base::sim800::Sim800Base::recv_byte();
//
//		if ((byte == -1)) {
//			break;
//		}
//
//		data[i++] = byte;
//
//		if (stop || (i == len))
//		{
//			stop = false;
//			break;
//		}
//	}
//
//	if(i > 0) {
//
//		net::base::sim800::Sim800Base::_logger.add_str(utils::Logger::Flag::DEBG, "[ IN]");
//		net::base::sim800::Sim800Base::_logger.add_hex(utils::Logger::Flag::DEBG, data, i);
//	}
//
//	return i;
}

u32_t sio_write(sio_fd_t fd, u8_t *data, u32_t len)
{
	net::base::sim800::Sim800Base::send_raw_hex(data, len);
	return len;
}

void sio_read_abort(sio_fd_t fd)
{
	stop = true;
	//net::base::sim800::Sim800Base::flush();
}

u32_t sys_jiffies(void)
{
	return xTaskGetTickCount();
}

void linkStatusCB(void * ctx, int errCode, void * arg)
{
	bool *connected = (bool*)ctx;

	struct ppp_addrs * addrs = (ppp_addrs *)arg;

	switch (errCode)
	{
	case PPPERR_NONE: { /* We are connected */
		printf("ip_addr = %s\r\n", inet_ntoa(addrs->our_ipaddr));
		printf("netmask = %s\r\n", inet_ntoa(addrs->netmask));
		printf("dns1 = %s\r\n", inet_ntoa(addrs->dns1));
		printf("dns2 = %s\r\n", inet_ntoa(addrs->dns2));
		*connected = 1;
		break;
	}
	case PPPERR_CONNECT: {
		printf("lost connection\r\n"); /* just wait */
		*connected = 0;
		break;
	}
	default: { /* We have lost connection */
		printf("connection error\r\n"); /* just wait */
		*connected = 0;
		break;
	}
	}
}

} // extern "C"
namespace net {

namespace base {

namespace sim800 {

constexpr char Sim800Base::AT_DELIMITER[];

Sim800Base::String   Sim800Base::received_string;
Sim800Base::Usart*   Sim800Base::_usart;
Sim800Base::Gpio*    Sim800Base::_modem_off;
Sim800Base::Gpio*    Sim800Base::_power;
Sim800Base::Gpio*    Sim800Base::_power_stat;
Sim800Base::Timer    Sim800Base::_timer;
Sim800Interrupt*     Sim800Base::_interrupt;
etlext::queue_atomic<uint8_t, 2060> Sim800Base::_incomming;

//utils::Logger Sim800Base::_logger("SIM800");

Sim800Base::Sim800Base()
{
	static bool _initialized = false;

	if (not _initialized) {
		_init(this);
		_initialized = true;
	}
}

#ifdef HW_REV4
void Sim800Base::set_power_hw(bool on)
{
	if (on) {
		_modem_off->clear();
		_usart->init(hw::config::GSM_UART, hw::config::GSM_SETTINGS);
		delay(HOLD_POWER_BUTTON_3S);
	}
	else {
		_usart->deinit();
		_modem_off->set();
		delay(HOLD_POWER_BUTTON_3S);
	}
}
#endif

bool Sim800Base::set_power(bool on)
{
	if (not on)
	{
		if (not power_stat()) {
			//LOG_PRINT(INFO, "[INFO] Power already off");
			return true;
		}
		else
		{
			//LOG_PRINT(INFO, "[INFO] Power off start...");
			_power->set();
			delay(HOLD_POWER_BUTTON_3S);
			_power->clear();
			delay(HOLD_POWER_BUTTON_200MS);
			if (power_stat())
			{
				//LOG_PRINT(FAIL, "[FAIL] Power off (STAT)");
				return false;
			}
			recv_string(TIMEOUT_10S, "NORMAL POWER DOWN");
			//LOG_PRINT(INFO, "[INFO] Power off done");
			return true;
		}
	}
	else
	{
		if(not power_stat())
		{
			//LOG_PRINT(INFO, "[INFO] Power on start...");
			_power->set();
			delay(HOLD_POWER_BUTTON_3S);
			_power->clear();
			delay(HOLD_POWER_BUTTON_3S);
			if (not power_stat())
			{
				//LOG_PRINT(FAIL, "[FAIL] Power on (STAT)");
				return false;
			}

			if (sim_ready())
			{
				//LOG_PRINT(INFO, "[INFO] Power on done...");
				return true;
			}

			//LOG_PRINT(FAIL, "[FAIL] Power on (SIM)");
			return false;
		}
		else
		{
			//LOG_PRINT(INFO, "[INFO] Power already on");
			_power->clear();
			return true;
		}
	}
}

bool Sim800Base::power_stat()
{
	if (_power_stat->get() > 0)
		return true;
	return false;
}

bool Sim800Base::is_alive()
{
	send_raw("AT\r\n");

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;

	return true;
}

bool Sim800Base::sim_ready()
{
	for (int i = 0; i < 10; i++)
	{
		delay(TIMEOUT_1S);
		send_raw("AT+CPIN?\r\n");

		if (recv_string(TIMEOUT_5S, "+CPIN: READY") != CommandStatus::SUCCESS)
			continue;

		if (recv_status(TIMEOUT_5S) != CommandStatus::SUCCESS)
			return false;
		else
			return true;
	}

	return false;
}

bool Sim800Base::disable_echo()
{
	send_raw("ATE0\r\n");

	if (recv_status(TIMEOUT_10S) != CommandStatus::SUCCESS)
		return false;

	return true;
}

auto Sim800Base::recv_status(uint32_t timeout) -> CommandStatus
{
	uint8_t result;

	if (wait_string(timeout, &result, 2, "OK", "ERROR"))
	{
		if (result == 1) { return SUCCESS; }
		if (result == 2) { return ERROR;   }
	}

	return TIMEOUT;
}

auto Sim800Base::recv_string(uint32_t timeout, const char *arg) -> CommandStatus
{
	uint8_t result;

	while (wait_string(timeout, &result, 2, arg, "ERROR"))
	{
		if ((result == 1) && (received_string.length() > strlen(AT_DELIMITER)))
		{
			received_string.pop_back();
			received_string.pop_back();
			received_string.add('\0');

			return SUCCESS;
		}

		if (result == 2) { return ERROR;   }
	}

	return TIMEOUT;
}

void Sim800Base::delay(uint32_t delay_ms)
{
	vTaskDelay(rtos::Ticks::MsToTicks(delay_ms));
}

void Sim800Base::send_byte(uint8_t byte)
{
	_usart->write_blocking(byte);
	//_logger.add_buf(utils::Logger::Flag::DEBG, (const char *)&byte, 1);
}

void Sim800Base::send_raw(const char *buffer)
{
	send_raw(buffer, strlen(buffer));
}

void Sim800Base::send_raw(const char *buffer, uint32_t length)
{
	for (uint32_t symb = 0; symb < length; symb++)
		_usart->write_blocking(buffer[symb]);

	if (length > strlen(AT_DELIMITER)) {
		//_logger.add_buf(utils::Logger::Flag::DEBG, buffer, strlen(buffer));
	}
}

void Sim800Base::send_raw_hex(const uint8_t *buffer, uint32_t length)
{
	for (uint32_t symb = 0; symb < length; symb++)
		_usart->write_blocking(buffer[symb]);

	//_logger.add_str(utils::Logger::Flag::DEBG, "[ OUT]");
	//_logger.add_hex(utils::Logger::Flag::DEBG, (uint8_t*)buffer, length);
}

bool Sim800Base::search(const char *needle)
{
	static SmallString _needle;
	static SmallString _any = "ANY";

	char *result;

	_needle.clear();
	_needle.add_cstring(needle);

	result = std::search(_any.begin(), _any.end(), _needle.begin(), _needle.end());
	if (result != _any.end())
		return true;

	result = std::search(received_string.begin(), received_string.end(), _needle.begin(), _needle.end());
	if (result != received_string.end())
		return true;

	return false;
}

bool Sim800Base::search(const char *haystack, const char *needle)
{
	static SmallString _haystack;
	static SmallString _needle;

	char *result;

	_haystack.clear();
	_needle.clear();

	_haystack.add_cstring(haystack);
	_needle.add_cstring(needle);

	result = std::search(_haystack.begin(), _haystack.end(), _needle.begin(), _needle.end());
	if (result != _haystack.end())
		return true;

	return false;
}

int Sim800Base::recv_byte(uint32_t timeout)
{
	int symb = -1;

	_timer.set_period_ms(timeout);
	_timer.start();

	while (not _timer.timeout())
	{
		if (_incomming.empty())
			continue;

		symb = _incomming.front();
		_incomming.pop();
		_timer.stop();
		break;
	}

	//_logger.add_str(utils::Logger::Flag::DEBG, "%d", symb);
	return symb;

}

int Sim800Base::recv_byte()
{
	int symb = -1;

	if (_incomming.empty())
		return symb;

	symb = _incomming.front();
	_incomming.pop();

	return symb;
}

bool Sim800Base::wait_string(uint32_t timeout, uint8_t *result, uint8_t param_count, ...)
{
	if(result == nullptr)
		return false;

	if(param_count == 0)
		return false;

	*result = 0;

	va_list tag;
	va_start(tag, param_count);
	char *arg[param_count];
	for(uint8_t i = 0; i < param_count; i++)
		arg[i] = va_arg(tag, char *);
	va_end(tag);

	uint8_t symb;
	bool wait_delimiter = false;

	received_string.clear();

	_timer.set_period_ms(timeout);
	_timer.start();

	while (not _timer.timeout())
	{
		if (_incomming.empty())
			continue;

		symb = _incomming.front();
		_incomming.pop();

		received_string.add(symb);

		if ((symb == 0x0D) && (not wait_delimiter)) {
			wait_delimiter = true;
			continue;
		}

		if ((symb == 0x0A) && wait_delimiter)
		{
			wait_delimiter = false;

			if (received_string.length() > strlen(AT_DELIMITER)) {
				//_logger.add_str(utils::Logger::Flag::DEBG, received_string.data());
			}

			for(uint8_t	mx = 0 ; mx < param_count; mx++)
			{
				if (search(arg[mx]))
				{
					*result = mx + 1;
					_timer.stop();
					return true;
				}
			}

			received_string.clear();
			continue;
		}

		wait_delimiter = false;
	}

	return false;
}

void Sim800Base::flush()
{
	received_string.clear();
	_incomming.clear();
}

void Sim800Base::_init(Sim800Base* ptr)
{
	_usart = new Usart(hw::config::GSM_UART, hw::config::GSM_SETTINGS);
	_usart->enable_rx_interrupt();

#ifdef HW_REV4
	_modem_off = new Gpio(hw::config::GSM_PWR_CTRL);
	_modem_off->mode_setup(Gpio::Mode::OUTPUT, Gpio::PullMode::NO_PULL);
	_modem_off->set_output_options(Gpio::OutputType::OPEN_DRAIN, Gpio::Speed::MEDIUM_25MHz);
#endif

	_power = new Gpio(hw::config::GSM_PWR);
	_power->mode_setup(Gpio::Mode::OUTPUT, Gpio::PullMode::NO_PULL);
	_power->set_output_options(Gpio::OutputType::PUSH_PULL, Gpio::Speed::MEDIUM_25MHz);

	_power_stat = new Gpio(hw::config::GSM_STAT);
	_power_stat->mode_setup(Gpio::Mode::INPUT, Gpio::PullMode::PULL_DOWN);

	_interrupt = new Sim800Interrupt(ptr);

	//_logger.set_enable_flags(utils::Logger::DEBG);
}

// Sim800Interrupt ########################################################################################
Sim800Interrupt::Sim800Interrupt(Sim800Base* owner)
{
    _owner = owner;
    this->register_isr(hw::config::GSM_MODULE_ISR, this);
}

void Sim800Interrupt::call()
{
	uint8_t data;

	if (_owner->_usart->interrupt_source_rx())
	{
		data = _owner->_usart->read();;
		_owner->_incomming.push(data);
	}
}
//#########################################################################################################

} /* namespace sim800 */

} /* namespace base */

} /* namespace net */

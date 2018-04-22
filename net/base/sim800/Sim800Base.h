#pragma once

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include <etlext/queue_atomic.hpp>
#include <config/HwConfig.h>
#include <config/UtilsConfig.h>
#include <lang/StringFixed.hpp>
#include <timers/OneShotTimer.hpp>
#include <utils/Logger.h>

extern "C" {
void linkStatusCB(void * ctx, int errCode, void * arg);
}

namespace net {

namespace base {

namespace sim800 {

// Sim800Interrupt ################################################################################
class Sim800Base;

class Sim800Interrupt : public cm3cpp::IInterruptable
{
public:
	Sim800Interrupt(Sim800Base* owner);
    virtual void call(void);

private:
    Sim800Base* _owner;
};
//#################################################################################################

namespace rtos = cpp_freertos;

class Sim800Base
{
public:
	friend class Sim800Interrupt;

	// USINGS #####################################################################################
	using String      = lang::StringFixed<hw::config::FTP_BUFFER_SIZE>;
	using SmallString = lang::StringFixed<hw::config::MAX_STRING_LEN>;
	using Timer       = timers::OneShotTimer;
	using Usart       = cm3cpp::usart::Usart;
	using Gpio        = cm3cpp::gpio::Gpio;
	//#############################################################################################

	// STATIC CONSTANTS ###########################################################################
	static constexpr const char AT_DELIMITER[]               = "\r\n";
    static constexpr uint16_t HOLD_POWER_BUTTON_200MS  	     = 200;
    static constexpr uint16_t HOLD_POWER_BUTTON_2S     	     = 2000;
    static constexpr uint16_t HOLD_POWER_BUTTON_3S     	     = 3000;
    static constexpr uint16_t HOLD_POWER_BUTTON_5S     	     = 5000;
	static constexpr uint32_t TIMEOUT_60S              	     = 60000;
	static constexpr uint32_t TIMEOUT_30S              	     = 30000;
	static constexpr uint32_t TIMEOUT_10S               	 = 10000;
	static constexpr uint32_t TIMEOUT_5S               	     = 5000;
	static constexpr uint32_t TIMEOUT_1S               	     = 1000;
	static constexpr uint32_t TIMEOUT_3S               	     = 3000;
	static constexpr uint8_t MAX_AT_LENGTH                   = 100;
	static constexpr uint8_t DEQUEUE_TIMEOUT_TICKS           = 20;
	//#############################################################################################

	// ENUMS ######################################################################################
	enum CommandStatus
	{
		SUCCESS,
		ERROR,
		TIMEOUT,
	};


    //static utils::Logger _logger;
    //#############################################################################################

	static String received_string;

	Sim800Base();
	~Sim800Base() = default;

#ifdef HW_REV4
    /**
     * @brief Turns on / off GSM modem (hardware)
     *
     * @param on true for on, false for off
     * @retval true if successfully powered on / off, false otherwise
     */
	void set_power_hw(bool on);
#endif
    /**
     * @brief Turns on / off GSM modem (software)
     *
     * @param on true for on, false for off
     * @retval true if successfully powered on / off, false otherwise
     */
    bool set_power(bool on);
    /**
     * @brief Checks that GSM modem is powered on
     *
     * @retval true if powered on, false otherwise
     */
    bool power_stat();
    /**
     * @brief Checks that SIM card is present and ready to use
     *
     * @retval true if SIM is ready, false otherwise
     */
	bool sim_ready();
    /**
     * @brief Checks that GSM modem is alive
     *
     * @retval true if alive, false otherwise
     */
	bool is_alive();
    /**
     * @brief Disables echo
     *
     * @retval true if command executed, false otherwise
     */
	bool disable_echo();
    /**
     * @brief Clears incomming queue and received string
     */
	static void flush();
    /**
     * @brief Sends data to modem
     *
     * @param buffer data to send
     * @param length of data
     */
	void send_raw(const char *buffer, uint32_t length);
	static void send_raw_hex(const uint8_t *buffer, uint32_t length);
    /**
     * @brief Returns first byte from _incoming_queue if any
     *
     * @param timeout How long should it waits for a byte
     * @retval received byte, -1 otherwise
     */
	static int recv_byte(uint32_t timeout);
	static int recv_byte();

protected:
    /**
     * @brief Waits for specified string
     *
     * @param timeout in milliseconds
     * @param arg string to looking for
     * @retval SUCCESS, ERROR or TIMEOUT
     */
	CommandStatus recv_string(uint32_t timeout, const char *arg);
    /**
     * @brief Waits for status
     *
     * @param timeout in milliseconds
     * @retval SUCCESS, ERROR or TIMEOUT
     */
	CommandStatus recv_status(uint32_t timeout);
    /**
     * @brief Sends byte to modem
     *
     * @param byte to send
     */
	void send_byte(uint8_t byte);
    /**
     * @brief Sends data to modem
     *
     * @param buffer data to send
     */
	void send_raw(const char *buffer);
    /**
     * @brief Delays task for specified amount of time
     *
     * @param delay in milliseconds
     */
	void delay(uint32_t delay_ms);
    /**
     * @brief Searches for specified string in received string
     *
     * @param needle string to look for
     * @retval true if success, false otherwise
     */
	bool search(const char *needle);
    /**
     * @brief Searches for needle string in haystack string
     *
     * @param haystack where to look
     * @param needle what to look for
     * @retval true if success, false otherwise
     */
	bool search(const char *haystack, const char *needle);
    /**
     * @brief Waits for any of the specified strings
     *
     * @param timeout in milliseconds
     * @param result number of received string
     * @param param_count number of strings
     * @param ... strings to looking for
     * @retval true if success, false otherwise
     */
	bool wait_string(uint32_t timeout, uint8_t *result, uint8_t param_count, ...);

private:
	// PRIVATE FIELDS #############################################################################
	static Usart*           _usart;
	static Gpio*            _modem_off;
	static Gpio*            _power;
	static Gpio*            _power_stat;
	static Timer            _timer;
	static Sim800Interrupt* _interrupt;
	static etlext::queue_atomic<uint8_t, 2060> _incomming;
	//#############################################################################################

    /**
     * @brief Initializes periphery
     */
	static void _init(Sim800Base* ptr);
};

} /* namespace sim800 */

} /* namespace base */

} /* namespace net */

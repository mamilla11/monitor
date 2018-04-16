#pragma once

#include <cstdarg>

#include <etlext/queue_atomic.hpp>

#include <thread.hpp>
#include <semaphore.hpp>

#include <config/HwConfig.h>

namespace utils {

class Logger
{
public:
	static constexpr uint32_t ALL_FLAGS_OFF             = 0;
	static constexpr uint32_t ALL_FLAGS_ON              = 0xFFFFFFFF;
	static constexpr uint32_t MAX_STRING_LENGHT         = hw::config::LOGGER_MESSAGE_LENGTH + hw::config::LOGGER_HEADER_LENGTH;
	static constexpr uint32_t QUEUE_LENGTH              = MAX_STRING_LENGHT * hw::config::LOGGER_MESSAGE_COUNT;

	enum Flag
	{
		DEBG = 1,
		INFO = 2,
		FAIL = 4,
	};

	Logger(const char* class_name, bool from_isr = false);
	~Logger() = default;

	static etlext::queue_atomic<uint8_t, QUEUE_LENGTH> queue;

	void set_enable_flags(uint32_t flags);
	void set_disable_flags(uint32_t flags);
	void set_flags(uint32_t flags);
	bool is_flag_enable(uint32_t flags);

	void add_str(uint32_t flag, const char *format, ...);
	void add_hex(uint32_t flag, uint8_t *buff, uint8_t len);
	void add_buf(uint32_t flag, const char *buff, uint16_t len);

private:
	static bool                                        _inited;
	static cpp_freertos::BinarySemaphore*              _semaphore;

	uint32_t _missed_cnt = 0;
	uint32_t _enable_flags;
	char     _buffer_to_send[MAX_STRING_LENGHT];
	bool     _from_isr;

	bool     _to_queue(void *item);

};

} /* namespace utils */

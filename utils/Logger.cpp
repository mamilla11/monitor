#include "Logger.h"
#include <iostream>
#include <cstdarg>
#include <stdio.h>
#include <cstring>

namespace utils {

bool                                                Logger::_inited = false;
etlext::queue_atomic<uint8_t, Logger::QUEUE_LENGTH> Logger::queue;
cpp_freertos::BinarySemaphore*                      Logger::_semaphore;

Logger::Logger(const char* class_name, bool from_isr)
{
	if (!_inited) {
		_inited    = true;
		_semaphore = new cpp_freertos::BinarySemaphore();
		_semaphore->Give();
	}

	_enable_flags = ALL_FLAGS_OFF;
	snprintf(_buffer_to_send, hw::config::LOGGER_HEADER_LENGTH, "\r\n%s:", class_name);
	uint32_t header_len = strlen(_buffer_to_send);

	for (int i = header_len; i < hw::config::LOGGER_HEADER_LENGTH; i++)
		_buffer_to_send[i] = ' ';

	_from_isr = from_isr;
}

void Logger::set_enable_flags(uint32_t flags)
{
	_enable_flags |= flags;
}

void Logger::set_disable_flags(uint32_t flags)
{
	_enable_flags &= flags^ALL_FLAGS_ON;
}

void Logger::set_flags(uint32_t flags)
{
	_enable_flags = flags;
}

bool Logger::is_flag_enable(uint32_t flags)
{
	return (_enable_flags & flags);
}

void Logger::add_str(uint32_t flag, const char * format, ...)
{
	if (not is_flag_enable(flag))
		return;

	va_list arglist;
	va_start(arglist, format);

    char buffer[hw::config::LOGGER_MESSAGE_LENGTH];
    vsnprintf(buffer, hw::config::LOGGER_MESSAGE_LENGTH, format, arglist);
    uint32_t len = strlen(buffer);
    add_buf(flag, buffer, len);

	va_end(arglist);
}

void Logger::add_hex(uint32_t flag, uint8_t *buff, uint8_t len)
{
	if (not is_flag_enable(flag))
		return;

	if(!len)
		return;

	char buffer[765] = {0};
	uint32_t index = 0;

	for (uint8_t i = 0; i < len; i++)
	{
		snprintf(&buffer[index], 4, "%02X ", buff[i]);
		index += 3;
	}

	add_buf(flag, buffer, strlen(buffer));
}

void Logger::add_buf(uint32_t flag, const char *buff, uint16_t len)
{
	if(not is_flag_enable(flag))
		return;

	if(!len)
		return;

	while (len != 0)
	{
		if (len < hw::config::LOGGER_MESSAGE_LENGTH)
		{
			snprintf(&_buffer_to_send[hw::config::LOGGER_HEADER_LENGTH], len + 1, buff);
			len -= len;
			_to_queue(_buffer_to_send);
		}

		else
		{
			snprintf(&_buffer_to_send[hw::config::LOGGER_HEADER_LENGTH], hw::config::LOGGER_MESSAGE_LENGTH, buff);
			len  -= (hw::config::LOGGER_MESSAGE_LENGTH - 1);
			buff += (hw::config::LOGGER_MESSAGE_LENGTH - 1);
			_to_queue(_buffer_to_send);
		}
	}
}

bool Logger::_to_queue(void *item)
{
	BaseType_t higher_priority_task_woken;

	if(_from_isr)
		_semaphore->TakeFromISR(&higher_priority_task_woken);
	else
		_semaphore->Take();

	if (queue.full()) {
		_missed_cnt++;
		return false;
	}

	for (uint32_t i = 0; i < strlen(_buffer_to_send); i++)
		queue.push(_buffer_to_send[i]);
	queue.push('\0');

	if (_from_isr)
		_semaphore->GiveFromISR(&higher_priority_task_woken);
	else
		_semaphore->Give();

	return true;
}

}

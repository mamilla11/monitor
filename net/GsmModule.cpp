#include "GsmModule.h"

#include <stdlib.h>
#include <string.h>

namespace net {

#ifdef GSM_LOGGING
utils::Logger* GsmModule::_logger;
#endif

GsmModule::GsmModule()
{

}

bool GsmModule::turn_on(hw::SimSwitcher::Sim sim)
{
	_sim = sim;

#ifdef HW_REV4
	set_power_hw(false);
#else
	if (not set_power(false)) {
		LOG_PRINT(FAILURES, "Turn off modem FAIL\r\n");
		return false;
	}
#endif

	_sim_switcher.select_sim(sim);

#ifdef HW_REV4
	set_power_hw(true);
#endif

	if (not set_power(true)) {
		_logger.add_str(utils::Logger::Flag::DEBG, "Turn on modem FAIL");
		return false;
	}

	_logger.add_str(utils::Logger::Flag::DEBG, "Turn on modem OK");


	if (not disable_echo()) {
		_logger.add_str(utils::Logger::Flag::DEBG, "Disable echo FAIL: %s", received_string.data());
		return false;
	}

	_logger.add_str(utils::Logger::Flag::DEBG, "Disable echo OK");

	if (not wait_net_registration()) {
		_logger.add_str(utils::Logger::Flag::DEBG, "Net registration FAIL: %s", received_string.data());
		return false;
	}

	_logger.add_str(utils::Logger::Flag::DEBG, "Net registration OK");

	if (not enter_data_state("PPP", 1)) {
		_logger.add_str(utils::Logger::Flag::DEBG, "Enter data state FAIL: %s", received_string.data());
		return false;
	}

	_logger.add_str(utils::Logger::Flag::DEBG, "Enter data state OK");

	return true;
}

bool GsmModule::turn_off()
{
	return set_power(false);
}

void GsmModule::_delay(uint32_t delay_ticks)
{
	vTaskDelay(delay_ticks);
}


} /* namespace net */

#pragma once

#include <ctime>

#include <lang/StringFixed.hpp>
#include <lang/StringView.h>
#include "hw/SimSwitcher.hpp"

#include "base/sim800/Gsm.h"

namespace net {

class GsmModule final : public base::sim800::Gsm
{
public:
	/**********************************************************************************************
	 * USINGS     *********************************************************************************
	 **********************************************************************************************/
	using String = lang::StringFixed<hw::config::MAX_INFO_STRING_LEN>;
	/**********************************************************************************************/

	enum class Status
	{
		OK         = 0,
		FAIL       = -1000,
		SIM_ERROR  = -1,
		GPRS_ERROR = -2,
	};

	enum class Info : uint8_t {
    	MODEM,
		SIM,
		SIMCHIP,
		GSM,
		LOCATION,
	};

	GsmModule();
	~GsmModule() = default;

	bool turn_on(hw::SimSwitcher::Sim sim);
	bool turn_off();

private:
    static constexpr uint8_t _WAIT_GPRS_RETRIES         = 2;
    static constexpr uint8_t _WAIT_GPRS_DELAY           = 200;

	hw::SimSwitcher       _sim_switcher;
	hw::SimSwitcher::Sim  _sim = hw::SimSwitcher::Sim::SIM_CHIP;

	void _delay(uint32_t delay_ticks);
};

} /* namespace net */

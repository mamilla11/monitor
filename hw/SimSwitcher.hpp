#ifndef HW_SIM_SWITCHER_HPP_
#define HW_SIM_SWITCHER_HPP_

#include <config/HwConfig.h>

namespace hw {

class SimSwitcher
{
public:
	using Gpio  = cm3cpp::gpio::Gpio;

	enum Sim : uint8_t {
		SIM_CARD,
		SIM_CHIP
	};

	SimSwitcher() : _sim_1_en(config::SIM_1_EN),
		            _sim_2_en(config::SIM_2_EN),
		            _sim_sel(config::SIM_CSEL)
	{
		_sim_1_en.mode_setup(Gpio::Mode::OUTPUT, Gpio::PullMode::NO_PULL);
		_sim_1_en.set_output_options(Gpio::OutputType::PUSH_PULL, Gpio::Speed::MEDIUM_25MHz);
		_sim_1_en.clear();

		_sim_2_en.mode_setup(Gpio::Mode::OUTPUT, Gpio::PullMode::NO_PULL);
		_sim_2_en.set_output_options(Gpio::OutputType::PUSH_PULL, Gpio::Speed::MEDIUM_25MHz);
		_sim_2_en.clear();

		_sim_sel.mode_setup(Gpio::Mode::OUTPUT, Gpio::PullMode::NO_PULL);
		_sim_sel.set_output_options(Gpio::OutputType::PUSH_PULL, Gpio::Speed::MEDIUM_25MHz);
	}

	void select_sim(Sim sim)
	{
		enable_sim(sim);

		switch (sim)
		{
		case SIM_CARD: _sim_sel.clear(); break;
		case SIM_CHIP: _sim_sel.set(); break;
		}
	}

	void enable_sim(Sim sim)
	{
		switch (sim)
		{
		case SIM_CARD:
			_sim_2_en.clear();
			_sim_1_en.set();
			break;
		case SIM_CHIP:
			_sim_1_en.clear();
			_sim_2_en.set();
			break;
		}
	}

	void disable_sim(Sim sim)
	{
		switch (sim)
		{
		case SIM_CARD: _sim_1_en.clear(); break;
		case SIM_CHIP: _sim_2_en.clear();   break;
		}
	}

private:
	Gpio _sim_1_en;
	Gpio _sim_2_en;
	Gpio _sim_sel;
};

} /* namespace hw */

#endif /* HW_SIM_SWITCHER_HPP_ */

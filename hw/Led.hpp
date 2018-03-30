#pragma once

#include <libopencm3_cpp_extensions/cm3cpp_gpio.h>

namespace hw {

class Led {

public:
	using Gpio = cm3cpp::gpio::Gpio;

	Led(Gpio::Pinout p) : _pinout(p)
	{
	    _pinout.mode_setup(Gpio::Mode::OUTPUT, Gpio::PullMode::NO_PULL);
	    off();
	}

	~Led() = default;

	void on() {
		_pinout.clear();
	}

	void off() {
		_pinout.set();
	}

	void toggle() {
	    _pinout.toggle();
	}

private:
	Gpio _pinout;
};

} /* namespace hw */

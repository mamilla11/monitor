#pragma once

#include <libopencm3_cpp_extensions/cm3cpp_gpio.h>

namespace hw {

namespace config {

using Gpio         = cm3cpp::gpio::Gpio;
	/**********************************************************************************************
	 * LEDS     ***********************************************************************************
	 **********************************************************************************************/
	static constexpr Gpio::Pinout LED_RED 	            		    = PE4;
	static constexpr Gpio::Pinout LED_GREEN 	            		= PD7;
	static constexpr Gpio::Pinout LED_YELLOW 	            	    = PE5;

} /* namespace config */

} /* namespace hw */

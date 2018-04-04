#pragma once

#include <libopencm3_cpp_extensions/cm3cpp_gpio.h>
#include <libopencm3_cpp_extensions/cm3cpp_usart.h>
#include <libopencm3_cpp_extensions/irq/cm3cpp_irq.h>

namespace hw {

namespace config {

using Gpio      = cm3cpp::gpio::Gpio;
using Usart     = cm3cpp::usart::Usart;
using Interrupt = cm3cpp::Interrupt;

static constexpr uint32_t              APB1_FREQ                = 30000000;
static constexpr uint32_t              APB2_FREQ                = 60000000;
static constexpr uint8_t               ISR_PRIORITY             = 191 + 0x10;

/**********************************************************************************************
 * LEDS     ***********************************************************************************
 **********************************************************************************************/
static constexpr Gpio::Pinout          LED_RED 	            	= PE4;
static constexpr Gpio::Pinout          LED_GREEN 	            = PD7;
static constexpr Gpio::Pinout          LED_YELLOW 	            = PE5;

/**********************************************************************************************
 * LOGGER   ***********************************************************************************
 **********************************************************************************************/
static constexpr Gpio::Pinout          LOGGER_SEL_A 			= PC4;
static constexpr Gpio::Pinout          LOGGER_SEL_B 			= PC5;
static constexpr Usart::LowLevelConfig LOGGER_UART      		= {4, PA0, PA1, ISR_PRIORITY};
static constexpr uint32_t              LOGGER_BAUDRATE          = 115200;
static constexpr Usart::Settings       LOGGER_UART_SETTINGS     = {LOGGER_BAUDRATE,
		                                                           cm3cpp::usart::DataBits::_8,
		                                                           cm3cpp::usart::StopBits::_1,
		                                                           cm3cpp::usart::Parity::PAR_NONE,
		                                                           cm3cpp::usart::Mode::TX,
		                                                           cm3cpp::usart::FlowControl::NONE };
static constexpr Interrupt             LOGGER_ISR               = Interrupt::ISR_UART4;
static constexpr uint32_t              LOGGER_HEADER_LENGTH     = 16;
static constexpr uint32_t              LOGGER_MESSAGE_LENGTH    = 255;
static constexpr uint32_t              LOGGER_MESSAGE_COUNT     = 20;


/**********************************************************************************************
 * MONITOR   **********************************************************************************
 **********************************************************************************************/
static constexpr uint8_t               MONITOR_TIMER_NUMBER 	= 2;
static constexpr uint32_t              MONITOR_TIMER_FREQ       = 1000000;
static constexpr uint32_t              MONITOR_TIMER_PRESQ 		= ((APB1_FREQ * 2) / MONITOR_TIMER_FREQ) - 1;
static constexpr uint32_t              MONITOR_AUTORELOAD       = 0xFFFF;

} /* namespace config */

} /* namespace hw */

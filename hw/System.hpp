/*
 * This file is part of the pastilda project.
 * hosted at http://github.com/thirdpin/pastilda
 *
 * Copyright (C) 2016  Third Pin LLC
 * www.thirdpin.ru
 * www.thirdpin.ru/en/
 *
 * Written by:
 *  Anastasiia Lazareva <a.lazareva@thirdpin.ru>
 *	Dmitrii Lisin 		<d.lisin@thirdpin.ru>
 *	Ilya Stolyarov 		<i.stolyarov@thirdpin.ru>
 *	Pavel Larionov      <p.larionov@thirdpin.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HW_SYSTEM_HPP_
#define HW_SYSTEM_HPP_

#include <libopencmsis/core_cm3.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>

namespace hw {

namespace system {

//must be changed according to your RCC configuration
static constexpr struct rcc_clock_scale rcc_hse_25mhz_to_hclk_120mhz =
{
	20 /*PLLM*/, 192 /*PLLN*/, 2 /*PLLP*/, 5 /*PLLQ*/,
	(FLASH_ACR_ICE | FLASH_ACR_DCE | FLASH_ACR_LATENCY_3WS) /*FLASH CONFIG*/,
	RCC_CFGR_HPRE_DIV_NONE /*HPRE*/,
	RCC_CFGR_PPRE_DIV_4 /*PPRE1*/, RCC_CFGR_PPRE_DIV_2 /*PPRE2*/,
	30000000 /*APB1_FREQ*/, 60000000 /*APB2_FREQ*/
};

constexpr uint32_t SYSTEM_CORE_CLOCK           = 120000000;
constexpr uint32_t SYSTEM_CORE_CLOCK_DIV       = 1000;

inline void clock_init()
{
	rcc_clock_setup_hse_3v3(&rcc_hse_25mhz_to_hclk_120mhz);

	rcc_periph_clock_enable(rcc_periph_clken::RCC_GPIOA);
	rcc_periph_clock_enable(rcc_periph_clken::RCC_GPIOB);
	rcc_periph_clock_enable(rcc_periph_clken::RCC_GPIOC);
	rcc_periph_clock_enable(rcc_periph_clken::RCC_GPIOD);
	rcc_periph_clock_enable(rcc_periph_clken::RCC_GPIOE);
	rcc_periph_clock_enable(rcc_periph_clken::RCC_GPIOG);
	rcc_periph_clock_enable(rcc_periph_clken::RCC_GPIOF);

	rcc_periph_clock_enable(rcc_periph_clken::RCC_USART1); //MDB
	rcc_periph_clock_enable(rcc_periph_clken::RCC_USART2); //MDB
	rcc_periph_clock_enable(rcc_periph_clken::RCC_USART3); //GSM
	rcc_periph_clock_enable(rcc_periph_clken::RCC_UART4);  //Multi uart

	rcc_periph_clock_enable(rcc_periph_clken::RCC_TIM2);   //Monitor
	rcc_periph_clock_enable(rcc_periph_clken::RCC_TIM3);   //BUZZ
	rcc_periph_clock_enable(rcc_periph_clken::RCC_TIM4);   //USB Host
	rcc_periph_clock_enable(rcc_periph_clken::RCC_TIM5);   //syscall 1000Hz clock
	rcc_periph_clock_enable(rcc_periph_clken::RCC_TIM6);   //one wire
	rcc_periph_clock_enable(rcc_periph_clken::RCC_TIM7);   //user button

	rcc_periph_clock_enable(rcc_periph_clken::RCC_OTGHS);  //USB
	rcc_periph_clock_enable(rcc_periph_clken::RCC_SDIO);   //SD card
	rcc_periph_clock_enable(rcc_periph_clken::RCC_DMA2);   //SD card, Power
	rcc_periph_clock_enable(rcc_periph_clken::RCC_I2C1);   //Accelerometer
	rcc_periph_clock_enable(rcc_periph_clken::RCC_SYSCFG); //EXTI
	rcc_periph_clock_enable(rcc_periph_clken::RCC_ADC1);   //Power
	rcc_periph_clock_enable(rcc_periph_clken::RCC_SPI1);   //Eth module
	rcc_periph_clock_enable(rcc_periph_clken::RCC_PWR);    //RTC module
	rcc_periph_clock_enable(rcc_periph_clken::RCC_RTC);    //RTC module
	rcc_periph_clock_enable(rcc_periph_clken::RCC_BKPSRAM);
}

inline void irq_disable() {
	__disable_irq();
}

inline void irq_enable() {
	__enable_irq();
}

inline void systick_init()
{
    systick_set_reload(SYSTEM_CORE_CLOCK / SYSTEM_CORE_CLOCK_DIV);
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_counter_enable();
    systick_interrupt_enable();
}

}

} // namespace hw

#endif /* HW_SYSTEM_HPP_ */

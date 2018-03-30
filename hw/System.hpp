#pragma once

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
	rcc_periph_clock_enable(rcc_periph_clken::RCC_UART4);  //logger
	rcc_periph_clock_enable(rcc_periph_clken::RCC_TIM2);   //monitor

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

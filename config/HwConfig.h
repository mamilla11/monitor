#ifndef CONFIG_HWCONFIG_H_
#define CONFIG_HWCONFIG_H_

#include <stdint.h>
#include <cstring>

#include <libopencm3/stm32/dma.h>

#include <libopencm3_cpp_extensions/cm3cpp_gpio.h>
#include <libopencm3_cpp_extensions/cm3cpp_usart.h>
#include <libopencm3_cpp_extensions/cm3cpp_adc.h>
#include <libopencm3_cpp_extensions/cm3cpp_timer.h>

#include <libopencm3_cpp_extensions/irq/cm3cpp_irq.h>

#include <net/NetCommon.h>

namespace hw {

namespace config {

	static constexpr uint32_t MAX_STRING_LEN                        = 100;
	static constexpr uint32_t MAX_INFO_STRING_LEN                   = 150;

	using Interrupt    = cm3cpp::Interrupt;
	using Adc          = cm3cpp::adc::Adc;
	using Gpio         = cm3cpp::gpio::Gpio;
    namespace cm3usart = cm3cpp::usart;
    using Usart        = cm3cpp::usart::Usart;

    static constexpr uint32_t MAX_FILE_NAME                         = 60;
    static constexpr uint32_t MAX_FILE_CHUNK                        = 512;
    static constexpr uint32_t FTP_BUFFER_SIZE                       = 600;

	static constexpr uint32_t APB1_FREQ                             = 30000000;
	static constexpr uint32_t APB2_FREQ                             = 60000000;

	/**********************************************************************************************
	 * LEDS     ***********************************************************************************
	 **********************************************************************************************/
	static constexpr Gpio::Pinout LED_RED 	            		    = PE4;
	static constexpr Gpio::Pinout LED_GREEN 	            		= PD7;
	static constexpr Gpio::Pinout LED_YELLOW 	            	    = PE5;
	static constexpr uint16_t LED_RED_IMPULSE_MS                    = 1000;
	static constexpr uint8_t  LED_RED_DUTY                          = 2;
	static constexpr uint16_t LED_GREEN_IMPULSE_MS                  = 500;
	static constexpr uint8_t  LED_GREEN_DUTY                        = 10;

	/**********************************************************************************************
	 * RELAYS     *********************************************************************************
	 **********************************************************************************************/
	static constexpr Gpio::Pinout RELAY_1 					        = PB2;
	static constexpr Gpio::Pinout RELAY_2 					        = PD4;
	static constexpr Gpio::Pinout RELAY_3 					        = PD3;
	/**********************************************************************************************
	 * BUTTONS     ********************************************************************************
	 **********************************************************************************************/
	static constexpr Gpio::Pinout BTN_OPEN_DET_1 				    = PB6;
	static constexpr Gpio::Pinout BTN_OPEN_DET_2 				    = PE9;
	static constexpr Gpio::Pinout BTN_USER 					        = PC2;

	static constexpr uint8_t   BTN_USER_TIMER                       = 7;
	static constexpr uint8_t   BTN_USER_NVIC                        = NVIC_TIM7_IRQ;
	static constexpr uint16_t  BTN_USER_PSC                         = ((APB1_FREQ * 2) / 1000) - 1; //1MS
	static constexpr uint16_t  BTN_USER_ARR                         = 10000;
	static constexpr Interrupt BTN_USER_ISR             		   	= Interrupt::ISR_TIM7;
	/**********************************************************************************************
	 * INPUTS     *********************************************************************************
	 **********************************************************************************************/
	static constexpr Gpio::Pinout nDI_1 				            = PE7;
	static constexpr Gpio::Pinout nDI_2 				            = PE8;
	static constexpr Gpio::Pinout nDI_3 				            = PD1;
	static constexpr Gpio::Pinout nDI_4 				            = PD5;

	static constexpr Interrupt INPUT_1_ISR             		   		= Interrupt::ISR_EXTI9_5;
	static constexpr Interrupt INPUT_2_ISR             		   		= Interrupt::ISR_EXTI2;
	static constexpr Interrupt INPUT_3_ISR             		   		= Interrupt::ISR_EXTI1;

	static constexpr uint8_t  INPUT_1_NVIC             		   	    = NVIC_EXTI9_5_IRQ;
	static constexpr uint8_t  INPUT_2_NVIC             		   	    = NVIC_EXTI2_IRQ;
	static constexpr uint8_t  INPUT_3_NVIC             		    	= NVIC_EXTI1_IRQ;
	/**********************************************************************************************
	 * BUZZ     ***********************************************************************************
	 **********************************************************************************************/
	static constexpr Gpio::Pinout BUZZ 				                = PB4;
	static constexpr Gpio::AltFuncNumber BUZZ_AF 				    = Gpio::AltFuncNumber::AF2;
	static constexpr uint8_t BUZZ_TIMER_NUMBER 				        = 3;
	static constexpr uint32_t BUZZ_TIMER_FREQ                       = 1000000;
	static constexpr uint32_t BUZZ_TIMER_PRESQ 				        = ((APB1_FREQ * 2) / BUZZ_TIMER_FREQ) - 1;
	static constexpr uint16_t BUZZ_PERIOD_US                        = 250;
	static constexpr uint16_t BUZZ_INPULSE_US                       = BUZZ_PERIOD_US / 2;
	/**********************************************************************************************
	 * ONE WIRE     *******************************************************************************
	 **********************************************************************************************/
	static constexpr Gpio::Pinout ONE_WIRE 				            = PB1;
	static constexpr uint8_t ONE_WIRE_TIMER_NUMBER 				    = 6;
	static constexpr uint32_t ONE_WIRE_TIMER_FREQ                   = 1000000;
	static constexpr uint32_t ONE_WIRE_TIMER_PRESQ 				    = ((APB1_FREQ * 2) / ONE_WIRE_TIMER_FREQ) - 1;
	/**********************************************************************************************
	 * GSM   **************************************************************************************
	 **********************************************************************************************/
	static constexpr Usart::LowLevelConfig GSM_UART      		    = {3, PB10, PB11, 191 + 0x10};
	static constexpr uint32_t GSM_BAUDRATE               		    = 115200;
	static constexpr Usart::Settings GSM_SETTINGS        		    = {GSM_BAUDRATE,
			                                                   	   	   cm3usart::DataBits::_8,
																	   cm3usart::StopBits::_1,
																	   cm3usart::Parity::PAR_NONE,
																	   cm3usart::Mode::RX_TX,
																	   cm3usart::FlowControl::NONE };
	static constexpr Gpio::Pinout GSM_PWR_CTRL                      = PD0;
	static constexpr Gpio::Pinout GSM_PWR               		    = PD8;
	static constexpr Gpio::Pinout GSM_STAT               		    = PA12;
	static constexpr Interrupt GSM_MODULE_ISR               		= Interrupt::ISR_USART3;
    /**********************************************************************************************
     * ETH      ***********************************************************************************
     **********************************************************************************************/
	static constexpr net::MacAddress MAC_ADDR                       = {0x00, 0x08, 0xDC, 0x4F, 0x27, 0x10};
	static constexpr uint32_t        ETH_SPI                        = 1;
	static constexpr Gpio::Pinout    ETH_SPI_SCK                    = PA5;
	static constexpr Gpio::Pinout    ETH_SPI_MISO                   = PA6;
	static constexpr Gpio::Pinout    ETH_SPI_MOSI                   = PA7;
	static constexpr Gpio::Pinout    ETH_SPI_NSS                    = PA15;
	static constexpr Gpio::Pinout    ETH_RESET                      = PE2;
	static constexpr Gpio::Pinout    ETH_IRQ                        = PE3;


	/**********************************************************************************************
	 * SDIO     ***********************************************************************************
	 **********************************************************************************************/
	static constexpr uint8_t SDIO_PIN_COUNT                			= 6;
	static constexpr Gpio::AltFuncNumber SDIO_AF 				    = Gpio::AltFuncNumber::AF12;
	static constexpr Gpio::Pinout SDIO_D0    					    = PC8;
	static constexpr Gpio::Pinout SDIO_D1    				 	    = PC9;
	static constexpr Gpio::Pinout SDIO_D2    					    = PC10;
	static constexpr Gpio::Pinout SDIO_D3    					    = PC11;
	static constexpr Gpio::Pinout SDIO_CK    					    = PC12;
	static constexpr Gpio::Pinout SDIO_CM    					    = PD2;
	static constexpr Gpio::Pinout SD_DET     					    = PA10;
	static constexpr uint32_t SDIO_DMA		               			= DMA2;
	static constexpr uint32_t SDIO_DMA_STREAM              			= DMA_STREAM3;
	static constexpr uint32_t SDIO_DMA_CHANNEL             			= DMA_SxCR_CHSEL_4;
	static constexpr uint32_t SDIO_NVIC             		   		= NVIC_SDIO_IRQ;
	static constexpr uint32_t SDIO_DMA_NVIC                 		= NVIC_DMA2_STREAM3_IRQ;
	static constexpr Interrupt SDIO_ISR             		   		= Interrupt::ISR_SDIO;
	static constexpr Interrupt SDIO_DMA_ISR                 		= Interrupt::ISR_DMA2_STREAM3;
	static constexpr uint32_t SDIO_NVIC_PRIORITY            		= 0x00;
	static constexpr uint32_t SDIO_DMA_NVIC_PRIORITY         		= 0x80;
	/**********************************************************************************************
	 * USB     ************************************************************************************
	 **********************************************************************************************/
	static constexpr Gpio::Pinout USB_DEVICE_P                      = PB15;
	static constexpr Gpio::Pinout USB_DEVICE_M                      = PB14;
	static constexpr Gpio::Pinout USB_DEVICE_ID                     = PB12;
	static constexpr Gpio::AltFuncNumber USB_DEVICE_AF 			    = Gpio::AltFuncNumber::AF12;
	static constexpr uint8_t USB_DEVICE_NVIC    					= NVIC_OTG_HS_IRQ;
	static constexpr uint8_t USB_DEVICE_NVIC_PRIORITY    			= 0xB0;
	static constexpr Interrupt USB_DEVICE_ISR                 		= Interrupt::ISR_OTG_HS;
	static constexpr Gpio::Pinout USB_HOST_P 					    = PB15;
	static constexpr Gpio::Pinout USB_HOST_M 					    = PB14;
	static constexpr Gpio::AltFuncNumber USB_HOST_AF 			    = Gpio::AltFuncNumber::AF12;
	static constexpr uint8_t  USB_HOST_TIMER_NUMBER     			= 4;
	static constexpr uint16_t USB_HOST_TIMER_PRESCALER  			= (60 - 1);
	static constexpr uint16_t USB_HOST_TIMER_PERIOD     			= (65535);
	static constexpr Gpio::Pinout USB_VBUS_PWR_EN					= PE12;

	/**********************************************************************************************
	 * I2C     ************************************************************************************
	 **********************************************************************************************/
	static constexpr Gpio::Pinout I2C_SCL 					        = PB8;
	static constexpr Gpio::Pinout I2C_SDA                           = PB9;
	static constexpr Gpio::Pinout I2C_nINT                          = PD15;
	static constexpr uint8_t LIS2DH_ADDRESS                         = 0x19;
	static constexpr uint8_t I2C_NUMBER 					        = 1;
	static constexpr uint8_t I2C_CCR 					            = 150;
	static constexpr uint8_t I2C_TRISE 					            = 31;
	static constexpr Interrupt ACCEL_SD_ISR             		    = Interrupt::ISR_EXTI15_10;
	static constexpr uint8_t ACCEL_SD_NVIC             		   	    = NVIC_EXTI15_10_IRQ;
	/**********************************************************************************************
	 * POWER     **********************************************************************************
	 **********************************************************************************************/
#ifdef HW_REV3
	static constexpr Gpio::Pinout POWER_nCHG 					    = PB3;
#else
	static constexpr Gpio::Pinout POWER_nCHG 					    = PD6;
#endif
	static constexpr Gpio::Pinout POWER_GOOD 					    = PE0;
	static constexpr Gpio::Pinout POWER_V_IN 					    = PC3;
	static constexpr Gpio::Pinout POWER_V_BAT 					    = PC0;
	static constexpr Gpio::Pinout PERIPH_EN 					    = PE1;
	static constexpr Gpio::Pinout AUTOMATE_EN                       = PB0;
	static constexpr uint32_t ADC_PIN_COUNT                         = 2;
	static constexpr Adc::Number ADC_NUMBER                         = Adc::Number::ADC_1;
	static constexpr Adc::Channel ADC_POWER_V_BAT                   = Adc::Channel::ADC_CHANNEL10;
	static constexpr Adc::Channel ADC_POWER_V_IN                    = Adc::Channel::ADC_CHANNEL13;
	static constexpr uint32_t ADC_DMA_NUMBER           	            = DMA2;
	static constexpr uint32_t ADC_DMA_STREAM                        = DMA_STREAM0;
	static constexpr uint32_t ADC_DMA_CHANNEL                       = DMA_SxCR_CHSEL_0;
	static constexpr Interrupt POWER_ISR             		   		= Interrupt::ISR_EXTI0;
	static constexpr uint8_t  POWER_NVIC             		   	    = NVIC_EXTI0_IRQ;
	/**********************************************************************************************
	 * SIM CARDS   ********************************************************************************
	 **********************************************************************************************/
	static constexpr Gpio::Pinout SIM_1_EN               		    = PD9;
	static constexpr Gpio::Pinout SIM_2_EN               		    = PA11;
	static constexpr Gpio::Pinout SIM_CSEL               		    = PD10;
	/**********************************************************************************************
	 * MDB NET MODULE   ************************************************************************
	 **********************************************************************************************/
	static constexpr uint8_t MAX_MDB_DEVICES = 5;

	static constexpr Usart::LowLevelConfig MDB_NET_UART      		= {2, PA2, PA3, 191 + 0x10};
	static constexpr uint32_t MDB_NET_BAUDRATE               		= 9600;
	static constexpr Usart::Settings MDB_NET_UART_SETTINGS        		= {MDB_NET_BAUDRATE,
			cm3usart::DataBits::_9,
			cm3usart::StopBits::_1,
			cm3usart::Parity::PAR_NONE,
			cm3usart::Mode::RX_TX,
			cm3usart::FlowControl::NONE };
	static constexpr Interrupt MDB_NET_UART_ISR               		= Interrupt::ISR_USART2;

	static constexpr Usart::LowLevelConfig MDB_LISTEN_SLAVE_UART      		= {1, PNULL, PB7, 191 + 0x10};
	static constexpr Usart::Settings MDB_LISTEN_SLAVE_UART_SETTINGS        		= {MDB_NET_BAUDRATE,
			cm3usart::DataBits::_9,
			cm3usart::StopBits::_1,
			cm3usart::Parity::PAR_NONE,
			cm3usart::Mode::RX,
			cm3usart::FlowControl::NONE };
	static constexpr Interrupt MDB_LISTEN_SLAVE_UART_ISR               		= Interrupt::ISR_USART1;

	static constexpr uint32_t FIFO_SALE_SIZE = 5;
	/**********************************************************************************************
	 * COM PORTS   ************************************************************************
	 **********************************************************************************************/
	static constexpr Gpio::Pinout          LOGGER_SEL_A 			= PC4;
	static constexpr Gpio::Pinout          LOGGER_SEL_B 			= PC5;
	static constexpr Usart::LowLevelConfig LOGGER_UART      		= {4, PA0, PA1, 191 + 0x10};
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
	static constexpr uint32_t              LOGGER_MESSAGE_COUNT     = 30;

	/**********************************************************************************************
	 * Monitor   ************************************************************************
	 **********************************************************************************************/
	static constexpr uint8_t MONITOR_TIMER_NUMBER 				   = 2;
	static constexpr uint32_t MONITOR_TIMER_FREQ                   = 1000000;
	static constexpr uint32_t MONITOR_TIMER_PRESQ 				   = ((APB1_FREQ * 2) / MONITOR_TIMER_FREQ) - 1;
	static constexpr uint32_t MONITOR_AUTORELOAD                   = 0xFFFF;

	/**********************************************************************************************
	 * Time    ************************************************************************************
	 **********************************************************************************************/
	static constexpr uint8_t TIME_TIMER_NUMBER 				    = 5;
	static constexpr uint32_t TIME_TIMER_BUS_DEVIDER                   = 1000; // Wants to 1000Hz

	static constexpr uint32_t TIME_TIMER_PRESQ 				    = ((APB1_FREQ * 2) / TIME_TIMER_BUS_DEVIDER) - 1;
	static constexpr uint32_t TIME_PERIOD                       =UINT32_MAX-1;
	static constexpr Interrupt TIME_ISR_TIMER             		   		= Interrupt::ISR_TIM5;
	static constexpr Interrupt TIME_ISR_RTC                          = Interrupt::ISR_RTC_ALARM;
	//static constexpr Interrupt REAL_TIME_ISR             		   		= Interrupt::ISR_RTC_ALARM;

    /**********************************************************************************************
     * Camera  ************************************************************************************
     **********************************************************************************************/
    static constexpr Gpio::Pinout CAMERA_RESET                      = PB5;
    static constexpr Usart::Settings CAMERA_UART_SETTINGS           = {57600,
                                                                      cm3usart::DataBits::_8,
                                                                      cm3usart::StopBits::_1,
                                                                      cm3usart::Parity::PAR_NONE,
                                                                      cm3usart::Mode::RX_TX,
                                                                      cm3usart::FlowControl::NONE };


} /* namespace config */

} /* namespace hw */

#endif /* CONFIG_HWCONFIG_H_ */

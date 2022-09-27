#include <main.h>

#include <adc.h>
#include <dac.h>
#include <dma2d.h>
#include <fdcan.h>
#include <fmc.h>
#include <gpio.h>
#include <i2c.h>
#include <i2s.h>
#include <sdmmc.h>
#include <spi.h>
#include <usart.h>

#include <fatfs.h>
#include <usb_device.h>

extern void SystemClock_Config(void);
extern void PeriphCommonClock_Config(void);

void CUBEMX_Init() {
	// Turn on I and D Cache
	SCB_EnableICache();
	SCB_EnableDCache();

	// Init HAL
	HAL_Init();

	// Init clocks
	SystemClock_Config();
	PeriphCommonClock_Config();

	// Init peripherals
	MX_GPIO_Init();
	MX_FMC_Init();
	MX_I2C2_Init();
	MX_USART1_UART_Init();
	MX_FDCAN1_Init();
	MX_I2S2_Init();
	MX_SPI4_Init();
	MX_SPI6_Init();
	MX_SDMMC1_SD_Init();
	MX_FATFS_Init();
	MX_USB_DEVICE_Init();
	MX_DMA2D_Init();
	MX_ADC3_Init();
	MX_DAC1_Init();
	MX_ADC1_Init();
	MX_ADC2_Init();
}

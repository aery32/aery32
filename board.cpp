#include "board.h"
#include <aery32/pm.h>
#include <aery32/gpio.h>
#include <aery32/flashc.h>

using namespace aery;

void init_board(void)
{
	gpio_init_pins(porta, 0xffffffff, GPIO_INPUT);
	gpio_init_pins(portb, 0xffffffff, GPIO_INPUT);
	gpio_init_pins(portc, 0x0000003f, GPIO_INPUT);
	gpio_init_pin(LED, GPIO_OUTPUT);

	pm_start_osc(0, OSC_MODE_GAIN3, OSC_STARTUP_36ms);
	pm_wait_osc_to_stabilize(0);

	pm_init_pllvco(pll0, PLL_SOURCE_OSC0, 11, 1, false); // VCO0 = 132 MHz
	pm_enable_pll(pll0, true); // PLL0 = 66 MHz
	pm_wait_pll_to_lock(pll0);

	pm_init_pllvco(pll1, PLL_SOURCE_OSC0, 16, 1, true); // VCO1 = 192 MHz
	pm_enable_pll(pll1, true); // PLL1 = 96 MHz
	pm_wait_pll_to_lock(pll1);

	flashc_init(FLASH_1WS, true); // One wait state for flash
	pm_select_mck(MCK_SOURCE_PLL0); // Main clock speed is now 66 MHz

	/*
	 * Peripheral clock masking. By default all modules are enabled.
	 * You might be interested in to disable modules you are not using.
	 */
	pm->hsbmask = HSBMASK_DEFAULT;
	pm->pbamask = PBAMASK_DEFAULT;
	pm->pbbmask = PBBMASK_DEFAULT;

	while (!(pm->isr & AVR32_PM_ISR_MSKRDY_MASK));
		/*
		 * Clocks are now masked according to (CPU/HSB/PBA/PBB)_MASK
		 * registers.
		 */

}

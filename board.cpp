#include "board.h"
#include <aery32/pm.h>
#include <aery32/gpio.h>

using namespace aery;

void init_board(void)
{
	gpio_init_pins(porta, 0xffffffff, GPIO_INPUT);
	gpio_init_pins(portb, 0xffffffff, GPIO_INPUT);
	gpio_init_pins(portc, 0x0000003f, GPIO_INPUT);

	pm_start_osc(0, OSC_MODE_GAIN3, OSC_STARTUP_36ms);
	pm_wait_osc_to_stabilize(0);

	/* Initialize f_vco0 to 132 MHz. Then divide this by two to get 66 MHz. */
	pm_init_pllvco(pll0, PLL_SOURCE_OSC0, 11, 1, false);
	pm_enable_pll(pll0, true);
	pm_wait_pll_to_lock(pll0);

	/*
	 * Also initialize and enable PLL1; 96 MHz is a good choice as it can be
	 * used for USB when divided by two when initialized as a general clock.
	 */
	pm_init_pllvco(pll1, PLL_SOURCE_OSC0, 16, 1, true);
	pm_enable_pll(pll1, true);
	pm_wait_pll_to_lock(pll1);

	/* For CPU clock speed over 33 MHz, flash wait state has to be set 1 */
	AVR32_FLASHC.FCR.fws = 1;

	/* Set main clock source to PLL0 (66 MHz) */
	pm_select_mck(MCK_SOURCE_PLL0);

	/*
	 * Peripheral clock masking. By default all modules are enabled.
	 * You might be interested in to disable modules you are not using. */
	pm->hsbmask = HSBMASK_DEFAULT;
	pm->pbamask = PBAMASK_DEFAULT;
	pm->pbbmask = PBBMASK_DEFAULT;

	while (!(pm->isr & AVR32_PM_ISR_MSKRDY_MASK));
		/*
		 * Clocks are now masked according to (CPU/HSB/PBA/PBB)_MASK
		 * registers. */

}

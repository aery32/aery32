#include <stdbool.h>
#include "aery32/pm.h"
#include "aery32/gpio.h"
#include "board.h"

void init_board(void)
{
	/* Initialize all pins input */
	aery_gpio_init_pins(
		porta,      /* pointer to port address */
		0xffffffff, /* pin mask */
		GPIO_INPUT  /* option flags */
	);
	aery_gpio_init_pins(portb, 0xffffffff, GPIO_INPUT);
	aery_gpio_init_pins(portc, 0x0000003f, GPIO_INPUT);

	/* Start oscillator */
	aery_pm_start_osc(
		0,                  /* oscillator number */
		PM_OSC_MODE_GAIN3,  /* oscillator mode, see datasheet p.74 */
		PM_OSC_STARTUP_36ms /* oscillator startup time */
	);

	aery_pm_wait_osc_to_stabilize(0 /* oscillator number */);

	/* Initialize f_vco0 of PLL0 to be 132 MHz. */
	aery_pm_init_pllvco(
		pll0,               /* pointer to pll address */
		PM_PLL_SOURCE_OSC0, /* source clock */
		11,                 /* multiplier */
		1,                  /* divider */
		false               /* high frequency */
	);

	/* Enable PLL0 with divide by two block to set f_pll0 to f_vco0 / 2
	 * or 66 MHz. */
	aery_pm_enable_pll(pll0, true  /* divide by two */);

	aery_pm_wait_pll_to_lock(pll0);

	/* Set main clock source to PLL0 == 66 MHz */
	aery_pm_select_mck(PM_MCK_SOURCE_PLL0 /* master clock source */);

	/* Peripheral clock masking. By default all modules are enabled.
	 * You might be interested in to disable modules you are not using. */
	pm->hsbmask = HSBMASK_DEFAULT;
	pm->pbamask = PBAMASK_DEFAULT;
	pm->pbbmask = PBBMASK_DEFAULT;

	while (!(pm->isr & AVR32_PM_ISR_MSKRDY_MASK));
		/* Clocks are now masked according to (CPU/HSB/PBA/PBB)_MASK
		 * registers. */

}

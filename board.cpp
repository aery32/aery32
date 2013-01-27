#include <aery32/pm.h>
#include <aery32/flashc.h>
#include <aery32/gpio.h>
#include "board.h"

using namespace aery;

void board::init(void)
{
	gpio_init_pins(porta, 0xffffffff, GPIO_INPUT);
	gpio_init_pins(portb, 0xffffffff, GPIO_INPUT);
	gpio_init_pins(portc, 0x0000003f, GPIO_INPUT);

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
}

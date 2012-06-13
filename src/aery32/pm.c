/**
 *  _____             ___ ___   |
 * |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
 * |     | -_|  _| | |_  |  _|  |  
 * |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
 *               |___|          |
 * 
 * Copyright (c) 2012, Muiku Oy
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 *    * Neither the name of Muiku Oy nor the names of its contributors may be
 *      used to endorse or promote products derived from this software without
 *      specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdbool.h>
#include <inttypes.h>
#include <avr32/io.h>
#include "aery32/pm.h"

#ifdef AERY_SHORTCUTS
	volatile avr32_pm_t *pm = &AVR32_PM;
	volatile avr32_pm_pll_t *pll0 = &AVR32_PM.PLL[0];
	volatile avr32_pm_pll_t *pll1 = &AVR32_PM.PLL[1];
#endif

int
aery_pm_start_osc(uint8_t oscnum, enum Pm_osc_mode mode,
                  enum Pm_osc_startup startup)
{
	switch (oscnum) {
	case 0:
		if (AVR32_PM.MCCTRL.osc0en == 1) {
			return -1;
		}
		if (mode == PM_OSC_MODE_OSC32) {
			return -1;
		}
		AVR32_PM.OSCCTRL0.mode = mode;
		AVR32_PM.OSCCTRL0.startup = startup;
		AVR32_PM.MCCTRL.osc0en = 1;
		break;
	case 1:
		if (AVR32_PM.MCCTRL.osc1en == 1) {
			return -1;
		}
		if (mode == PM_OSC_MODE_OSC32) {
			return -1;
		}
		AVR32_PM.OSCCTRL1.mode = mode;
		AVR32_PM.OSCCTRL1.startup = startup;
		AVR32_PM.MCCTRL.osc1en = 1;
		break;
	case 32:
		if (AVR32_PM.OSCCTRL32.osc32en == 1) {
			return -1;
		}
		if (mode != PM_OSC_MODE_OSC32 || mode != PM_OSC_MODE_EXTERNAL) {
			return -1;
		}
		AVR32_PM.OSCCTRL32.mode = mode;
		AVR32_PM.OSCCTRL32.startup = startup;
		AVR32_PM.OSCCTRL32.osc32en = 1;
		break;
	default:
		return -1;
	}

	return 0;
}

int
aery_pm_init_pllvco(volatile avr32_pm_pll_t *ppll, enum Pm_pll_source src,
                    uint8_t mul, uint8_t div, bool hifreq)
{
	if (mul < 3 || mul > 16) {
		// PLLMUL term in the equation of f_vco has to be at least 2 (not 3),
		// but the mul parameter here takes account the plus one term which
		// is included in f_vco equation (PLLMUL + 1) thus the mul has to
		// be at last 3 and can be maximum 16. When assigning the value to
		// the register, the mul is decreased by one to match the equation.
		return -1;
	}

	ppll->plltest = 0;
	ppll->plliotesten = 0;
	ppll->pllcount = 63;
	ppll->pllmul = mul - 1;
	ppll->plldiv = (div & ~(0xf0));
	ppll->pllopt = (hifreq == true) ? 0b001 : 0b101;
	ppll->pllosc = src;

	return 0;
}

void
aery_pm_enable_pll(volatile avr32_pm_pll_t *ppll, bool divby2)
{
	switch (divby2) {
	case true:
		ppll->pllopt |= 2;
		break;
	case false:
		ppll->pllopt &= ~2;
		break;
	}

	ppll->pllen = 1;
}

int
aery_pm_init_gclk(enum Pm_gclk clknum, enum Pm_gclk_source clksrc,
                  uint16_t clkdiv)
{
	volatile avr32_pm_gcctrl_t *gclock = &(AVR32_PM.GCCTRL[clknum]);

	if (clkdiv > 256) return -1;

	// Disable general clock before init to prevent glitches on the clock
	// during the possible reinitialization. We have to wait before cen
	// reads zero.
	gclock->cen = 0;
	while (gclock->cen);

	// Select clock source
	gclock->oscsel = (bool) (clksrc & 1);
	gclock->pllsel = (bool) (clksrc & 2);

	// Set divider for the clock source, f_gclk = f_src / (2 * clkdiv)
	if (clkdiv > 0) {
		gclock->diven = 1;
		gclock->div = clkdiv - 1;
	} else {
		gclock->diven = 0;
	}

	return 0;
}

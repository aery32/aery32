/*
 *  _____             ___ ___   |
 * |  _  |___ ___ _ _|_  |_  |  |  C/C++ framework for 32-bit AVRs
 * |     | -_|  _| | |_  |  _|  |  
 * |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
 *               |___|          |
 *
 * Copyright (c) 2012, Muiku Oy
 * All rights reserved.
 *
 * LICENSE
 *
 * New BSD License, see the LICENSE.txt bundled with this package. If you did
 * not receive a copy of the license and are unable to obtain it through the
 * world-wide-web, please send an email to contact@muiku.com so we can send
 * you a copy.
 */

#include "aery32/pm.h"

namespace aery {
	volatile avr32_pm_t *pm = &AVR32_PM;
	volatile avr32_pm_pll_t *pll0 = &AVR32_PM.PLL[0];
	volatile avr32_pm_pll_t *pll1 = &AVR32_PM.PLL[1];
}

#define CKSEL_RESET_MASK(SEL) \
	(~(AVR32_PM_CKSEL_##SEL##SEL_MASK | AVR32_PM_CKSEL_##SEL##DIV_MASK))

#define CKSEL_PRESCALER_MASK(VAR, SEL) \
	(((VAR - 1) << AVR32_PM_CKSEL_##SEL##SEL_OFFSET) | AVR32_PM_CKSEL_##SEL##DIV_MASK)

#define CKSEL_HASDIV(VAR, SEL) \
	((VAR & AVR32_PM_CKSEL_##SEL##DIV_MASK) != 0)

#define CKSEL_GETDIV(VAR, SEL) \
	((VAR & AVR32_PM_CKSEL_##SEL##SEL_MASK) >> AVR32_PM_CKSEL_##SEL##SEL_OFFSET)

int aery::pm_start_osc(uint8_t oscnum, enum Pm_osc_mode mode,
		enum Pm_osc_startup startup)
{
	switch (oscnum) {
	case 0:
		if (AVR32_PM.MCCTRL.osc0en == 1)
			return -1;

		if (mode == OSC_MODE_OSC32)
			return -1;

		AVR32_PM.OSCCTRL0.mode = mode;
		AVR32_PM.OSCCTRL0.startup = startup;
		AVR32_PM.MCCTRL.osc0en = 1;
		break;
	case 1:
		if (AVR32_PM.MCCTRL.osc1en == 1)
			return -1;

		if (mode == OSC_MODE_OSC32)
			return -1;

		AVR32_PM.OSCCTRL1.mode = mode;
		AVR32_PM.OSCCTRL1.startup = startup;
		AVR32_PM.MCCTRL.osc1en = 1;
		break;
	case 32:
		if (AVR32_PM.OSCCTRL32.osc32en == 1)
			return -1;

		if (mode != OSC_MODE_OSC32 || mode != OSC_MODE_EXTERNAL)
			return -1;

		AVR32_PM.OSCCTRL32.mode = mode;
		AVR32_PM.OSCCTRL32.startup = startup;
		AVR32_PM.OSCCTRL32.osc32en = 1;
		break;
	default:
		return -1;
	}

	return 0;
}

int aery::pm_init_pllvco(volatile avr32_pm_pll_t *ppll, enum Pm_pll_source src,
		uint8_t mul, uint8_t div, bool hifreq)
{
	bool was_enabled = (bool) ppll->pllen;

	if (mul < 3 || mul > 16) /* mul < 3, is not a typo */
		return -1;
	ppll->pllen = 0;

	ppll->plltest = 0;
	ppll->plliotesten = 0;
	ppll->pllcount = 63;
	ppll->pllmul = mul - 1;
	ppll->plldiv = (div & ~(0xf0));
	ppll->pllopt = (hifreq == true) ? 0b001 : 0b101;
	ppll->pllosc = src;

	if (was_enabled)
		ppll->pllen = 1;
	return 0;
}

int aery::pm_init_gclk(enum Pm_gclk clknum, enum Pm_gclk_source clksrc,
		uint16_t clkdiv)
{
	volatile avr32_pm_gcctrl_t *gclock = &(AVR32_PM.GCCTRL[clknum]);
	bool was_enabled = (bool) gclock->cen;

	if (clkdiv > 256)
		return -1;

	/*
	 * Disable general clock before init to prevent glitches on the clock
	 * during the possible reinitialization.
	 */
	aery::pm_disable_gclk(clknum);

	/* Select clock source */
	gclock->oscsel = (bool) (clksrc & 1);
	gclock->pllsel = (bool) (clksrc & 2);

	if (clkdiv > 0) {
		gclock->diven = 1;
		gclock->div = clkdiv - 1;
	} else {
		gclock->diven = 0;
	}
	if (was_enabled)
		aery::pm_enable_gclk(clknum);

	return 0;
}

void aery::pm_wait_osc_to_stabilize(uint8_t oscnum)
{
	switch (oscnum) {
	case 0:
		while (!(AVR32_PM.isr & AVR32_PM_ISR_OSC0RDY_MASK));
		break;
	case 1:
		while (!(AVR32_PM.isr & AVR32_PM_ISR_OSC1RDY_MASK));
		break;
	case 32:
		while (!(AVR32_PM.isr & AVR32_PM_ISR_OSC32RDY_MASK));
		break;
	}
}

void aery::pm_wait_pll_to_lock(volatile avr32_pm_pll_t *ppll)
{
	if (ppll == &AVR32_PM.PLL[0]) {
		while (!(AVR32_PM.isr & AVR32_PM_ISR_LOCK0_MASK));
	} else if (ppll == &AVR32_PM.PLL[1]) {
		while (!(AVR32_PM.isr & AVR32_PM_ISR_LOCK1_MASK));
	}
}

void aery::pm_enable_gclk(enum Pm_gclk clknum)
{
	AVR32_PM.GCCTRL[clknum].cen = 1;
}

void aery::pm_disable_gclk(enum Pm_gclk clknum)
{
	AVR32_PM.GCCTRL[clknum].cen = 0;

	/* We have to wait before cen reads zero. */
	while (AVR32_PM.GCCTRL[clknum].cen);
}

void aery::pm_select_mck(enum Pm_mck_source mcksrc)
{
	AVR32_PM.MCCTRL.mcsel = mcksrc;
}

uint32_t aery::pm_get_fmck(void)
{
	uint32_t mck = 0;
	volatile avr32_pm_pll_t *pll0 = &AVR32_PM.PLL[0];

	switch (AVR32_PM.MCCTRL.mcsel) {
	case 0:
		mck = F_SLOWCLK;
		break;
	case 1:
		mck = F_OSC0;
		break;
	case 2:
		if (pll0->pllosc == 0)
			mck = F_OSC0;
		else
			mck = F_OSC1;

		if (pll0->plldiv > 0)
			mck *= (pll0->pllmul + 1) / pll0->plldiv;
		else
			mck *= 2 * (pll0->pllmul + 1);

		if (pll0->pllopt & 2)
			mck /= 2;
		break;
	}

	return mck;
}

int aery::pm_setup_clkdomain(uint8_t prescal, uint8_t domain)
{
	uint32_t cksel = AVR32_PM.cksel;

	if (prescal != 0 && prescal > 8)
		return -1;

	if (domain & CLKDOMAIN_CPU) {
		cksel &= CKSEL_RESET_MASK(CPU);
		if (prescal != 0) {
			cksel |= CKSEL_PRESCALER_MASK(prescal, CPU);
			cksel |= CKSEL_PRESCALER_MASK(prescal, HSB);
		}
	}

	if (domain & CLKDOMAIN_PBA) {
		cksel &= CKSEL_RESET_MASK(PBA);
		if (prescal != 0)
			cksel |= CKSEL_PRESCALER_MASK(prescal, PBA);
	}

	if (domain & CLKDOMAIN_PBB) {
		cksel &= CKSEL_RESET_MASK(PBB);
		if (prescal != 0)
			cksel |= CKSEL_PRESCALER_MASK(prescal, PBB);
	}

	/* Check that PBA and PBB clocks are equal or smaller than CPU clock */
	if (CKSEL_HASDIV(cksel, CPU)) {
		if (!CKSEL_HASDIV(cksel, PBA) || !CKSEL_HASDIV(cksel, PBB))
			return -1;

		if (CKSEL_GETDIV(cksel, CPU) > CKSEL_GETDIV(cksel, PBA))
			return -1;

		if (CKSEL_GETDIV(cksel, CPU) > CKSEL_GETDIV(cksel, PBB))
			return -1;
	}

	/* The register must not be re-written until CKRDY goes high. */
	while (!(AVR32_PM.isr & AVR32_PM_ISR_CKRDY_MASK));
	AVR32_PM.cksel = cksel;

	return 0;
}

uint32_t aery::pm_get_fclkdomain(uint8_t domain)
{
	uint32_t f = aery::pm_get_fmck();

	switch (domain) {
	case CLKDOMAIN_CPU:
		if (CKSEL_HASDIV(AVR32_PM.cksel, CPU))
			f = f >> (CKSEL_GETDIV(AVR32_PM.cksel, CPU) + 1);
		break;
	case CLKDOMAIN_PBA:
		if (CKSEL_HASDIV(AVR32_PM.cksel, PBA))
			f = f >> (CKSEL_GETDIV(AVR32_PM.cksel, PBA) + 1);
		break;
	case CLKDOMAIN_PBB:
		if (CKSEL_HASDIV(AVR32_PM.cksel, PBB))
			f = f >> (CKSEL_GETDIV(AVR32_PM.cksel, PBB) + 1);
		break;
	default:
		f = 0; /* error */
	}

	return f;
}


void aery::pm_enable_pll(volatile avr32_pm_pll_t *ppll, bool divby2)
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

void aery::pm_disable_pll(volatile avr32_pm_pll_t *ppll)
{
	ppll->pllen = 0;
}
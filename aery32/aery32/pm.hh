/*   _____             ___ ___   |
    |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
    |     | -_|  _| | |_  |  _|  |  
    |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
                  |___|          |

    Copyright (c) 2012, Muiku Oy
    All rights reserved.

    LICENSE: This source file is subject to the new BSD license that is
    bundled with this package in the file LICENSE.txt. If you did not
    receive a copy of the license and are unable to obtain it through
    the world-wide-web, please send an email to contact@muiku.com so
    we can send you a copy.
*/

/*!
\file aery32/pm.hh
\brief Power Manager (PM) with aery namespace
\note C++ header file
*/

#ifndef __AERY32_PM_HH
#define __AERY32_PM_HH

#include "pm.h"

namespace aery {

static inline int
pm_start_osc(uint8_t oscnum, enum Pm_osc_mode mode,
             enum Pm_osc_startup startup)
{
	return aery_pm_start_osc(oscnum, mode, startup);
}

static inline int
pm_init_pllvco(volatile avr32_pm_pll_t* ppll, enum Pm_pll_source src,
               uint8_t mul, uint8_t div, bool hifreq)
{
	return aery_pm_init_pllvco(ppll, src, mul, div, hifreq);
}

static inline void
pm_enable_pll(volatile avr32_pm_pll_t* ppll, bool divby2)
{
	aery_pm_enable_pll(ppll, divby2);
}

static inline int
pm_init_gclk(enum Pm_gclk clknum, enum Pm_gclk_source clksrc, uint8_t div)
{
	return aery_pm_init_gclk(clknum, clksrc, div);
}

static inline void
pm_wait_osc_to_stabilize(uint8_t oscnum)
{
	aery_pm_wait_osc_to_stabilize(oscnum);
}

static inline void
pm_wait_pll_to_lock(volatile avr32_pm_pll_t *ppll)
{
	aery_pm_wait_pll_to_lock(ppll);
}

static inline void
pm_enable_gclk(enum Pm_gclk clknum)
{
	aery_pm_enable_gclk(clknum);
}

static inline void
pm_disable_gclk(enum Pm_gclk clknum)
{
	aery_pm_disable_gclk(clknum);
}

static inline void
pm_select_mck(enum Pm_mck_source mcksrc)
{
	aery_pm_select_mck(mcksrc);
}

static inline uint32_t
pm_get_mck(void) {
	return aery_pm_get_mck();
}

static inline int
pm_setup_clkdomain(uint8_t prescaler, enum Pm_ckldomain domain)
{
	return aery_pm_setup_clkdomain(prescaler, domain);
}

static inline uint32_t
pm_get_clkdomain_freq(void) {
	return aery_pm_get_clkdomain_freq();
}

}
#endif

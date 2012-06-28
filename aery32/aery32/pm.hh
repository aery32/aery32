/**
 * \file aery32/pm.hh
 * \brief Power Manager (PM) with aery namespace
 * \note C++ header file
 *
 * \verbatim
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
 * \endverbatim
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

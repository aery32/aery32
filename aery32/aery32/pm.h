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

/*!
\file aery32/pm.h
\brief Power Manager (PM)
*/
 
#ifndef __AERY32_PM_H
#define __AERY32_PM_H

extern "C" {
	#include <inttypes.h>
	#include <avr32/io.h>
}

#ifndef F_OSC0
#define F_OSC0 12000000UL
#endif

#ifndef F_OSC1
#define F_OSC1 16000000UL
#endif

#ifndef F_SLOWCLK
#define F_SLOWCLK 115000UL
#endif

#define CLKDOMAIN_CPU 01
#define CLKDOMAIN_PBA 02
#define CLKDOMAIN_PBB 04
#define CLKDOMAIN_ALL 07

namespace aery {

extern volatile avr32_pm_t *pm;
extern volatile avr32_pm_pll_t *pll0;
extern volatile avr32_pm_pll_t *pll1;

/**
 * General clock number
 */
enum Pm_gclk {
	GCLK0,
	GCLK1,
	GCLK2,
	GCLK3,
	GCLK_USBB,
	GCLK_ABDAC
};

/**
 * General clock source
 */
enum Pm_gclk_source {
	GCLK_SOURCE_OSC0 = 0b00,
	GCLK_SOURCE_OSC1 = 0b01,
	GCLK_SOURCE_PLL0 = 0b10,
	GCLK_SOURCE_PLL1 = 0b11
};

/**
 * PLL source
 */
enum Pm_pll_source {
	PLL_SOURCE_OSC0,
	PLL_SOURCE_OSC1
};

/**
 * Master (or main) clock source
 */
enum Pm_mck_source {
	MCK_SOURCE_RC,
	MCK_SOURCE_OSC0,
	MCK_SOURCE_PLL0
};

/**
 * Oscillator mode
 */
enum Pm_osc_mode {
	OSC_MODE_EXTERNAL,
	OSC_MODE_OSC32,
	OSC_MODE_GAIN0 = 4,
	OSC_MODE_GAIN1,
	OSC_MODE_GAIN2,
	OSC_MODE_GAIN3
};

/**
 * Oscillator startup time
 */
enum Pm_osc_startup {
	OSC_STARTUP_0,
	OSC_STARTUP_560us,
	OSC_STARTUP_1m1s,
	OSC_STARTUP_18ms,
	OSC_STARTUP_36ms,
	OSC_STARTUP_71ms,
	OSC_STARTUP_142ms,
	OSC32_STARTUP_0 = 0,
	OSC32_STARTUP_1m1s,
	OSC32_STARTUP_72m3s,
	OSC32_STARTUP_134ms,
	OSC32_STARTUP_570ms,
	OSC32_STARTUP_1s1,
	OSC32_STARTUP_2s3,
	OSC32_STARTUP_4s6
};

/**
 * Starts the given oscillator
 * \param oscnum  Oscillator number which to start: 0, 1, 32
 * \param mode    Oscillator mode
 * \param startup Oscillator startup time
 * \return Returns 0 on success and -1 on error
 *
 * \note After start you shall wait OSC to stabilize
 *
 * \par Example:
 * \code
 * pm_start_osc(0, OSC_MODE_GAIN3, OSC_STARTUP_36ms);
 * pm_wait_osc_to_stabilize(0);
 * \endcode
 */
int pm_start_osc(uint8_t oscnum, enum Pm_osc_mode mode,
		enum Pm_osc_startup startup);

/**
 * Initializes the voltage controlled oscillator (VCO) the given PLL
 * \param ppll   Pointer to the PLL register
 * \param src    Source oscillator
 * \param mul    The source frequency is multiplier by this value
 * \param div    The source frequency is divided by this value
 * \param hifreq If true enables the high frequency mode
 *
 * \par Equations:
 * - If div > 0, then f_vco = f_src * mul / div
 * - If div = 0, then f_vco = 2 * f_src * mul
 *
 * \note VCO frequency has to be in range of 80 MHz - 180 MHz or if hifreq
 *       mode is enabled in the range of 160 MHz - 240 MHz. 
 */
int pm_init_pllvco(volatile avr32_pm_pll_t *ppll, enum Pm_pll_source src,
		uint8_t mul, uint8_t div, bool hifreq);

/**
 * Enables phase locked loop (PLL)
 * \param ppll   Poiter to pll register
 * \param divby2 Divide the vco frequency by two
 *
 * \note Before enabling PLL, you have to initialize PLL's VCO via
 *       pm_init_pllvco().
 */
void pm_enable_pll(volatile avr32_pm_pll_t *ppll, bool divby2);

/**
 * Initializes the chosen generic clock
 * \param clknum Generic clock number
 * \param clksrc The source that will be used for this generic clock
 * \param div    Source clock divider
 * \return Returns 0 on success and -1 on error
 *
 * \par Equation:
 * - f_gclk = f_src / (2 * div)
 */
int pm_init_gclk(enum Pm_gclk clknum, enum Pm_gclk_source clksrc,
		uint16_t div);

/**
 * Waits oscillator to stabilize
 * \param oscnum Oscillator number
 */
void pm_wait_osc_to_stabilize(uint8_t oscnum);

/**
 * Waits pll to lock
 * \param ppll Pointer to PLL register
 */
void pm_wait_pll_to_lock(volatile avr32_pm_pll_t *ppll);

/**
 * Enables the chosen generic clock
 * \param clknum Clock number which to enable
 */
void pm_enable_gclk(enum Pm_gclk clknum);

/**
 * Disables the given generic clock
 * \param clknum Clock number which to disable
 */
void pm_disable_gclk(enum Pm_gclk clknum);

/**
 * Selects master clock source
 * \param mcksrc Clock source
 */
void pm_select_mck(enum Pm_mck_source mcksrc);

/**
 * Get the master (or main) clock frequency
 * \return Master clock frequency in hertz
 *
 * \note Depends on the defined F_OSC0 and F_OSC1 values. By default calculates
 *       the master clock with F_OSC0 = 12MHz and F_OSC1 = 16MHz. If other
 *       oscillator frequencies are used, make sure to put the new value in
 *       CFLAGS manually or via Makefile, like CFLAGS+=-DF_OSC0=8000000UL.
 */
uint32_t pm_get_fmck(void);

/**
 * Set up clock domain frequency
 * \param prescal   Prescaler value
 * \param clkdomain Clock domain selection
 * \return Returns 0 on success and -1 on error
 *
 * \par Equations:
 * - If prescal > 0, f_clkdomn = f_mck / (2^prescaler)
 * - If prescal = 0, f_clkdomn = f_mck
 */
int pm_setup_clkdomain(uint8_t prescal, uint8_t clkdomain);

/**
 * Get the clock domain frequency
 * \param clkdomain Clock domain selection
 * \return Clock domain frequency in hertz, 0 on error
 *
 * \note Calls pm_get_fmck()
 */
uint32_t pm_get_fclkdomain(uint8_t clkdomain);

} /* end of namespace aery */

#endif

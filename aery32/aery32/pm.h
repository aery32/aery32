/*
 *  _____             ___ ___   |
 * |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <inttypes.h>
#include <avr32/io.h>

#ifdef AERY_SHORTCUTS
	extern volatile avr32_pm_t *pm;
	extern volatile avr32_pm_pll_t *pll0;
	extern volatile avr32_pm_pll_t *pll1;
#endif

#ifndef F_OSC0
#	define F_OSC0 12000000UL
#endif

#ifndef F_OSC1
#	define F_OSC1 16000000UL
#endif

#ifndef F_SLOWCLK
#	define F_SLOWCLK 115000UL
#endif

/**
 * General clock number
 */
enum Pm_gclk {
	PM_GCLK0,
	PM_GCLK1,
	PM_GCLK2,
	PM_GCLK3,
	PM_GCLK_USBB,
	PM_GCLK_ABDAC
};

/**
 * General clock source
 */
enum Pm_gclk_source {
	PM_GCLK_SOURCE_OSC0 = 0b00,
	PM_GCLK_SOURCE_OSC1 = 0b01,
	PM_GCLK_SOURCE_PLL0 = 0b10,
	PM_GCLK_SOURCE_PLL1 = 0b11
};

/**
 * PLL source
 */
enum Pm_pll_source {
	PM_PLL_SOURCE_OSC0,
	PM_PLL_SOURCE_OSC1
};

/**
 * Master (or main) clock source
 */
enum Pm_mck_source {
	PM_MCK_SOURCE_RC,
	PM_MCK_SOURCE_OSC0,
	PM_MCK_SOURCE_PLL0
};

/**
 * Oscillator mode
 */
enum Pm_osc_mode {
	PM_OSC_MODE_EXTERNAL,
	PM_OSC_MODE_OSC32,
	PM_OSC_MODE_GAIN0 = 4,
	PM_OSC_MODE_GAIN1,
	PM_OSC_MODE_GAIN2,
	PM_OSC_MODE_GAIN3
};

/**
 * Oscillator startup time
 */
enum Pm_osc_startup {
	PM_OSC_STARTUP_0,
	PM_OSC_STARTUP_560us,
	PM_OSC_STARTUP_1m1s,
	PM_OSC_STARTUP_18ms,
	PM_OSC_STARTUP_36ms,
	PM_OSC_STARTUP_71ms,
	PM_OSC_STARTUP_142ms,
	PM_OSC32_STARTUP_0 = 0,
	PM_OSC32_STARTUP_1m1s,
	PM_OSC32_STARTUP_72m3s,
	PM_OSC32_STARTUP_134ms,
	PM_OSC32_STARTUP_570ms,
	PM_OSC32_STARTUP_1s1,
	PM_OSC32_STARTUP_2s3,
	PM_OSC32_STARTUP_4s6
};

/**
 * Clock domain number
 */
enum Pm_ckldomain {
	PM_CLKDOMAIN_CPU = 01,
	PM_CLKDOMAIN_PBA = 02,
	PM_CLKDOMAIN_PBB = 04,
	PM_CLKDOMAIN_ALL = 07
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
 * aery_pm_start_osc(0, PM_OSC_MODE_GAIN3, PM_OSC_STARTUP_36ms);
 * aery_pm_wait_osc_to_stabilize(0);
 * \endcode
 */
int aery_pm_start_osc(uint8_t oscnum, enum Pm_osc_mode mode,
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
int aery_pm_init_pllvco(volatile avr32_pm_pll_t *ppll, enum Pm_pll_source src,
		uint8_t mul, uint8_t div, bool hifreq);

/**
 * Enables phase locked loop (PLL)
 * \param ppll   Poiter to pll register
 * \param divby2 Divide the vco frequency by two
 *
 * \note Before enabling PLL, you have to initialize PLL's VCO via
 *       aery_pm_init_pllvco().
 */
void aery_pm_enable_pll(volatile avr32_pm_pll_t *ppll, bool divby2);

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
int aery_pm_init_gclk(enum Pm_gclk clknum, enum Pm_gclk_source clksrc,
		uint16_t div);

/**
 * Waits oscillator to stabilize
 * \param oscnum Oscillator number
 */
void aery_pm_wait_osc_to_stabilize(uint8_t oscnum);

/**
 * Waits pll to lock
 * \param ppll Pointer to PLL register
 */
void aery_pm_wait_pll_to_lock(volatile avr32_pm_pll_t *ppll);

/**
 * Enables the chosen generic clock
 * \param clknum Clock number which to enable
 */
void aery_pm_enable_gclk(enum Pm_gclk clknum);

/**
 * Disables the given generic clock
 * \param clknum Clock number which to disable
 */
void aery_pm_disable_gclk(enum Pm_gclk clknum);

/**
 * Selects master clock source
 * \param mcksrc Clock source
 */
void aery_pm_select_mck(enum Pm_mck_source mcksrc);

/**
 * Get the master (or main) clock frequency
 * \return Master clock frequency in hertz
 *
 * \note Depends on the defined F_OSC0 and F_OSC1 values. By default calculates
 *       the master clock with F_OSC0 = 12MHz and F_OSC1 = 16MHz. If other
 *       oscillator frequencies are used, make sure to put the new value in
 *       CFLAGS manually or via Makefile, like CFLAGS+=-DF_OSC0=8000000UL.
 */
uint32_t aery_pm_get_fmck(void);

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
int aery_pm_setup_clkdomain(uint8_t prescal, enum Pm_ckldomain clkdomain);

/**
 * Get the clock domain frequency
 * \param clkdomain Clock domain selection
 * \return Clock domain frequency in hertz
 *
 * \note Calls aery_pm_get_mck()
 */
uint32_t aery_pm_get_fclkdomain(enum Pm_ckldomain clkdomain);

#ifdef __cplusplus
}
#endif

#endif

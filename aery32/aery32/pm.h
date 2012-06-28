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

enum Pm_gclk {
	PM_GCLK0,
	PM_GCLK1,
	PM_GCLK2,
	PM_GCLK3,
	PM_GCLK_USBB,
	PM_GCLK_ABDAC
};

enum Pm_gclk_source {
	PM_GCLK_SOURCE_OSC0 = 0b00,
	PM_GCLK_SOURCE_OSC1 = 0b01,
	PM_GCLK_SOURCE_PLL0 = 0b10,
	PM_GCLK_SOURCE_PLL1 = 0b11
};

enum Pm_pll_source {
	PM_PLL_SOURCE_OSC0,
	PM_PLL_SOURCE_OSC1
};

enum Pm_mck_source {
	PM_MCK_SOURCE_RC,
	PM_MCK_SOURCE_OSC0,
	PM_MCK_SOURCE_PLL0
};

enum Pm_osc_mode {
	PM_OSC_MODE_EXTERNAL,
	PM_OSC_MODE_OSC32,
	PM_OSC_MODE_GAIN0 = 4,
	PM_OSC_MODE_GAIN1,
	PM_OSC_MODE_GAIN2,
	PM_OSC_MODE_GAIN3
};

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

enum Pm_ckldomain {
	PM_CLKDOMAIN_CPU = 01,
	PM_CLKDOMAIN_PBA = 02,
	PM_CLKDOMAIN_PBB = 04,
	PM_CLKDOMAIN_ALL = 07
};

/**
 * Starts the chosen oscillator
 *
 * \note After start you shall wait OSC to stabilize
 *
 * \par Example
 * \code
 * // Start 12MHz crystal connected in XIN0/XOUT0
 * aery_pm_start_osc(0, PM_OSC_MODE_GAIN3, PM_OSC_STARTUP_36ms);
 * aery_pm_wait_osc_to_stabilize(0);
 * \endcode
 *
 * \param oscnum Oscillator number which to start: 0, 1, 32
 * \param mode Oscillator mode, see datasheet p. 74.
 * \param startup Oscillator startup time
 * \return Returns 0 on success and -1 on error
 */
int aery_pm_start_osc(uint8_t oscnum, enum Pm_osc_mode mode, enum Pm_osc_startup startup);

/**
 * Initializes the frequency of PLL's voltage controlled oscillator (VCO)
 *
 * f_vco = f_src * mul / div, if div > 0
 * f_vco = 2 * f_src * mul, if div == 0
 *
 * \note VCO frequency has to be in range of 80 MHz - 180 MHz or if hifreq mode
 *       is enabled in the range of 160 MHz - 240 MHz. 
 *
 * \param ppll Pointer to pll register
 * \param src Source oscillator
 * \param mul Source frequency is multiplier by this value
 * \param div Source frequency is divided by this value. If zero the 
 *            multiplier is doubled
 * \param hifreq Enable high frequency mode
 */
int aery_pm_init_pllvco(volatile avr32_pm_pll_t *ppll, enum Pm_pll_source src, uint8_t mul,
                        uint8_t div, bool hifreq);

/**
 * Enables phase locked loop (PLL)
 *
 * \note Before enabling PLL you have to initialize PLL vco frequency
 *       by calling aery_pm_init_pllvco()
 *
 * \param ppll Poiter to pll register
 * \param divby2 Divide the vco frequency by two
 */
void aery_pm_enable_pll(volatile avr32_pm_pll_t *ppll, bool divby2);

/**
 * Initializes the chosen generic clock
 *
 * \param clknum Generic clock number:
 * 	PM_GCLK0, PM_GCLK1, PM_GCLK2, PM_GCLK3,
 * 	PM_GCLK_USBB, PM_GCLK_ABDAC
 * \param clksrc Source of the generic clock
 * \param div Clock source divider, f_gclk = f_src/(2*div)
 * \return Returns 0 on success and -1 on error
 */
int aery_pm_init_gclk(enum Pm_gclk clknum, enum Pm_gclk_source clksrc, uint16_t div);

/**
 * Waits oscillator to stabilize
 *
 * \param oscnum Oscillator number: 0, 1 or 32
 */
void aery_pm_wait_osc_to_stabilize(uint8_t oscnum);

/**
 * Waits pll to lock
 *
 * \param ppll Pointer to PLL register
 */
void aery_pm_wait_pll_to_lock(volatile avr32_pm_pll_t *ppll);

/**
 * Enables the chosen generic clock
 *
 * \param clknum Clock number which to enable
 */
void aery_pm_enable_gclk(enum Pm_gclk clknum);

/**
 * Disables the chosen generic clock
 *
 * \param clknum Clock number which to disable
 */
void aery_pm_disable_gclk(enum Pm_gclk clknum);

/**
 * Selects master clock source
 *
 * \param mcksrc Clock source
 */
void aery_pm_select_mck(enum Pm_mck_source mcksrc);

/**
 * Get the master (or main) clock frequency
 *
 * \note Depends on the defined F_OSC0 and F_OSC1 values. By default calculates
 * the master clock with F_OSC0 = 12MHz. If other is used, make sure to define
 * this before including the <aery32/pm.h>.
 *
 * \return Master clock frequency in hertz
 */
uint32_t aery_pm_get_mck(void);

/**
 * Set up clock domain frequency
 *
 * \param prescaler Prescaler value, domain clock frequency will
 * be f_mck / (2^prescaler). 0 disables the prescaler making domain clock
 * equalt to the main clock, f_mck.
 * \param clkdomain Clock domain selection: PM_CLKDOMAIN_CPU,
 * PM_CLKDOMAIN_PBA, PM_CLKDOMAIN_PBB
 * \return Returns 0 on success and -1 on error
 */
int aery_pm_setup_clkdomain(uint8_t prescaler, enum Pm_ckldomain clkdomain);

/**
 * Get the clock domain frequency
 *
 * \param clkdomain Clock domain selection: PM_CLKDOMAIN_CPU,
 * PM_CLKDOMAIN_PBA, PM_CLKDOMAIN_PBB
 * \return Returns clock domain frequency in hertz
 */
uint32_t aery_pm_get_clkdomain_freq(enum Pm_ckldomain clkdomain);

#ifdef __cplusplus
}
#endif

#endif

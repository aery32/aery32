/*
 *  _____             ___ ___   |
 * |  _  |___ ___ _ _|_  |_  |  |  C/C++ framework for 32-bit AVRs
 * |     | -_|  _| | |_  |  _|  |  
 * |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
 *               |___|          |
 *
 * Copyright (c) 2012-2013, Muiku Oy
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
\file aery32/tc.h
\brief Timer Counter module 
*/

#ifndef __AERY32_TC_H
#define __AERY32_TC_H

extern "C" {
	#include <inttypes.h>
	#include <avr32/io.h>
}

namespace aery {

/**
 * Pointer to the internal TC module register
 */
extern volatile avr32_tc_t *tc;

/**
 * Last status read
 */
extern volatile uint32_t __tc_lsr;

enum Tc_clock {
	TIMER_CLOCK1,
	TIMER_CLOCK2,
	TIMER_CLOCK3,
	TIMER_CLOCK4,
	TIMER_CLOCK5,
	TIMER_XC0,
	TIMER_XC1,
	TIMER_XC2
};

/**
 * Initializes Timer Counter module
 */
void tc_init(void);

/**
 */
void tc_setup_clock(enum Tc_clock clk);


} /* end of namespace */

#endif

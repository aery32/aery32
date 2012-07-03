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
\file aery32/delay.hh
\brief Delay functions for small delays
\note C++ header file
*/

#ifndef __AERY32_DELAY_HH
#define __AERY32_DELAY_HH

#include "delay.h"

namespace aery {

static inline void delay_cycles(uint32_t cycles)
{
	aery_delay_cycles(cycles);
}

static inline void delay_us(uint16_t us)
{
	aery_delay_us(us);
}

static inline void delay_ms(uint16_t ms)
{
	aery_delay_ms(ms);
}

}
#endif

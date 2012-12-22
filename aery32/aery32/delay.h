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
\file aery32/delay.h
\brief Delay functions for small delays

\note F_CPU has to be defined before hand to make these function accessible.
\par Example
\code
#define F_CPU 12000000UL
#include <aery32/delay.h>
\endcode
*/

#ifndef __AERY32_DELAY_H
#define __AERY32_DELAY_H

extern "C" {
	#include <inttypes.h>
	#include <avr32/io.h>
}

extern int __builtin_mfsr(int reg);
extern void __builtin_mtsr(int reg, int val);

#ifndef F_CPU
#error "F_CPU Not Defined: Define your CPU frequency (F_CPU) in settings.h and provide it through -include option."
#endif

namespace aery {

/**
 * Creates a delay of the desired number of CPU cycles
 * \param cycles CPU cycles
 */
static inline void delay_cycles(uint32_t cycles)
{
	__builtin_mtsr(AVR32_COUNT, 0);
	while ((uint32_t) __builtin_mfsr(AVR32_COUNT) < cycles);
}

/**
 * Delays the program the desired amount of microseconds
 * \param us microseconds
 */
static inline void delay_us(uint16_t us)
{
	delay_cycles((uint32_t) ((F_CPU / 1000000) * us));
}


/**
 * Delays the program the desired amount of milliseconds
 * \param ms milliseconds
 */
static inline void delay_ms(uint16_t ms)
{
	delay_cycles((uint32_t) ((F_CPU / 1000) * ms));
}

} /* end of namespace */

#endif

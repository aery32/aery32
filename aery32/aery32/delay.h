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
\file aery32/delay.h
\brief Delay functions for small delays

\note F_CPU has to be defined before hand to make these function accessible.
\par Example
\code
#define F_CPU 12000000UL
#include <aery32/delay.h>
\endcode

\example toggle_led.c
*/

#ifndef __AERY32_DELAY_H
#define __AERY32_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <avr32/io.h>

#ifndef F_CPU
#	error "F_CPU NOT DEFINED: Delay functions cannot be declared without knowledge of the CPU frequency."
#endif

/**
 * Creates a delay of the desired number of CPU cycles
 *
 * \param cycles CPU cycles
 */
static inline void aery_delay_cycles(uint32_t cycles)
{
	__builtin_mtsr(AVR32_COUNT, 0);
	while ((uint32_t) __builtin_mfsr(AVR32_COUNT) < cycles);
}

/**
 * Delays the program the desired amount of microseconds
 *
 * \param us microseconds
 */
static inline void aery_delay_us(uint16_t us)
{
	aery_delay_cycles((uint32_t) ((F_CPU / 1000000) * us));
}


/**
 * Delays the program the desired amount of milliseconds
 *
 * \param ms milliseconds
 */
static inline void aery_delay_ms(uint16_t ms)
{
	aery_delay_cycles((uint32_t) ((F_CPU / 1000) * ms));
}

#ifdef __cplusplus
}
#endif

#endif

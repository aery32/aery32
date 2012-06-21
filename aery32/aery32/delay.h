/**
 * \file aery32/delay.h
 * \brief Delay functions for small delays
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
 *
 * \example toggle_led.c
 */

#ifndef __AERY32_DELAY_H
#define __AERY32_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <avr32/io.h>

/**
 * Creates a delay of the desired number of CPU cycles
 *
 * \param cycles CPU cycles
 */
static inline void
aery_delay_cycles(uint32_t cycles)
{
	__builtin_mtsr(AVR32_COUNT, 0);
	while ((uint32_t) __builtin_mfsr(AVR32_COUNT) < cycles);
}

#ifdef F_CPU
	/**
	 * Delays the program the desired amount of microseconds
	 * \note F_CPU has to be defined before hand to make this function accessible
	 * \par Example
	 * \code
	 * #define F_CPU 12000000
	 * #include <aery32/delay.h>
	 * \endcode
	 *
	 * \param us microseconds
	 */
	static inline void
	aery_delay_us(uint16_t us)
	{
		aery_delay_cycles((uint32_t) (F_CPU / 1000000) * us);
	}


	/**
	 * Delays the program the desired amount of milliseconds
	 * \note F_CPU has to be defined before hand to make this function accessible
	 * \par Example
	 * \code
	 * #define F_CPU 12000000
	 * #include <aery32/delay.h>
	 * \endcode
	 *
	 * \param ms milliseconds
	 */
	static inline void
	aery_delay_ms(uint16_t ms)
	{
		aery_delay_cycles((uint32_t) (F_CPU / 1000) * ms);
	}
#endif

#ifdef __cplusplus
}
#endif

#endif

/**
 * \file aery32/rtc.h
 * \brief Real Time Counter (RTC)
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
 * \example rtc_interrupt.c
 * \example rtc_delay.c
 */

#ifndef __AERY32_RTC_H
#define __AERY32_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <avr32/io.h>
#include <inttypes.h>
#include <stdbool.h>

enum Rtc_source {
	RTC_SOURCE_RC,
	RTC_SOURCE_OSC32
};

#define RTC_WAIT_MAX 300 // Experimentally set by using 66MHz MCK

/**
 * Initializes RTC
 *
 * \param val Value where to start counting
 * \param top Top value where to count
 * \param prescaler The RTC clock is divided by the 2^(1+prescaler). Prescaler
 *                  can be 0-15.
 * \param source The source for RTC clock
 * \return Returns 0 on success and -1 on error
 */
int aery_rtc_init(uint32_t, uint32_t, uint8_t, enum Rtc_source);

/**
 * Set RTC control register
 *
 * \note You should be fine with rtc_init() in most cases
 * \param ctrl Controller register settings
 */
int aery_rtc_set_control(uint32_t);

/**
 * Set RTC value
 *
 * \param val Value where to start counting
 */
int aery_rtc_set_value(uint32_t);

/**
 * Set RTC top value
 *
 * \param topval Top value where to count
 */
int aery_rtc_set_top(uint32_t);

/**
 * Enable RTC
 *
 * \param enint Enable interrupts. Set true if want to rise an interrupt
 *              always when the counter reach top value. Otherwise set false
 *              to use polling.
 */
void aery_rtc_enable(bool);

/**
 * Wait RTC control register to be ready for write
 *
 * \note RTC_WAIT_MAX can be used for mck_cycles
 * \param mck_cycles How many MCK cycles to wait
 */
int aery_rtc_wait(uint32_t);

/**
 * RTC delay
 *
 * \param rtc_cycles The delay lenght in RTC cycles
 */
void aery_rtc_delay_cycle(uint32_t);

/**
 * Clear RTC interrupt flag
 */
inline void aery_rtc_clear_interrupt(void)
{
	AVR32_RTC.icr = 1;
}

#ifdef __cplusplus
}
#endif

#endif

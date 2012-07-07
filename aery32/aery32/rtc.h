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
\file aery32/rtc.h
\brief Real Time Counter (RTC)
\example rtc_interrupt.c
*/

#ifndef __AERY32_RTC_H
#define __AERY32_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <inttypes.h>
#include <avr32/io.h>

/**
 * RTC source
 */
enum Rtc_source {
	RTC_SOURCE_RC,
	RTC_SOURCE_OSC32
};

#define RTC_WAIT_LOOPMAX 300 /* Experimentally set by using 66MHz MCK */

/**
 * Initializes RTC
 * \param val Value where to start counting
 * \param top Top value where to count
 * \param prescaler The RTC clock is divided by the 2^(1+prescaler). Prescaler
 *                  can be 0-15.
 * \param source The source for RTC clock
 * \return Returns 0 on success and -1 on error
 */
int aery_rtc_init(uint32_t val, uint32_t top, uint8_t prescaler,
		enum Rtc_source source);

/**
 * Set RTC control register
 * \param ctrl Controller register settings
 * \return Returns 0 on success and -1 on error
 * \note You should be fine with rtc_init() in most cases
 */
int aery_rtc_set_control(uint32_t ctrl);

/**
 * Set RTC value
 * \param val Value where to start counting
 * \return Returns 0 on success and -1 on error
 */
int aery_rtc_set_value(uint32_t val);

/**
 * Set RTC top value
 * \param topval Top value where to count
 * \return Returns 0 on success and -1 on error
 */
int aery_rtc_set_top(uint32_t topval);

/**
 * Enable RTC
 * \param enint Enable interrupts. Set true if want to rise an interrupt
 *              always when the counter reach top value. Otherwise set false
 *              to use polling.
 */
void aery_rtc_enable(bool enint);

/**
 * Indicates if RTC registers are ready to written
 * \param loopcycles How many loop cycles to wait. Provides timeout if nothing
 *                   happens.
 * \return Returns true if busy, false if ready
 */
bool aery_rtc_isbusy(uint32_t loopcycles);

/**
 * RTC delay
 * \param cycles The delay lenght in RTC cycles.
 */
void aery_rtc_delay_cycles(uint32_t cycles);

/**
 * Clear RTC interrupt flag
 */
void aery_rtc_clear_interrupt(void);

#ifdef __cplusplus
}
#endif

#endif

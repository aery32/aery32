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
\file aery32/pwm.h
\brief PWM module functions
*/

#ifndef __AERY32_PWM_H
#define __AERY32_PWM_H

extern "C" {
	#include <inttypes.h>
	#include <avr32/io.h>
}

namespace aery {

/**
 * Pointer to the MCU's internal PWM module register
 */
extern volatile avr32_pwm_t *pwm;

/**
 * PWM alignment
 */
enum Pwm_alignment {
	LEFT_ALIGNED,
	CENTER_ALIGNED
};

/**
 * PWM plarity
 */
enum Pwm_polarity {
	START_LOW,
	START_HIGH
};

/**
 * PWM channel clock frequency
 */
enum Pwm_channel_clk {
	MCK,
	MCK_DIVIDED_BY_2,
	MCK_DIVIDED_BY_4,
	MCK_DIVIDED_BY_8,
	MCK_DIVIDED_BY_16,
	MCK_DIVIDED_BY_32,
	MCK_DIVIDED_BY_64,
	MCK_DIVIDED_BY_128,
	MCK_DIVIDED_BY_256,
	MCK_DIVIDED_BY_512,
	MCK_DIVIDED_BY_1024,
	PWM_CLKA,
	PWM_CLKB
};

/**
 * Initializes PWM extra clocks A and B
 * \param prea Prescaler for PWM clock A
 * \param diva Linear divider for PWM clock A. 0 disables the divider.
 * \param preb Prescaler for PWM clock B. Optional.
 * \param divb Linear divider for PWM clock B. Optional.
 * \return 0 on success. -1 on error
 *
 * These extra clocks can be used for PWM channels when initialized by
 * pwm_init_channel(). PWM Clock A and B provide more precise clock selection
 * via the linear divider.
 */
int pwm_init_divab(enum Pwm_channel_clk prea, uint8_t diva,
		enum Pwm_channel_clk preb = MCK, uint8_t divb = 0);

/**
 * Initializes channel in general
 * \param chanum   Channel number
 * \param clk      Channel's clock frequency
 * \param duration Duration that the event (or PWM function) is active.
 *                 Default value is 0.
 * \param period   Period of the event (or PWM function). Default value
 *                 is 0xFFFFF that is the maximum. When period is in its
 *                 maximum the duty-cycle can be set most accurately.
 * \return 0 on success. -1 on error.
 */
int pwm_init_channel(uint8_t chanum, enum Pwm_channel_clk clk,
		uint32_t duration = 0, uint32_t period = 0xFFFFF);

/**
 * Setups channel's mode
 * \param chanum Channel number
 * \param align  Channel's alignment
 * \param polar  Channel's polarity
 * \return 0 on success. -1 on error.
 */
int pwm_setup_chamode(uint8_t chanum, enum Pwm_alignment align,
		enum Pwm_polarity polar);

/**
 * Updates channel's duration
 * \param chanum Channel number
 * \param newval New value for the duration of the PWM
 * \return 0 on success. -1 on error.
 *
 * \note New duration cannot exceed the currently set period.
 */
int pwm_update_duration(uint8_t chanum, uint32_t newval);

/**
 * Updates channel's period
 * \param chanum Channel number
 * \param newval New value for the period of the PWM
 * \return 0 on success. -1 on error.
 *
 * \note New period cannot be less than the currently set duration.
 */
int pwm_update_period(uint8_t chanum, uint32_t newval);

/**
 * Sets (or updates) channel's duty cycle as a percentage
 * \param chanum Channel number
 * \param D      Duty cycle. Min 0.0. Max 1.0.
 * \return 0 on success. -1 on error.
 */
int pwm_update_dutycl(uint8_t chanum, double D);

/**
 * Waits the number of periods
 * \param chanum  Channel number
 * \param periods Number of periods
 */
void pwm_wait_periods(uint8_t chanum, uint32_t periods);

/**
 * Enables channel or channels
 * \param chamask Channel mask
 */
void pwm_enable(uint8_t chamask);

/**
 * Disables channel or channels
 * \param chamask Channel mask
 */
void pwm_disable(uint8_t chamask);

/**
 * Tells if the channel is enabled
 * \param chanum Channel number
 * \return True if the channel is enabled. False if disabled.
 */
bool pwm_isenabled(uint8_t chanum);

} /* end of namespace aery */

#endif

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

extern volatile avr32_pwm_t *pwm;

enum Pwm_alignment {
	LEFT_ALIGNED,
	RIGHT_ALIGNED
};

enum Pwm_polarity {
	START_LOW,
	START_HIGH
};

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
 * Initializes PWM Clocks A and B
 */
int pwm_init_divab(enum Pwm_channel_clk prea, uint8_t diva,
		enum Pwm_channel_clk preb = MCK, uint8_t divb = 0);

/**
 * Init channel
 */
int pwm_init_channel(uint8_t chanum, enum Pwm_channel_clk clk,
		uint32_t duration = 0, uint32_t period = 0x100000);

/**
 * Setup channel mode
 */
int pwm_setup_chamode(uint8_t chanum, enum Pwm_alignment align,
		enum Pwm_polarity polar);

int pwm_update_duration(uint8_t chanum, uint32_t newval);

int pwm_update_period(uint8_t chanum, uint32_t newval);

void pwm_wait_periods(uint8_t chanum, uint32_t periods);

int pwm_update_dutycl(uint8_t chanum, double D);

void pwm_enable(uint8_t chamask);

void pwm_disable(uint8_t chamask);

bool pwm_isenabled(uint8_t chanum);

} /* end of namespace aery */

#endif

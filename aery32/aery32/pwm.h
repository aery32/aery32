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

extern volatile avr32_pwm_t pwm;

extern volatile avr32_pwm_channel_t pwm0;
extern volatile avr32_pwm_channel_t pwm1;
extern volatile avr32_pwm_channel_t pwm2;
extern volatile avr32_pwm_channel_t pwm3;
extern volatile avr32_pwm_channel_t pwm4;
extern volatile avr32_pwm_channel_t pwm5;
extern volatile avr32_pwm_channel_t pwm6;

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
 * Initializes PWM linear divider
 */
int pwm_init_lineardiv(void);

/**
 * Selects channel clock
 */
int pwm_select_clk(volatile avr32_pwm_channel_t pwm,
		enum Pwm_channel_clk clk);

/**
 * Setup channel mode
 */
int pwm_setup_mode(volatile avr32_pwm_channel_t pwm,
		enum Pwm_alignment align, enum Pwm_polarity polar);

int pwm_set_dutycl(volatile avr32_pwm_channel_t pwm,
		uint32_t dutycl, uint32_t period);

int pwm_enable(uint16_t chamask);

int pwm_disable(uint16_t chamask);

} /* end of namespace aery */

#endif

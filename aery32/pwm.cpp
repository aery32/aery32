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

#include "aery32/pwm.h"

volatile avr32_pwm_t *aery::pwm = &AVR32_PWM;

int aery::pwm_init_divab(enum Pwm_channel_clk prea, uint8_t diva,
		enum Pwm_channel_clk preb, uint8_t divb)
{
	if (prea == PWM_CLKA || prea == PWM_CLKB)
		return -1;
	if (preb == PWM_CLKA || preb == PWM_CLKB)
		return -1;
	
	AVR32_PWM.MR.prea = prea;
	AVR32_PWM.MR.diva = diva;

	AVR32_PWM.MR.preb = preb;
	AVR32_PWM.MR.divb = divb;

	return 0;
}

int aery::pwm_init_channel(uint8_t chanum, enum Pwm_channel_clk clk,
		uint32_t duration, uint32_t period)
{
	/* Writing in CDTYn and CPRDn is possible while the chan is disabled */
	if (aery::pwm_isenabled(chanum))
		return -1;
	if (duration > period)
		return -1;

	volatile avr32_pwm_channel_t *pwm = &AVR32_PWM.channel[chanum];
	pwm->CMR.cpre = clk;
	pwm->cdty = duration;
	pwm->cprd = period;

	return 0;
}

int aery::pwm_setup_chamode(uint8_t chanum, enum Pwm_alignment align,
		enum Pwm_polarity polar)
{
	/* Mode can be modified only when the channel is disabled. */
	if (aery::pwm_isenabled(chanum))
		return -1;

	AVR32_PWM.channel[chanum].CMR.calg = align;
	AVR32_PWM.channel[chanum].CMR.cpol = polar;
	return 0;
}

int aery::pwm_update_channel(uint8_t chanum, enum Pwm_update update,
		uint32_t newval)
{
	switch (update)	{
	case PWM_DURATION:
		if (newval > AVR32_PWM.channel[chanum].cprd)
			return -1;
	case PWM_PERIOD:
		if (newval < AVR32_PWM.channel[chanum].cdty)
			return -1;
	}

	uint32_t status = AVR32_PWM.isr;
	AVR32_PWM.channel[chanum].CMR.cpd = update;

	AVR32_PWM.ier |= (1 << chanum);
	while ((status = AVR32_PWM.isr) & (1 << chanum));
	AVR32_PWM.channel[chanum].cupd = newval;
	AVR32_PWM.idr |= (1 << chanum);

	return 0;
}


int aery::pwm_update_dutycl(uint8_t chanum, double D)
{
	return 0;
}

void aery::pwm_enable(uint8_t chamask)
{
	AVR32_PWM.ena = chamask;
}

void aery::pwm_disable(uint8_t chamask)
{
	AVR32_PWM.dis = chamask;
}

bool aery::pwm_isenabled(uint8_t chanum)
{
	return AVR32_PWM.sr & (1 << chanum);
}
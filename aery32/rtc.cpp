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
 
#include <avr32/io.h>
#include <inttypes.h>
#include "aery32/rtc.h"

const uint32_t RTC_CTRL_INIT_DEFAULT = 0b00000000000000010000000000000001;

int aery_rtc_init(uint32_t val, uint32_t topval, uint8_t psel,
		enum Rtc_source src)
{
	uint32_t ctrl =	RTC_CTRL_INIT_DEFAULT |
		((psel & 0x0f) << AVR32_RTC_PSEL_OFFSET);

	if (src == RTC_SOURCE_OSC32)
		ctrl |= 1 << AVR32_RTC_CTRL_CLK32_OFFSET;

	if (aery_rtc_set_value(val) == -1)
		return -1;

	if (aery_rtc_set_top(topval) == -1)
		return -1;

	return aery_rtc_set_control(ctrl);
}

void aery_rtc_enable(bool enint)
{
	if (enint == true)
		AVR32_RTC.ier = 1;
	AVR32_RTC.CTRL.en = 1;
}

int aery_rtc_set_control(uint32_t ctrl)
{
	if (aery_rtc_isbusy(RTC_WAIT_LOOPMAX))
		return -1;
	AVR32_RTC.ctrl = ctrl;

	return 0;
}

int aery_rtc_set_value(uint32_t val)
{
	if (aery_rtc_isbusy(RTC_WAIT_LOOPMAX))
		return -1;
	AVR32_RTC.val = val;

	return 0;
}

int aery_rtc_set_top(uint32_t topval)
{
	if (aery_rtc_isbusy(RTC_WAIT_LOOPMAX))
		return -1;
	AVR32_RTC.top = topval;

	return 0;
}

bool aery_rtc_isbusy(uint32_t loopcycles)
{
	do {
		if (!(AVR32_RTC.ctrl & AVR32_RTC_BUSY_MASK))
			return false;
	} while (loopcycles--);
	return true;
}

void aery_rtc_delay_cycles(uint32_t cycles)
{
	uint32_t target = AVR32_RTC.val + cycles;
	while (target > AVR32_RTC.val);
}

void aery_rtc_clear_interrupt(void)
{
	AVR32_RTC.icr = 1;
}

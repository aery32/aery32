/**
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
 */
 
#include <avr32/io.h>
#include <inttypes.h>
#include "rtc.h"

const uint32_t RTC_CTRL_INIT_DEFAULT = 0b00000000000000010000000000000001;

int
aery_rtc_init(uint32_t val, uint32_t topval, uint8_t psel, enum Rtc_source src)
{
	uint32_t ctrl = RTC_CTRL_INIT_DEFAULT |
			((psel & 0x0f) << AVR32_RTC_PSEL_OFFSET);

	/* If osc32 is 1 select OSC32 else use internal RC clock */
	if (src == 1) {
		ctrl |= 1 << AVR32_RTC_CTRL_CLK32_OFFSET;
	}
	if (!aery_rtc_set_value(val)) {
		return -1;
	}
	if (!aery_rtc_set_top(topval)) {
		return -1;
	}
	return aery_rtc_set_control(ctrl);
}

void
aery_rtc_enable(bool enint)
{
	if (enint == true) {
		AVR32_RTC.ier = 1;
	}
	AVR32_RTC.CTRL.en = 1;
}

int
aery_rtc_set_control(uint32_t ctrl)
{
	if (!aery_rtc_wait(RTC_WAIT_MAX)) {
		return -1;
	}
	AVR32_RTC.ctrl = ctrl;
	return 0;
}

int
aery_rtc_set_value(uint32_t val)
{
	if (!aery_rtc_wait(RTC_WAIT_MAX)) {
		return 0;
	}
	AVR32_RTC.val = val;
	return 1;
}

int
aery_rtc_set_top(uint32_t topval)
{
	if (!aery_rtc_wait(RTC_WAIT_MAX)) {
		return 0;
	}
	AVR32_RTC.top = topval;
	return 1;
}

int
aery_rtc_wait(uint32_t mck_cycles)
{
	for (; mck_cycles > 0; mck_cycles--) {
		if (!(AVR32_RTC.ctrl & AVR32_RTC_BUSY_MASK)) {
			return 1;
		}
	}
	return 0;
}

void
aery_rtc_delay_cycle(uint32_t aery_rtc_cycles)
{
	uint32_t target = AVR32_RTC.val + aery_rtc_cycles;
	while (target > AVR32_RTC.val);
}

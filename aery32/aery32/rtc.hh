/**
 * \file aery32/rtc.hh
 * \brief Real Time Counter (RTC) with aery namespace
 * \note C++ header file
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
 */

#ifndef __AERY32_RTC_HH
#define __AERY32_RTC_HH

#include "rtc.h"

namespace aery {

static inline int
rtc_init(uint32_t val, uint32_t top, uint8_t prescaler,
         enum Rtc_source source)
{
	return aery_rtc_init(val, top, prescaler, source);
}

static inline int
rtc_set_control(uint32_t ctrl)
{
	return aery_rtc_set_control(ctrl);
}

static inline int
rtc_set_value(uint32_t val)
{
	return aery_rtc_set_value(val);
}

static inline int
rtc_set_top(uint32_t topval)
{
	return aery_rtc_set_top(topval);
}

static inline void
rtc_enable(bool enint)
{
	aery_rtc_enable(enint);
}

static inline int
aery_rtc_wait(uint32_t mck_cycles)
{
	return aery_rtc_wait(uint32_t mck_cycles);
}

static inline void
rtc_delay_cycles(uint32_t cycles)
{
	aery_rtc_delay_cycles(uint32_t cycles)
}

static inline void
rtc_clear_interrupt(void)
{
	aery_rtc_clear_interrupt();
}

}
#endif

/*   _____             ___ ___   |
    |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
    |     | -_|  _| | |_  |  _|  |  
    |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
                  |___|          |

    Copyright (c) 2012, Muiku Oy
    All rights reserved.

    LICENSE: This source file is subject to the new BSD license that is
    bundled with this package in the file LICENSE.txt. If you did not
    receive a copy of the license and are unable to obtain it through
    the world-wide-web, please send an email to contact@muiku.com so
    we can send you a copy.
*/

/*!
\file aery32/rtc.hh
\brief Real Time Counter (RTC) with aery namespace
\note C++ header file
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

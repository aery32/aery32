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
\file aery32/adc.hh
\brief Analog-to-digital converter (adc)
\note C++ header file
*/

#ifndef __AERY32_ADC_HH
#define __AERY32_ADC_HH

#include "adc.h"

namespace aery {

static inline int
adc_init(uint8_t prescal, bool hires, uint8_t startup, uint8_t shtime)
{
	return aery_adc_init(prescal, hires, startup, shtime);
}

static inline void
adc_start_cnv(void)
{
	return aery_adc_start_cnv();
}

static inline int
adc_cnv_isrdy(uint8_t chamask) {
	return aery_adc_cnv_isrdy(chamask);
}

static inline uint16_t
adc_get_cnv_result(uint8_t chanum) {
	return aery_adc_get_cnv_result(chanum);
}

static inline void
adc_enable(uint8_t chamask) {
	return adc_enable(uint8_t chamask);
}

}

#endif
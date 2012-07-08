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
\file aery32/adc.hh
\brief Analog-to-digital converter (adc)
\note C++ header file
*/

#ifndef __AERY32_ADC_HH
#define __AERY32_ADC_HH

#include "adc.h"

namespace aery {

static inline int adc_init(uint8_t prescal, bool hires, uint8_t shtime,
	uint8_t startup)
{
	return aery_adc_init(prescal, hires, shtime, startup);
}

static inline void adc_start_cnv(void)
{
	return aery_adc_start_cnv();
}

static inline int adc_cnv_isrdy(uint8_t chamask) {
	return aery_adc_cnv_isrdy(chamask);
}

static inline uint16_t adc_get_cnv(uint8_t chanum) {
	return aery_adc_get_cnv(chanum);
}

static inline void adc_enable(uint8_t chamask) {
	return aery_adc_enable(uint8_t chamask);
}

static inline void adc_disable(uint8_t chamask) {
	return aery_adc_disable(uint8_t chamask);
}

}

#endif
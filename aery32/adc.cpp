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

#include "aery32/adc.h"

namespace aery {
	volatile avr32_adc_t *adc = &AVR32_ADC;
	volatile uint32_t __adc_lsr = AVR32_ADC.sr;
}

void aery::adc_init(uint8_t prescal, bool hires, uint8_t shtime,
		uint8_t startup)
{
	uint8_t chamask = AVR32_ADC.chsr;

	/* Software reset */
	AVR32_ADC.CR.swrst = 1;
	while (AVR32_ADC.CR.swrst == 1);

	/* Options */
	AVR32_ADC.MR.prescal = prescal;
	AVR32_ADC.MR.shtim = shtime & ~0xf0;
	AVR32_ADC.MR.startup = startup & ~0xe0;
	AVR32_ADC.MR.sleep = 0;
	AVR32_ADC.MR.lowres = !hires;
	AVR32_ADC.MR.trgen = 0;

	/* Re-enable channels */
	AVR32_ADC.cher = chamask;
}

void aery::adc_setup_trigger(enum Adc_trigger trigger)
{
	AVR32_ADC.MR.trgen = 1;
	AVR32_ADC.MR.trgsel = trigger;
}

void aery::adc_start_cnv(void)
{
	AVR32_ADC.CR.start = true;
}

uint16_t aery::adc_read_cnv(uint8_t chanum)
{
	return *(&(AVR32_ADC.cdr0) + chanum);
}

uint16_t aery::adc_read_lastcnv(void)
{
	return AVR32_ADC.lcdr;
}

void aery::adc_enable(uint8_t chamask)
{
	AVR32_ADC.cher = chamask;
}

void aery::adc_disable(uint8_t chamask)
{
	AVR32_ADC.chdr = chamask;
}

bool aery::adc_is_enabled(uint8_t chamask)
{
	if (chamask > 0)
		return (AVR32_ADC.chsr & chamask) == chamask;
	return (AVR32_ADC.chsr & chamask) != 0;
}

bool aery::adc_isbusy(uint8_t chamask)
{
	aery::__adc_lsr = AVR32_ADC.sr;
	if (chamask > 0)
		return (aery::__adc_lsr & chamask) != chamask;
	return (aery::__adc_lsr & AVR32_ADC_SR_DRDY_MASK) == 0;
}

bool adc_has_overrun(uint8_t chamask, bool reread)
{
	if (reread == true)
		aery::__adc_lsr = AVR32_ADC.sr;
	if (chamask == 0)
		return (aery::__adc_lsr & AVR32_ADC_GOVRE_MASK);
	return (aery::__adc_lsr & (chamask << AVR32_ADC_OVRE0_OFFSET));
}
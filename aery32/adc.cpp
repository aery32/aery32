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
#include "aery32/pm.h"

volatile avr32_adc_t *aery::adc = &AVR32_ADC;
volatile uint32_t aery::__adc_lsr = AVR32_ADC.sr;

int aery::adc_init(uint8_t prescal, bool hires, uint8_t shtime,
		uint8_t startup)
{
	uint32_t adclk;

	if (prescal > 63)
		return -1;
	AVR32_ADC.CR.swrst = 1;
	adclk = aery::pm_get_fclkdomain(CLKDOMAIN_PBA) / (2 * (prescal + 1));

	switch (hires) {
	case true:
		if (adclk > ADCLK_HIRES_MAX)
			return -1;
		break;
	case false:
		if (adclk > ADCLK_LORES_MAX)
			return -1;
		break;
	}
	
	AVR32_ADC.MR.prescal = prescal;
	AVR32_ADC.MR.shtim = shtime & ~0xf0;
	AVR32_ADC.MR.startup = startup & ~0xe0;
	AVR32_ADC.MR.sleep = 0;
	AVR32_ADC.MR.lowres = !hires;
	AVR32_ADC.MR.trgen = 0;

	return 0;
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

int aery::adc_isbusy(uint8_t chamask)
{
	using namespace aery;

	__adc_lsr = AVR32_ADC.sr;
	if (AVR32_ADC.chsr == 0 || (AVR32_ADC.chsr & chamask) != chamask)
		return -1;
	if (chamask == 0)
		return (__adc_lsr & AVR32_ADC_SR_DRDY_MASK) == 0;
	return (__adc_lsr & chamask) == chamask;
}

bool adc_hasoverrun(uint8_t chamask)
{
	using namespace aery;
	if (chamask == 0)
		return (__adc_lsr & AVR32_ADC_GOVRE_MASK);
	return (__adc_lsr & (chamask << AVR32_ADC_OVRE0_OFFSET));
}
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

#include <stdbool.h>
#include <inttypes.h>
#include <avr32/io.h>
#include "aery32/adc.h"
#include "aery32/pm.h"

int aery_adc_init(uint8_t prescal, bool hires, uint8_t shtime, uint8_t startup)
{
	uint32_t adclk;

	if (prescal > 63)
		return -1;

	adclk = aery_pm_get_fclkdomain(PM_CLKDOMAIN_PBA) / (2 * (prescal + 1));

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

void aery_adc_enable(uint8_t chamask)
{
	AVR32_ADC.cher = chamask;
}

void aery_adc_start_cnv(void)
{
	AVR32_ADC.CR.start = true;
}

int aery_adc_cnv_isrdy(uint8_t chamask)
{
	/* Make sure that the channels are also enabled */
	if ((AVR32_ADC.cher & chamask) != chamask)
		return -1;

	return (AVR32_ADC.sr & chamask) == chamask;
}

uint16_t aery_adc_get_cnv(uint8_t chanum)
{
	return *(&(AVR32_ADC.cdr0) + chanum);
}

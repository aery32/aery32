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

#include "aery32/eic.h"

namespace aery {
	volatile avr32_eic_t *eic = &AVR32_EIC;
}

void aery::eic_init(uint8_t pinmask, uint8_t options)
{
	using namespace aery;

	if (options & 001) {
		eic->mode = pinmask; /* Level trigged */
		if (options & 002)
			eic->level = pinmask;
		else
			eic->level &= ~pinmask;
	} else {
		eic->mode &= ~pinmask; /* Edge trigged */
		if (options & 002)
			eic->edge = pinmask;
		else
			eic->edge &= ~pinmask;
	}

	if (options & 004)
		eic->async = pinmask;
	else
		eic->async &= ~pinmask;

	if (options & 010)
		eic->filter = pinmask;
	else
		eic->filter &= ~pinmask;
}

void aery::eic_enable(uint8_t pinmask)
{
	using namespace aery;
	eic->en = pinmask;
	eic->ier = pinmask;
}

void aery::eic_disable(uint8_t pinmask)
{
	using namespace aery;
	eic->dis = pinmask;
}

void aery::eic_clear_event(uint8_t pinmask)
{
	using namespace aery;
	eic->icr = pinmask;
}
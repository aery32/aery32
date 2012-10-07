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

/*!
\file aery32/eic.h
\brief External Interrupt Controller
*/

#ifndef __AERY32_EIC_H
#define __AERY32_EIC_H

extern "C" {
	#include <inttypes.h>
	#include <avr32/io.h>
}

#define EIC_EDGE_TRIGD_FALLING 000
#define EIC_EDGE_TRIGD_RISING  002
#define EIC_LEVEL_TRIGD_LOW    001
#define EIC_LEVEL_TRIGD_HIGH   003
#define EIC_ASYNC              004
#define EIC_ENABLE_FILTER      010

namespace aery {

/**
 * Initializes external interrupt controller module
 */
void eic_init(uint8_t pinmask, uint8_t options);

void eic_enable(uint8_t pinmask);
void eic_disable(uint8_t pinmask);
void eic_clear_event(uint8_t pinmask);

} /* end of namespace */

#endif

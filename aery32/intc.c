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

#include <inttypes.h>
#include <avr32/io.h>
#include "aery32/intc.h"

// These two globals come from exception.S
extern const unsigned int _ipr[20];
extern const unsigned int _evba;

/*
 * ISR handler table; pointers to interrupt service routine functions.
 * One function for every intc group, see datasheet p. 41.
 */
void (*_isr_table[20])(void) = {};

void aery_intc_init(void)
{
	__builtin_mtsr(AVR32_EVBA, (int32_t) &_evba);
	for (int i = 0; i < 20; i++) {
		AVR32_INTC.ipr[i] = _ipr[i];
	}
}

void aery_intc_register_isrhandler(void (*handler)(void),
		uint32_t group, uint8_t priority)
{
	_isr_table[group] = handler;
	AVR32_INTC.ipr[group] |= (priority << AVR32_INTC_INTLEVEL_OFFSET);
}

void aery_intc_enable_globally(void)
{
	__builtin_mtsr(AVR32_SR, __builtin_mfsr(AVR32_SR) & ~(1 << 16));
}

void aery_intc_disable_globally(void)
{
	__builtin_mtsr(AVR32_SR, __builtin_mfsr(AVR32_SR) | (1 << 16));
}

// This is proxy to _isr_table[]. The call happens from exception.S.
__attribute__((__interrupt__)) void _isrhandler_proxy(uint32_t group)
{
	_isr_table[group]();
}


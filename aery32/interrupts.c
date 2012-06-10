/**
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
 */

#include <inttypes.h>
#include <avr32/io.h>
#include "aery32/interrupts.h"

// These two globals come from exception.S
extern const unsigned int _ipr[20];
extern const unsigned int _evba;

// ISR handler table; pointers to interrupt service routine functions.
// One function for every intc group, see datasheet p. 41.
void (*_isr_table[20])(void) = {};

void
aery_intc_init(void)
{
	__builtin_mtsr(AVR32_EVBA, (int32_t) &_evba);
	for (int i = 0; i < 20; i++) {
		AVR32_INTC.ipr[i] = _ipr[i];
	}
}

void
aery_intc_register_isrhandler(void (*handler)(void),
		uint32_t group, uint8_t priority)
{
	_isr_table[group] = handler;
	AVR32_INTC.ipr[group] |= (priority << AVR32_INTC_INTLEVEL_OFFSET);
}

void
aery_intc_enable_globally(void)
{
	__builtin_mtsr(AVR32_SR, __builtin_mfsr(AVR32_SR) & ~(1 << 16));
}

void
aery_intc_disable_globally(void)
{
	__builtin_mtsr(AVR32_SR, __builtin_mfsr(AVR32_SR) | (1 << 16));
}

// This is proxy to _isr_table[]. The call happens from exception.S.
__attribute__((__interrupt__)) void
_isrhandler_proxy(uint32_t group)
{
	_isr_table[group]();
}


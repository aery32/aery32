/**
 * \file aery32/intc.h
 * \brief Interrupt Controller (INTC)
 *
 * \verbatim
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
 * \endverbatim
 *
 * \example rtc_interrupt.c
 */

#ifndef __AERY32_INTC_H
#define __AERY32_INTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

/* Some functions defined here use gcc built-in functions. See more info
 * from AVR32006 http://www.atmel.com/dyn/resources/prod_documents/doc32074.pdf
 */
extern int __builtin_mfsr(int reg);
extern void __builtin_mtsr(int reg, int val);

/**
 * Initializes Interrupt Request Register (IPR) of Interrupt Controller (INTC)
 *
 * Defines IPR register with the addresses of reserved memory for the
 * registered or to be registered ISR function pointers.
 */
void aery_intc_init(void);

/**
 * Register interrupt service routine function
 *
 * The registered function is called when ever the interrupt of the
 * group which for the function was registered occur.
 *
 * \note Remember to init and enable interrupts after the function
 *       registration. You can also have several handlers for the same
 *       interrupt group and switch the handler from one to another by
 *       calling this function multiple times. To change the priority
 *       level just reregister the same handler with the different
 *       priority.
 * \param handler Pointer to the ISR function
 * \param group   Which group the registered ISR function will handle,
 *                see datasheet p. 41.
 * \param priorty Interrupt priority level of this group, see INTLEVEL
 *                from datasheet p. 103.
 */
void aery_intc_register_isrhandler(void (*)(void), uint32_t, uint8_t);

/**
 * Enables interrupts globally
 *
 * \attention Be sure that your code is ready to handle interrupts
 */
void aery_intc_enable_globally(void);

/**
 * Disables interrupts globally
 */
void aery_intc_disable_globally(void);

/**
 * Proxy to _isr_table[] containing the registered ISR function pointers
 *
 * \attention The call of this function happens from exception.S and
 *            user should not call this function from C source code.
 * \param group Interrupt group number, see datasheet p. 41. Reflects to the
 *              _isr_table[] index.
 */
#ifndef DOXYGEN
__attribute__((__interrupt__))
#endif
void _isrhandler_proxy(uint32_t group);

#ifdef __cplusplus
}
#endif

#endif

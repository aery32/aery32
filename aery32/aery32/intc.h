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
\file aery32/intc.h
\brief Interrupt Controller (INTC)
\example rtc_interrupt.c
*/

#ifndef __AERY32_INTC_H
#define __AERY32_INTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

/*
 * Some functions defined here use gcc built-in functions. See more info
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
 * \param handler  Pointer to the ISR function
 * \param group    Which group the registered ISR function will handle,
 *                 see datasheet p. 41.
 * \param priority Interrupt priority level of this group, see INTLEVEL
 *                 from datasheet p. 103.
 *
 * \note Remember to init and enable interrupts after the function
 *       registration. You can also have several handlers for the same
 *       interrupt group and switch the handler from one to another by
 *       calling this function multiple times. To change the priority
 *       level just reregister the same handler with the different
 *       priority.
 *
 * The registered function is called when ever the interrupt of the
 * group which for the function was registered occur.
 */
void aery_intc_register_isrhandler(void (*)(void), uint32_t group,
		uint8_t priority);

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
 * \param group Interrupt group number, see datasheet p. 41. Reflects to the
 *              _isr_table[] index.
 *
 * \attention The call of this function happens from exception.S and
 *            user should not call this function from C source code.
 */
#ifndef DOXYGEN
__attribute__((__interrupt__))
#endif
void _isrhandler_proxy(uint32_t group);

#ifdef __cplusplus
}
#endif

#endif

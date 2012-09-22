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
\file aery32/intc.h
\brief Interrupt Controller (INTC)
*/

#ifndef __AERY32_INTC_H
#define __AERY32_INTC_H

extern "C" {
	#include <inttypes.h>
	#include <avr32/io.h>
}

extern "C" {
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
}

namespace aery {

/**
 * Pointer to the internal interrupt controller register
 */
extern volatile avr32_intc_t *intc;

/**
 * Initializes Interrupt Request Register (IPR) of Interrupt Controller (INTC)
 *
 * Defines IPR register with the addresses of reserved memory for the
 * registered or to be registered ISR function pointers.
 */
void intc_init(void);

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
void intc_register_isrhandler(void (*handler)(void), uint32_t group,
		uint8_t priority);

/**
 * Enables interrupts globally
 * \attention Be sure that your code is ready to handle interrupts
 */
void intc_enable_globally(void);

/**
 * Disables interrupts globally
 */
void intc_disable_globally(void);

} /* end of namespace aery */

#endif

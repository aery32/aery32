/*   _____             ___ ___   |
    |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
    |     | -_|  _| | |_  |  _|  |  
    |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
                  |___|          |

    Copyright (c) 2012, Muiku Oy
    All rights reserved.

    LICENSE: This source file is subject to the new BSD license that is
    bundled with this package in the file LICENSE.txt. If you did not
    receive a copy of the license and are unable to obtain it through
    the world-wide-web, please send an email to contact@muiku.com so
    we can send you a copy.
*/

/*!
\file aery32/intc.hh
\brief Interrupt Controller (INTC) with aery namespace
\note C++ header file
*/

#ifndef __AERY32_INTC_HH
#define __AERY32_INTC_HH

#include "intc.h"

namespace aery {

static inline void
intc_init(void)
{
	aery_intc_init();
}

static inline void
intc_register_isrhandler(void (*)(void) handler, uint32_t group,
                         uint8_t priority)
{
	aery_intc_register_isrhandler(handler, group, priority);
}

static inline void
intc_enable_globally(void)
{
	aery_intc_enable_globally();
}

static inline void
intc_disable_globally(void)
{
	aery_intc_disable_globally();
}

}
#endif

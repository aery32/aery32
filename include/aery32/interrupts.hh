/**
 * \file aery32/interrupts.hh
 * \brief Interrupt Controller (INTC) with aery namespace
 * \note C++ header file
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
 */

#ifndef __AERY32_INTERRUPTS_HH
#define __AERY32_INTERRUPTS_HH

#include "aery32/interrupts.h"

namespace aery {

inline void
intc_init(void)
{
	aery_intc_init();
}

inline void
intc_register_isrhandler(void (*)(void) handler, uint32_t group,
                         uint8_t priority)
{
	aery_intc_register_isrhandler(handler, group, priority);
}

inline void
intc_enable_globally(void)
{
	aery_intc_enable_globally();
}

inline void
intc_disable_globally(void)
{
	aery_intc_disable_globally();
}

}
#endif

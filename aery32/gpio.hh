/**
 * \file aery32/gpio.hh
 * \brief General Purpose Input/Output (GPIO) with aery namespace
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

#ifndef __AERY32_GPIO_HH
#define __AERY32_GPIO_HH

#include "gpio.h"

namespace aery {

inline void gpio_init_pins(volatile avr32_gpio_port_t *pport,
                           uint32_t pinmask, int options)
{
	aery_gpio_init_pins(pport, pinmask, options);
}

inline void gpio_init_pin(uint8_t pinnum, int options)
{
	aery_gpio_init_pin(pinnum, options);
}

inline void gpio_set_pin_high(uint8_t pinnum)
{
	aery_gpio_set_pin_high(pinnum);
}

inline void gpio_set_pin_low(uint8_t pinnum)
{
	aery_gpio_set_pin_low(pinnum);
}

inline void gpio_toggle_pin(uint8_t pinnum)
{
	aery_gpio_toggle_pin(pinnum);
}

inline bool gpio_read_pin(uint8_t pinnum)
{
	return aery_gpio_read_pin(pinnum);
}

inline void gpio_enable_localbus(void)
{
	aery_gpio_enable_localbus();
}

inline void gpio_disable_localbus(void)
{
	aery_gpio_disable_localbus();
}

}
#endif

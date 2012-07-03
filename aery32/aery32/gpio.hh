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

#ifndef __AERY32_GPIO_HH
#define __AERY32_GPIO_HH

#include "gpio.h"

namespace aery {

static inline void gpio_init_pins(volatile avr32_gpio_port_t *pport,
		uint32_t pinmask, int options)
{
	aery_gpio_init_pins(pport, pinmask, options);
}

static inline void gpio_init_pin(uint8_t pinnum, int options)
{
	aery_gpio_init_pin(pinnum, options);
}

static inline void gpio_set_pin_high(uint8_t pinnum)
{
	aery_gpio_set_pin_high(pinnum);
}

static inline void gpio_set_pin_low(uint8_t pinnum)
{
	aery_gpio_set_pin_low(pinnum);
}

static inline void gpio_toggle_pin(uint8_t pinnum)
{
	aery_gpio_toggle_pin(pinnum);
}

static inline bool gpio_read_pin(uint8_t pinnum)
{
	return aery_gpio_read_pin(pinnum);
}

static inline void gpio_enable_localbus(void)
{
	aery_gpio_enable_localbus();
}

static inline void gpio_disable_localbus(void)
{
	aery_gpio_disable_localbus();
}

}
#endif

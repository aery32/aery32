/**
 * \file aery32/gpio.h
 * \brief General Purpose Input/Output (GPIO)
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
 * \example toggle_led.c
 */

#ifndef __AERY32_GPIO_H
#define __AERY32_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <inttypes.h>
#include <avr32/io.h>

extern int __builtin_mfsr(int reg);
extern void __builtin_mtsr(int reg, int val);

#ifdef AERY_SHORTCUTS
	extern volatile avr32_gpio_port_t *porta;
	extern volatile avr32_gpio_port_t *portb;
	extern volatile avr32_gpio_port_t *portc;

	extern volatile avr32_gpio_local_port_t *lporta;
	extern volatile avr32_gpio_local_port_t *lportb;
	extern volatile avr32_gpio_local_port_t *lportc;
#endif

/**
 * Converts GPIO number to the corresponding PORT, see datasheet p. 175
 *
 * \param gpio GPIO number: AVR32_GPIO_PIN00 | AVR32_GPIO_PIN01 |
 *                          AVR32_GPIO_PIN02 | AVR32_GPIO_PIN03 | etc.
 */
#define GPIO_NUM2PORT(gpio) (gpio >> 5)

/**
 * Converts GPIO number to the corresponding pin number of the port number,
 * which is M_GPIO_NUM2PORT(gpio_number), see datasheet p. 175.
 *
 * \par Example
 * \code
 * port = GPIO_NUM2PORT(34); // port = 1, or PORTB
 * pin = GPIO_NUM2PIN(34);   // pin  = 2, or PB02
 * \endcode
 * \param gpio GPIO number: AVR32_GPIO_PIN00 | AVR32_GPIO_PIN01 |
 *                          AVR32_GPIO_PIN02 | AVR32_GPIO_PIN03 | etc.
 */
#define GPIO_NUM2PIN(gpio) (gpio % 32)

#define GPIO_INPUT            0000
#define GPIO_LOW              0000
#define GPIO_HIGH             0001
#define GPIO_OUTPUT           0002
#define GPIO_FUNCTION_A       0004
#define GPIO_FUNCTION_B       0005
#define GPIO_FUNCTION_C       0006
#define GPIO_FUNCTION_D       0007
#define GPIO_INT_PIN_CHANGE   0040
#define GPIO_INT_RAISING_EDGE 0050
#define GPIO_INT_FALLING_EDGE 0060
#define GPIO_PULLUP           0100
#define GPIO_OPENDRAIN        0200
#define GPIO_GLITCH_FILTER    0400
#define GPIO_HIZ              0203

/**
 * Initializes several pins at once
 *
 * \param pport Pointer to the GPIO port
 * \param pinmask Pins to initialize, e.g. pin mask 0x7 covers pins 0, 1 and 2
 * \param options Option flags
 */
void aery_gpio_init_pins(volatile avr32_gpio_port_t*, uint32_t, int);

/**
 * Initializes one pin which is given as a GPIO number
 *
 * \param pinnum GPIO number, e.g. AVR32_PIN_PA05
 * \param options Option flags
 */
inline void aery_gpio_init_pin(uint8_t pinnum, int options)
{
	aery_gpio_init_pins(&AVR32_GPIO.port[GPIO_NUM2PORT(pinnum)],
		(1 << GPIO_NUM2PIN(pinnum)), options);
}

/**
 * Sets pin high
 *
 * \note Does not work for local bus
 * \param pinnum GPIO number, e.g. AVR32_PIN_PA05
 */
inline void aery_gpio_set_pin_high(uint8_t pinnum)
{
	AVR32_GPIO.port[GPIO_NUM2PORT(pinnum)].ovrs = 1 << GPIO_NUM2PIN(pinnum);
}

/**
 * Sets pin low
 *
 * \note Does not work for local bus
 * \param pinnum GPIO number, e.g. AVR32_PIN_PA05
 */
inline void aery_gpio_set_pin_low(uint8_t pinnum)
{
	AVR32_GPIO.port[GPIO_NUM2PORT(pinnum)].ovrc = 1 << GPIO_NUM2PIN(pinnum);
}

/**
 * Toggles pin 
 *
 * \note Does not work for local bus
 * \param pinnum GPIO number, e.g. AVR32_PIN_PA05
 */
inline void aery_gpio_toggle_pin(uint8_t pinnum)
{
	AVR32_GPIO.port[GPIO_NUM2PORT(pinnum)].ovrt = 1 << GPIO_NUM2PIN(pinnum);
}

/**
 * Reads pin value
 *
 * \note Does not work for local bus
 * \param pinnum GPIO number, e.g. AVR32_PIN_PA05
 * \return Pin value high or low
 */
inline int aery_gpio_read_pin(uint8_t pinnum)
{
	return AVR32_GPIO.port[GPIO_NUM2PORT(pinnum)].pvr
	       & (1 << GPIO_NUM2PIN(pinnum));
}

/**
 * Enables GPIO local bus
 *
 * Before GPIO ports can be accessed through local bus you have to call
 * this function. Note that when local bus has been enabled you
 * cannot access GPIO ports through PBB (Peripheral Bus B) anymore, which
 * was the default bus accessing them, and thus functions such as
 * aery_gpio_set_pin_high() or aery_gpio_read_pin() has no effect. From now on
 * you have to use bitbanging and instead of using, for example, porta->ovr
 * you have to use new expression, lporta->ovr. To reroute GPIO back to
 * PBB call function gpio_disable_localbus().
 *
 * \attention CPU clock has to match with PBB clock to make local bus
 *            functional
 */
inline void aery_gpio_enable_localbus(void)
{
	__builtin_mtsr(AVR32_CPUCR,
		__builtin_mfsr(AVR32_CPUCR) | AVR32_CPUCR_LOCEN_MASK);
}

/**
 * Disables GPIO local bus.
 *
 * \see gpio_enable_localbus()
 */ 
inline void aery_gpio_disable_localbus(void)
{
	__builtin_mtsr(AVR32_CPUCR,
		__builtin_mfsr(AVR32_CPUCR) & ~AVR32_CPUCR_LOCEN_MASK);
}

#ifdef __cplusplus
}
#endif

#endif

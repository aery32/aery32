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
\file aery32/gpio.h
\brief General Purpose Input/Output (GPIO)
*/

#ifndef __AERY32_GPIO_H
#define __AERY32_GPIO_H

extern "C" {
	#include <inttypes.h>
	#include <avr32/io.h>
}

extern int __builtin_mfsr(int reg);
extern void __builtin_mtsr(int reg, int val);

/**
 * Converts GPIO number to the corresponding PORT, see datasheet p. 175
 * \param gpio GPIO number, AVR32_GPIO_PIN00, AVR32_GPIO_PIN01, etc.
 */
#define GPIO_NUM2PORT(gpio) (gpio >> 5)

/**
 * Converts GPIO number to the corresponding pin number of the port number,
 * which is GPIO_NUM2PORT(gpio_number), see datasheet p. 175.
 * \param gpio GPIO number, AVR32_GPIO_PIN00, AVR32_GPIO_PIN01, etc.
 *
 * \par Example:
 * \code
 * port = GPIO_NUM2PORT(34); // port = 1, or PORTB
 * pin = GPIO_NUM2PIN(34);   // pin  = 2, or PB02
 * \endcode
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

namespace aery {

/**
 * Pointer to GPIO register port A
 */
extern volatile avr32_gpio_port_t *porta;

/**
 * Pointer to GPIO register port B
 */
extern volatile avr32_gpio_port_t *portb;

/**
 * Pointer to GPIO register port C
 */
extern volatile avr32_gpio_port_t *portc;

/**
 * Pointer to local GPIO register port A
 */
extern volatile avr32_gpio_local_port_t *lporta;

/**
 * Pointer to local GPIO register port B
 */
extern volatile avr32_gpio_local_port_t *lportb;

/**
 * Pointer to local GPIO register port C
 */
extern volatile avr32_gpio_local_port_t *lportc;

/**
 * Initializes several pins at once
 * \param pport   Pointer to the GPIO port
 * \param pinmask Pins to initialize, e.g. pin mask 0x7 covers pins 0, 1 and 2
 * \param options Option flags
 */
void gpio_init_pins(volatile avr32_gpio_port_t *pport, uint32_t pinmask,
		int options);

/**
 * Initializes one pin which is given as a GPIO number
 * \param pinnum  GPIO number, e.g. AVR32_PIN_PA05
 * \param options Option flags
 */
void gpio_init_pin(uint8_t pinnum, int options);

/**
 * Sets pin high
 * \param pinnum GPIO number, e.g. AVR32_PIN_PA05
 *
 * \note Does not work for local bus
 */
static inline void gpio_set_pin_high(uint8_t pinnum)
{
	AVR32_GPIO.port[GPIO_NUM2PORT(pinnum)].ovrs = 1UL << GPIO_NUM2PIN(pinnum);
}

/**
 * Sets pin low
 * \param pinnum GPIO number, e.g. AVR32_PIN_PA05
 *
 * \note Does not work for local bus
 */
static inline void gpio_set_pin_low(uint8_t pinnum)
{
	AVR32_GPIO.port[GPIO_NUM2PORT(pinnum)].ovrc = 1UL << GPIO_NUM2PIN(pinnum);
}

/**
 * Toggles pin 
 * \param pinnum GPIO number, e.g. AVR32_PIN_PA05
 *
 * \note Does not work for local bus
 */
static inline void gpio_toggle_pin(uint8_t pinnum)
{
	AVR32_GPIO.port[GPIO_NUM2PORT(pinnum)].ovrt = 1UL << GPIO_NUM2PIN(pinnum);
}

/**
 * Reads pin value
 * \param pinnum GPIO number, e.g. AVR32_PIN_PA05
 * \return Pin value high or low, 1 or 0
 *
 * \note Does not work for local bus
 */
static inline bool gpio_read_pin(uint8_t pinnum)
{
	return AVR32_GPIO.port[GPIO_NUM2PORT(pinnum)].pvr & (1UL << GPIO_NUM2PIN(pinnum));
}

/**
 * Enables GPIO local bus
 *
 * Before GPIO ports can be accessed through local bus you have to call
 * this function. Note that when local bus has been enabled you
 * cannot access GPIO ports through PBB (Peripheral Bus B) anymore, which
 * was the default bus accessing them, and thus functions such as
 * gpio_set_pin_high() or gpio_read_pin() has no effect. From now on
 * you have to use bitbanging and instead of using, for example, porta->ovr
 * you have to use new expression, lporta->ovr. To reroute GPIO back to
 * PBB call function gpio_disable_localbus().
 *
 * \attention CPU clock has to match with PBB clock to make local bus
 *            functional
 */
static inline void gpio_enable_localbus(void)
{
	__builtin_mtsr(AVR32_CPUCR,
		__builtin_mfsr(AVR32_CPUCR) | AVR32_CPUCR_LOCEN_MASK);
}

/**
 * Disables GPIO local bus.
 *
 * \see gpio_enable_localbus()
 */ 
static inline void gpio_disable_localbus(void)
{
	__builtin_mtsr(AVR32_CPUCR,
		__builtin_mfsr(AVR32_CPUCR) & ~AVR32_CPUCR_LOCEN_MASK);
}

} /* end of namespace aery */

#endif

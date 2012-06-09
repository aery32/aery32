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
#include "gpio.h"

#ifdef AERY_SHORTCUTS
	volatile avr32_gpio_port_t *porta = &AVR32_GPIO.port[0];
	volatile avr32_gpio_port_t *portb = &AVR32_GPIO.port[1];
	volatile avr32_gpio_port_t *portc = &AVR32_GPIO.port[2];

	volatile avr32_gpio_local_port_t *lporta = &AVR32_GPIO_LOCAL.port[0];
	volatile avr32_gpio_local_port_t *lportb = &AVR32_GPIO_LOCAL.port[1];
	volatile avr32_gpio_local_port_t *lportc = &AVR32_GPIO_LOCAL.port[2];
#endif

void
aery_gpio_init_pins(volatile avr32_gpio_port_t *pport, uint32_t pinmask,
                    int options)
{
	/* By default define pin to be controlled by the GPIO with output
	 * drivers disabled. */
	pport->gpers = pinmask;
	pport->oderc = pinmask;

	switch (options & 0007) {
	case 0002:		/* Output */
		pport->oders = pinmask;
		pport->ovrc = pinmask;
		break;
	case 0003:		/* Output high */
		pport->oders = pinmask;
		pport->ovrs = pinmask;
		break;

	/* Before the pin peripheral function can be assigned, the pin has to
	 * be controlled by the GPIO with output drivers disabled, as has been
	 * done by default. The pin can then be assured to be tri-stated while
	 * changing the Peripheral Mux Registers.
	 */
	case 0004:		/* Peripheral Function A */
		pport->pmr1c = pinmask;
		pport->pmr0c = pinmask;
		pport->gperc = pinmask;
		break;
	case 0005:		/* Peripheral Function B */
		pport->pmr1c = pinmask;
		pport->pmr0s = pinmask;
		pport->gperc = pinmask;
		break;
	case 0006:		/* Peripheral Function C */
		pport->pmr1s = pinmask;
		pport->pmr0c = pinmask;
		pport->gperc = pinmask;
		break;
	case 0007:		/* Peripheral Function D */
		pport->pmr1s = pinmask;
		pport->pmr0s = pinmask;
		pport->gperc = pinmask;
		break;
	}

	if (options & 0040) {
		/* Interrupt can be enabled on a pin, regardless of the configuration
		 * the I/O line, i.e. controlled by the GPIO or assigned to a
		 * peripheral function. [ds, p.173]
		 */
		switch (options & 0030) {
		case 0000:
			pport->imr0c = pinmask;
			pport->imr1c = pinmask;
			break;
		case 0010:
			pport->imr0s = pinmask;
			pport->imr1c = pinmask;
			break;
		case 0020:
			pport->imr0c = pinmask;
			pport->imr1s = pinmask;
			break;
		}
		pport->iers = pinmask;
	} else {
		pport->ierc = pinmask;
	}

	if (options & 0100) {
		/* Control of the pull-up resistor is possible whether an I/O line
		 * is controlled by a peripheral or the GPIO. */
		pport->puers = pinmask;
	} else {
		pport->puerc = pinmask;
	}

	if (options & 0200) {
		/* The open drain mode can be selected whether the I/O line is
		 * controlled by the GPIO or assigned to a peripheral function. */
		pport->odmers = pinmask;
	} else {
		pport->odmerc = pinmask;
	}

	if (options & 0400) {
		pport->gfers = pinmask;
	} else {
		pport->gferc = pinmask;
	}
}

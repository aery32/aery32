/*
 *  _____             ___ ___   |
 * |  _  |___ ___ _ _|_  |_  |  |  C/C++ framework for 32-bit AVRs
 * |     | -_|  _| | |_  |  _|  |  
 * |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
 *               |___|          |
 *
 * Copyright (c) 2012-2013, Muiku Oy
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
\file aery32/devnull.h
*/

#ifndef __AERY32_OLDSCHOOL_H
#define __AERY32_OLDSCHOOL_H

#include <avr32/io.h>

#define PORTA	(AVR32_GPIO.port[0].ovr)
#define PINA	(AVR32_GPIO.port[0].pvr)
#define DDRA	(AVR32_GPIO.port[0].oder)

#define PORTB	(AVR32_GPIO.port[1].ovr)
#define PINB	(AVR32_GPIO.port[1].pvr)
#define DDRB	(AVR32_GPIO.port[1].oder)

#define PORTC	(AVR32_GPIO.port[2].ovr)
#define PINC	(AVR32_GPIO.port[2].pvr)
#define DDRC	(AVR32_GPIO.port[2].oder)

#endif

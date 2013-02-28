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
\file aery32/util.h
\brief Utilities
*/

#ifndef __AERY32_UTIL_H
#define __AERY32_UTIL_H

extern "C" {
	#include <avr32/io.h>
}

extern int __builtin_mfsr(int reg);
extern void __builtin_mtsr(int reg, int val);

#define START_MEASURING_CYCLES() (__builtin_mtsr(AVR32_COUNT, 0))
#define STOP_MEASURING_CYCLES(__count) ((__count) = __builtin_mfsr(AVR32_COUNT))

#endif

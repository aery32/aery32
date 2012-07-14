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
\file aery32/n2str.h
\brief Set of functions to convert number to string

Sometimes works as a space saver over the sprintf().
*/

#ifndef __AERY32_N2STR_H
#define __AERY32_N2STR_H

extern "C" {
	#include <inttypes.h>
}

namespace aery {

/**
 * Converts unsigned int to string
 * \param number unsigned int
 * \param buf    buffer where to write the string
 * \return number of written chars
 */
int ui2str(unsigned int number, char *buf);

/**
 * Converts int to string
 * \param number int
 * \param buf    buffer where to write the string
 * \return number of written chars
 */
int i2str(int number, char *buf);

/**
 * Converts double to string
 * \param number    double
 * \param precision number of digits
 * \param buf       buffer where to write the string
 * \return number of written chars
 */
int d2str(double number, uint8_t precision, char *buf);

} /* end of namepsace */

#endif
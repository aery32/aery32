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
\file aery32/string.h
\brief String functions

Every now and then sprintf/sscanf family of functions take too much space
of memory or do not even work because of problems like stack overflow. Thus
Aery32 Framework provides a bunch of small string functions. 

EXPERIMENTAL!!! These functions may be removed or heavily changed.
*/

#ifndef __AERY32_STRING_H
#define __AERY32_STRING_H

#include <cstring>
extern "C" {
	#include <inttypes.h>
}

namespace aery {

/**
 * Converts unsigned int to string
 * \param number Unsigned integer
 * \param buffer Buffer where to write the string
 * \param n      A pointer to the number of written chars
 * \return A pointer to the resulting null-terminated string
 */
char *utoa(unsigned int number, char *buffer, size_t *n = NULL);

/**
 * Converts int to string
 * \param number Integer
 * \param buffer Buffer where to write the string
 * \param n      A pointer to the number of written chars
 * \return A pointer to the resulting null-terminated string
 */
char *itoa(int number, char *buffer, size_t *n = NULL);

/**
 * Converts double to string
 * \param number Double
 * \param buffer Buffer where to write the string
 * \param n      A pointer to the number of written chars
 * \return A pointer to the resulting null-terminated string
 */
char *dtoa(double number, uint8_t precision, char *buffer, size_t *n = NULL);

int nputs(const char *str, size_t n, int (*_putchar)(int));

} /* end of namespace aery */

#endif

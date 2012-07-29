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
\file aery32/extio.h
\brief Extended input and output functions for embedded coding
*/

#ifndef __AERY32_EXTIO_H
#define __AERY32_EXTIO_H

#include <cstdio>

namespace aery {

int puts2(const char *str, int (*_putchar)(int));

int nputs2(const char *str, size_t n, int (*_putchar)(int));

} /* end of namespace aery */

#endif

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

/* EXPERIMENTAL!!! These functions may be removed or heavily changed. */

#include "aery32/extio.h"

int puts2(const char *str, int (*_putchar)(int))
{
	int i = 0;
	for (; *(str+i); i++)
		if (_putchar(*(str+i) == EOF)) return EOF;
	return i;
}

int nputs2(const char *str, size_t n, int (*_putchar)(int))
{
	int i = 0;
	for (; *(str+i) && n > 0; i++, n--)
		if (_putchar(*(str+i) == EOF)) return EOF;
	return i;
}

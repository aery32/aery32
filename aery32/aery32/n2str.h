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

/*!
\file aery32/n2str.h
\brief Set of functions to convert the number to string
*/

#ifndef __AERY32_TOSTR_H
#define __AERY32_TOSTR_H

#include <inttypes.h>

int ui2str(unsigned int number, char *buf);
int i2str(int number, char *buf);
int d2str(double number, uint8_t precision, char *buf);

#endif
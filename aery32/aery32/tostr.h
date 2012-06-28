/*   _____             ___ ___   |
    |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
    |     | -_|  _| | |_  |  _|  |  
    |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
                  |___|          |

    Copyright (c) 2012, Muiku Oy
    All rights reserved.

    LICENSE: This source file is subject to the new BSD license that is
    bundled with this package in the file LICENSE.txt. If you did not
    receive a copy of the license and are unable to obtain it through
    the world-wide-web, please send an email to contact@muiku.com so
    we can send you a copy.
*/

/*!
\file aery32/tostr.h
\brief "Something to string" functions
*/

#ifndef __AERY32_TOSTR_H
#define __AERY32_TOSTR_H

#include <inttypes.h>
#include <math.h>

int uint2str(unsigned int number, char *buf);
int int2str(int number, char *buf);
int dbl2str(double number, uint8_t precision, char *buf);

#endif
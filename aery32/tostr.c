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

// !!! EXPERIMENTAL

#include <math.h>
#include <inttypes.h>
#include "aery32/tostr.h"

int uint2str(unsigned int number, char *buf)
{
	uint8_t i = 0, k = 0;
	char t;
	
	if (number == 0)
		buf[i++] = '0';

	while (number > 0) {
		buf[i++] = "0123456789"[number % 10];
		number = number / 10;
	}
	buf[i--] = '\0';

	// Swap the numbers since they are in reverse direction
	while (k < i) {
		t = buf[k];
		buf[k++] = buf[i];
		buf[i--] = t;
	}
	return k+i+1;
}

int int2str(int number, char *buf)
{
	if (number < 0) {
		buf[0] = '-';
		return 1 + uint2str((unsigned int) (-1 * number), &buf[1]);
	}
	return uint2str((unsigned int) number, buf);
}

int dbl2str(double number, uint8_t precision, char *buf)
{
	int n;
	double intpart;
	long double fractpart;

	if ((fractpart = modf(number, &intpart)) < 0)
		fractpart *= -1;

	n = int2str((int) intpart, buf);
	buf[n++] = '.';

	while (precision--) {
		fractpart *= 10;
		uint2str((unsigned int) fractpart, buf + n++);
		fractpart -= (unsigned int) fractpart;
	}
	return n;
}
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

#include <cstdio>
#include <cmath>
extern "C" {
	#include <ieeefp.h>
}
#include "aery32/string.h"

static const char lookup[10] = {'0','1','2','3','4','5','6','7','8','9'};

char *aery::utoa(unsigned int number, char *buffer, size_t *n)
{
	uint8_t i = 0, k = 0;
	char t;
	
	if (number == 0)
		buffer[i++] = '0';

	while (number > 0) {
		buffer[i++] = lookup[number % 10];
		number = number / 10;
	}
	buffer[i--] = '\0';

	/* swap the numbers since they are in reverse order */
	while (k < i) {
		t = buffer[k];
		buffer[k++] = buffer[i];
		buffer[i--] = t;
	}
	if (n != NULL) *n = k+i+1;
	return buffer;
}

char *aery::itoa(int number, char *buffer, size_t *n)
{
	if (number < 0) {
		buffer[0] = '-';
		aery::utoa((unsigned int) (-1 * number), &buffer[1], n);
		if (n != NULL) *n++;
		return buffer;
	}
	return aery::utoa((unsigned int) number, buffer, n);
}

char *aery::dtoa(double number, uint8_t precision, char *buffer, size_t *n)
{
	size_t n2 = 0;
	double ip, fp; /* integer and fractional parts */

	if (isnan(number)) {
		if (n != NULL) *n = 3;
		return strcpy(buffer, "NaN");
	}
	if (isinf(number)) {
		if (n != NULL) *n = 3;
		return strcpy(buffer, "Inf");
	}
	if ((fp = modf(number, &ip)) < 0)
		fp *= -1;

	/* write the integer part and the dot into the buffer */
	aery::itoa((int) ip, buffer, &n2);
	buffer[n2++] = '.';

	/* write the fractional part into the buffer */
	while (precision--) {
		fp *= 10;
		aery::utoa((unsigned int) fp, buffer + n2++);
		fp = fp - (unsigned int) fp;
	}
	if (n != NULL) *n = n2;
	return buffer;
}

int aery::nputs(const char *buffer, size_t n, int (*_putchar)(int))
{
	int i = 0;
	for (; (*(buffer+i) && n > 0); i++, n--) {
		if (_putchar(*(buffer+i)) == EOF)
			return EOF;
	}
	return i;
}
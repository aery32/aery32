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

#include <cmath>
#include <cctype>
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
		if (n != NULL)
			*n += 1;
		return buffer;
	}
	return aery::utoa((unsigned int) number, buffer, n);
}

/* TODO: Fix rounding */
char *aery::dtoa(double number, uint8_t precision, char *buffer, size_t *n)
{
	size_t n2 = 0;
	double ip, fp; /* integer and fractional parts */

	if (std::isnan(number)) {
		if (n != NULL) *n = 3;
		return strcpy(buffer, "NaN");
	}
	if (std::isinf(number)) {
		if (n != NULL) *n = 3;
		return strcpy(buffer, "Inf");
	}

	fp = modf(number, &ip);
	if (std::signbit(number)) {
		/* compensate for loss of sign when converting -0.0 to integer */
		fp *= -1;
		ip *= -1;
		buffer[0] = '-';
		++n2;
	}

	/* write the integer part and the dot into the buffer */
	aery::utoa((unsigned int) ip, &buffer[n2], &n2);
	if (std::signbit(number))
		++n2;
	buffer[n2++] = '.';

	/* write the fractional part into the buffer */
	if (precision > 8)
		precision = 8;
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
	int i = 0, rv;
	for (; (*(buffer+i) && n > 0); i++, n--) {
		if ((rv = _putchar(*(buffer+i))) < 0)
			return rv;
	}
	return i;
}

int aery::line_to_argv(char *line, char *argv[])
{
	unsigned int i = 0, j = 0;

	begin:
		while (line[i] && isspace(line[i])) i++;
		argv[j++] = &line[i];

		while (line[i] && !isspace(line[i])) i++;
		if (line[i] == '\0') goto end;
		line[i++] = '\0'; goto begin;

	end:
		return j;
}

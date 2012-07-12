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
#include <ieeefp.h>
#include <inttypes.h>
#include "aery32/n2str.h"

static const char nlookup[10] = {'0','1','2','3','4','5','6','7','8','9'};

int ui2str(unsigned int number, char *buf)
{
	uint8_t i = 0, k = 0;
	char t;
	
	if (number == 0)
		buf[i++] = '0';

	while (number > 0) {
		buf[i++] = nlookup[number % 10];
		number = number / 10;
	}
	buf[i--] = '\0';

	/* swap the numbers since they are in reverse order */
	while (k < i) {
		t = buf[k];
		buf[k++] = buf[i];
		buf[i--] = t;
	}
	return k+i+1;
}

int i2str(int number, char *buf)
{
	if (number < 0) {
		buf[0] = '-';
		return 1 + ui2str((unsigned int) (-1 * number), &buf[1]);
	}
	return ui2str((unsigned int) number, buf);
}

int d2str(double number, uint8_t precision, char *buf)
{
	int n;
	double ip, fp; /* integer and fractional parts */

	if (isnan(number)) {
		buf[0] = 'N';
		buf[1] = 'a';
		buf[2] = 'N';
		buf[3] = '\0';
		return 3;
	}
	if (isinf(number)) {
		buf[0] = 'I';
		buf[1] = 'n';
		buf[2] = 'f';
		buf[3] = '\0';
		return 3;
	}

	if ((fp = modf(number, &ip)) < 0)
		fp *= -1;

	/* write the integer part with the dot into the buf */
	n = i2str((int) ip, buf);
	buf[n++] = '.';

	/* write the fractional part into the buf */
	while (precision--) {
		fp *= 10;
		ui2str((unsigned int) fp, buf + n++);
		fp = fp - (unsigned int) fp;
	}
	return n;
}
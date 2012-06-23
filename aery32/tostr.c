/**
 *  _____             ___ ___   |
 * |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
 * |     | -_|  _| | |_  |  _|  |  
 * |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
 *               |___|          |
 * 
 * Copyright (c) 2012, Muiku Oy
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 *    * Neither the name of Muiku Oy nor the names of its contributors may be
 *      used to endorse or promote products derived from this software without
 *      specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <math.h>
#include <inttypes.h>
#include "aery32/tostr.h"

int uint2str(unsigned int number, char *buf)
{
	uint8_t i = 0, k = 0;
	char t;
	
	if (number == 0) {
		buf[i++] = '0';
	} else {
		while (number > 0) {
			buf[i++] = "0123456789"[number % 10];
			number = number / 10;
		}
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

	if ((fractpart = modf(number, &intpart)) < 0) {
		fractpart *= -1;
	}
	n = int2str((int) intpart, buf);
	buf[n++] = '.';

	while (precision--) {
		fractpart *= 10;
		uint2str((unsigned int) fractpart, buf + n++);
		fractpart -= (unsigned int) fractpart;
	}
	return n;
}
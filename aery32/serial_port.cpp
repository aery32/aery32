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

#include <cctype>

#include "aery32/serial_port.h"
#include "aery32/string.h"
#include "aery32/pm.h"

using namespace aery;

volatile static char __tbuf[25] = ""; /* temp buffer for operator>>() */

serial_port::serial_port(volatile avr32_usart_t *u, periph_idma &i,
	periph_odma &o) : usart(u), idma(i), odma(o), precision(8)
{
	init();
}

serial_port& serial_port::init()
{
	odma.reset();
	idma.reset();
	usart_init_serial(usart, USART_PARITY_NONE, USART_STOPBITS_1,
		USART_DATABITS_8);
	set_speed(115200);
	return *this;
}

double serial_port::set_speed(unsigned int speed)
{
	unsigned int clk = pm_get_fclkdomain(CLKDOMAIN_PBA);
	double cd =  clk / 16 / speed;
	double error =  1 - (speed / (clk / 16 / (unsigned int) cd));

	usart_setup_speed(usart, USART_CLK_PBA, (unsigned int) cd);
	return error;
}

serial_port& serial_port::set_parity(enum Usart_parity parity)
{
	usart->MR.par = parity;
	return *this;
}

serial_port& serial_port::set_stopbits(enum Usart_stopbits stopbits)
{
	usart->MR.nbstop = stopbits;
	return *this;
}

int serial_port::getc()
{
	if (idma.has_overflown())
		return -1;

	uint8_t c;
	while (idma.read(&c, 1) == 0);
	return (int) c;
}

char* serial_port::getline(char *str, char delim)
{
	size_t n = 0;
	return getline(str, &n, delim);
}

char* serial_port::getline(char *str, size_t *n, char delim)
{
	size_t i = 0, j = 0;
	int c;

	for (; i < idma.bufsize; i++) {
		c = getc();
		if (c == delim) {
			break;
		} else if (c == 127) { /* c == (del) */
			if (j > 1)
				j--;
			else 
				j = 0;
		} else {
			str[j++] = c;
		}
	}
	str[j] = '\0';
	*n = j;
	return str;
}

int serial_port::putc(char c)
{
	while (odma.bytes_in_progress());
	odma.write_byte((uint8_t) c);
	odma.flush();
	return 1;
}

int serial_port::puts(const char *str)
{
	size_t n = strlen(str);
	while (odma.bytes_in_progress());
	odma.write((uint8_t*) str, n);
	odma.flush();
	return n;
}

serial_port& serial_port::putback(char c)
{
	idma.r_idx--;
	idma.buffer[idma.r_idx] = (uint8_t) c;
	return *this;
}

serial_port& serial_port::flush()
{
	idma.flush();
	return *this;
}

size_t serial_port::bytes_available()
{
	return idma.bytes_available();
}

bool serial_port::has_overflown()
{
	return idma.has_overflown();
}

serial_port& serial_port::enable()
{
	idma.enable();
	usart_enable_rx(usart);

	odma.enable();
	usart_enable_tx(usart);
	return *this;
}

serial_port& serial_port::disable()
{
	usart_disable_rx(usart);
	idma.disable();

	usart_disable_tx(usart);
	odma.disable();
	return *this;
}

serial_port& serial_port::reset()
{
	bool was_enabled = is_enabled();
	
	disable().init();
	if (was_enabled == true)
		enable();

	return *this;
}

bool serial_port::is_enabled()
{
	return idma.is_enabled() && odma.is_enabled();
}

serial_port& serial_port::operator<<(char c)
{
	putc(c);
	return *this;
}

serial_port& serial_port::operator<<(const char *str)
{
	puts(str);
	return *this;
}

serial_port& serial_port::operator<<(int d)
{
	while (odma.bytes_in_progress());
	itoa(d, (char*) odma.buffer, &odma.w_idx);
	odma.flush();
	return *this;
}

serial_port& serial_port::operator<<(double lf) 
{
	while (odma.bytes_in_progress());
	dtoa(lf, precision, (char*) odma.buffer, &odma.w_idx);
	odma.flush();
	return *this;
}

serial_port& serial_port::operator<<(unsigned int u)
{
	while (odma.bytes_in_progress());
	utoa(u, (char*) odma.buffer, &odma.w_idx);
	odma.flush();
	return *this;
}

serial_port& serial_port::operator<<(unsigned char u)
{
	return *this << (unsigned int) u;
}

serial_port& serial_port::operator<<(unsigned short u)
{
	return *this << (unsigned int) u;
}

serial_port& serial_port::operator<<(unsigned long u)
{
	return *this << (unsigned int) u;
}

serial_port& serial_port::operator>>(int &value)
{
	for (uint8_t i = 0; i < 11; i++) {
		if (isspace(__tbuf[i] = getc()))
			break;
	}
	value = atoi((char*) &__tbuf[0]);
	return *this;
}

serial_port& serial_port::operator>>(double &value)
{
	for (uint8_t i = 0; i < 24; i++) {
		if (isspace(__tbuf[i] = getc()))
			break;
	}
	value = atof((char*) &__tbuf[0]);
	return *this;
}
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

#include "aery32/serial_port_clsdrv.h"
#include "aery32/string.h"
#include "aery32/pm.h"

using namespace aery;

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

	uint8_t c = 0;
	while (idma.read(&c, 1) == 0);
	return (int) c;
}

char* serial_port::getline(char *str, size_t *nread, char delim)
{
	size_t i = 0, j = 0;
	int c = 0;

	for (; i < idma.bufsize; i++) {
		c = getc();
		if (c == delim) {
			break;
		}
		if (c == 127) { /* c == (del) */
			j = (j > 1) ? (j - 1) : 0;
			continue;
		}
		str[j++] = c;
	}
	str[j] = '\0';
	*nread = j;
	return str;
}

char* serial_port::getline(char *str, size_t *nread, const char *delim)
{
	size_t i = 0, j = 0;
	int c = 0;

	for (; i < idma.bufsize; i++) {
		c = getc();
		if (c == delim[1] && str[j-1] == delim[0]) {
			j--;
			break;
		}
		if (c == 127) { /* c == (del) */
			j = (j > 1) ? (j - 1) : 0;
			continue;
		}
		str[j++] = c;
	}
	str[j] = '\0';
	*nread = j;
	return str;
}

int serial_port::putc(char c)
{
	while (odma.bytes_in_progress());
	odma.write((uint8_t*) &c, 1);
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
	idma.idx--;
	idma.buffer[idma.idx] = (uint8_t) c;
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



// --------------------------------------------------------------------------
// OUTPUT, characters
// --------------------------------------------------------------------------
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



// --------------------------------------------------------------------------
// OUTPUT, signed integers
// --------------------------------------------------------------------------
serial_port& serial_port::operator<<(int value)
{
	while (odma.bytes_in_progress());
	itoa(value, (char*) odma.buffer, &odma.idx);
	odma.flush();
	return *this;
}
serial_port& serial_port::operator<<(signed long value)
{
	return this->operator<<((int) value);
}



// --------------------------------------------------------------------------
// OUTPUT, unsigned integers
// --------------------------------------------------------------------------
serial_port& serial_port::operator<<(unsigned int value)
{
	while (odma.bytes_in_progress());
	utoa(value, (char*) odma.buffer, &odma.idx);
	odma.flush();
	return *this;
}
serial_port& serial_port::operator<<(unsigned long value)
{
	return this->operator<<((unsigned int) value);
}
serial_port& serial_port::operator<<(unsigned short value)
{
	return this->operator<<((unsigned int) value);
}
serial_port& serial_port::operator<<(unsigned char value)
{
	return this->operator<<((unsigned int) value);
}



// --------------------------------------------------------------------------
// OUTPUT, double
// --------------------------------------------------------------------------
serial_port& serial_port::operator<<(double lf) 
{
	while (odma.bytes_in_progress());
	dtoa(lf, precision, (char*) odma.buffer, &odma.idx);
	odma.flush();
	return *this;
}

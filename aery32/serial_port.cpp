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

#include "aery32/serial_port.h"
#include "aery32/string.h"
#include "aery32/pm.h"

#include <cstdio>
#include <cstdarg>

using namespace aery;

serial_port::serial_port(volatile avr32_usart_t *u, periph_idma &i,
	periph_odma &o) : usart(u), idma(i), odma(o)
{
	init();
}

void serial_port::init()
{
	odma.reset();
	idma.reset();
	usart_init_serial(usart, USART_PARITY_NONE, USART_STOPBITS_1,
		USART_DATABITS_8);
	set_speed(115200);
}

double serial_port::set_speed(unsigned int speed)
{
	unsigned int clk = pm_get_fclkdomain(CLKDOMAIN_PBA);
	double cd =  clk / 16 / speed;
	double error =  1 - (speed / (clk / 16 / (unsigned int) cd));

	usart_setup_speed(usart, USART_CLK_PBA, (unsigned int) cd);
	return error;
}

void serial_port::set_parity(enum Usart_parity parity)
{
	usart->MR.par = parity;
}

void serial_port::set_stopbits(enum Usart_stopbits stopbits)
{
	usart->MR.nbstop = stopbits;
}

int serial_port::putc(char c)
{
	while (odma.bytes_in_progress());
	odma.write_byte((uint8_t) c);
	odma.flush();
	return 1;
}

int serial_port::getc()
{
	if (idma.has_overflown())
		return EOF;

	int c;
	/* TODO: idma.read should tell how many bytes was read */
	//while (idma.read((uint8_t*) &c, 1) == 1);
	idma.read((uint8_t*) &c, 1);
	return c;
}

int serial_port::puts(const char *str)
{
	size_t n = strlen(str);

	while (odma.bytes_in_progress());
	odma.write((uint8_t*) str, n);
	odma.flush();
	return n;
}

int serial_port::print(const char *format, ... )
{
	int n;
	va_list args;
	va_start(args, format);
	
	while (odma.bytes_in_progress());
	n = vsnprintf((char*) odma.buffer, odma.bufsize, format, args);
	va_end(args);
	
	if (n < 0) { /* vsnprintf() failed */
		odma.w_idx = 0;
		return n;
	}

	odma.w_idx = n;
	odma.flush();
	return n;
}

char* serial_port::getline(char *str, char delim)
{
	size_t i = 0;
	for (; i < idma.bufsize; i++) {
		str[i] = getc();
		if (str[i] == delim)
			break;
		if (str[i] == EOF)
			return NULL;
	}
	str[i] = '\0';
	return str;
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

serial_port& serial_port::operator<<(int i)
{
	print("%d", i);
	return *this;
}

serial_port& serial_port::operator<<(unsigned int i)
{
	print("%u", i);
	return *this;
}

serial_port& serial_port::operator<<(double d) 
{
	print("%lf", d);
	return *this;
}
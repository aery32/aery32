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

using namespace aery;

serial_port::serial_port(volatile avr32_usart_t *u, periph_idma &i,
	periph_odma &o) : usart(u), idma(i), odma(o)
{
}

// } uint32_t speed,
// 	enum Usart_databits databits, enum Usart_parity parity, enum Usart_stopbits)
// {

int serial_port::putc(char c)
{
	return 0;
}

int serial_port::getc()
{
	return 0;
}

int serial_port::puts(const char *str)
{
	return 0;
}

int serial_port::print(const char *str, ... )
{
	return 0;
}

char* serial_port::getline(char *str, size_t n, char terminator)
{
	return 0;
}

char* serial_port::getline(char *str, char terminator)
{
	return 0;
}

serial_port& serial_port::flush()
{
	return *this;
}

size_t serial_port::bytes_available()
{
	return 0;
}

bool serial_port::has_overflown()
{
	return 0;
}

serial_port& serial_port::enable()
{
	return *this;
}

serial_port& serial_port::disable()
{
	return *this;
}

serial_port& serial_port::reset()
{
	return *this;
}

bool serial_port::is_enabled()
{
	return true;
}

serial_port& serial_port::operator<<(const char *str)
{
	return *this;
}

serial_port& serial_port::operator<<(int i)
{
	return *this;
}

serial_port& serial_port::operator<<(unsigned int i)
{
	return *this;
}

serial_port& serial_port::operator<<(double d) 
{
	return *this;
}

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
	init();
}

void serial_port::init()
{
	usart->CR.rsttx = 1;
	usart->CR.rstrx = 1;
	usart->MR.mode = 0;
	usart->MR.filter = 0;
	usart->MR.msbf = 0;
	usart->MR.sync = 0;
	usart->MR.clko = 0;

	set_speed(115200);
	set_databits(USART_DATABITS_8);
	set_parity(USART_PARITY_NONE);
	set_stopbits(USART_STOPBITS_1);
}

double serial_port::set_speed(unsigned int speed)
{
	unsigned int clk = pm_get_fclkdomain(CLKDOMAIN_PBA);
	double cd =  clk / 8 / speed;
	double error =  1 - (speed / (clk / 8 / (unsigned int) cd));

	usart_setup_speed(usart, USART_CLK_PBA, (unsigned int) cd);
	return error;
}

void serial_port::set_databits(enum Usart_databits databits)
{
	usart_set_databits(usart, databits);
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
	return 0;
}

int serial_port::getc()
{
	return 0;
}

int serial_port::puts(const char *str)
{
	size_t n = strlen(str);
	odma.write((uint8_t*) str, n);
	odma.write_byte('\n');
	odma.flush();
	return n;
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
	idma.enable();
	odma.enable();
	usart_enable_rx(usart);
	usart_enable_tx(usart);
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

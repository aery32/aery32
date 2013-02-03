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

serial_port::serial_port(volatile avr32_usart_t *usart,	uint32_t speed,
	enum Usart_databits databits, enum Usart_parity parity, enum Usart_stopbits)
{

}

serial_port& serial_port::set_idma(aery::periph_idma &idma)
{}

serial_port& serial_port::set_odma(aery::periph_odma &odma)
{}

int serial_port::putc(char c)
{}

int serial_port::getc()
{}

int serial_port::puts(const char *str)
{}

int serial_port::print(const char *str, ... )
{}

char* serial_port::getline(char *str, size_t n, char terminator)
{}

char* serial_port::getline(char *str, char terminator)
{}

serial_port& serial_port::flush()
{}

size_t serial_port::bytes_available()
{}

bool serial_port::has_overflown()
{}

serial_port& serial_port::enable()
{}

serial_port& serial_port::disable()
{}

serial_port& serial_port::reset()
{}

bool serial_port::is_enabled()
{}

serial_port& serial_port::operator<<(const char *str)
{}

serial_port& serial_port::operator<<(int)
{}

serial_port& serial_port::operator<<(unsigned int)
{}

serial_port& serial_port::operator<<(double)
{}


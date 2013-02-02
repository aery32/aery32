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

/*!
\file aery32/serial_port.h
\brief An RS-232 serial port connection
*/

#ifndef __AERY32_SERIAL_PORT_H
#define __AERY32_SERIAL_PORT_H

extern "C" {
	#include <avr32/io.h>
	#include <inttypes.h>
}
#include <cstdlib>

#include "usart.h"
#include "periph_idma.h"
#include "periph_odma.h"

#ifndef SERIAL_PORT_DEFAULT_TERMINATOR
#define SERIAL_PORT_DEFAULT_TERMINATOR '\n'
#endif

namespace aery {

class serial_port {

	public:
		serial_port(
			volatile avr32_usart_t *usart,
			uint32_t speed,
			enum Usart_databits databits = USART_DATABITS_8,
			enum Usart_parity parity = USART_PARITY_NONE,
			enum Usart_stopbits = USART_STOPBITS_1
		);

		serial_port& set_idma(aery::periph_idma &idma);
		serial_port& set_odma(aery::periph_odma &odma);

		int puts(const char *str);
		int putc(char c);
		int getc();

		char* getline(char *str, size_t n,
			char terminator = SERIAL_PORT_DEFAULT_TERMINATOR);
		char* getline(char *str,
			char terminator = SERIAL_PORT_DEFAULT_TERMINATOR);

		serial_port& flush();

		size_t bytes_available();
		bool has_overflown();
		
		serial_port& enable();
		serial_port& disable();
		serial_port& reset();

		bool is_enabled();

	protected:
		aery::periph_idma idma;
		aery::periph_odma odma;

};

} /* end of namespace aery */

#endif

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

/*!
\file aery32/serial_port.h
\brief An RS-232 serial port connection
*/

#ifndef __AERY32_SERIAL_PORT_CLSDRV_H
#define __AERY32_SERIAL_PORT_CLSDRV_H

extern "C" {
	#include <avr32/io.h>
	#include <inttypes.h>
}
#include <cstdlib>

#include "usart.h"
#include "periph_idma_clsdrv.h"
#include "periph_odma_clsdrv.h"

namespace aery {

class serial_port {
	public:
		/**
		 * Pointer to the low level AVR32 USART register
		 */
		volatile avr32_usart_t *usart;

		/**
		 * Peripheral input DMA buffer object
		 */
		aery::periph_idma idma;

		/**
		 * Peripheral output DMA buffer object
		 */
		aery::periph_odma odma;

		/**
		 * Baud rate error.
		 */
		double error;

		/**
		 * Constructor
		 * \param usart pointer to the low level AVR32 USART register
		 * \param idma peripheral input DMA on which the character
		 *             receive is performed
		 * \param odma peripheral output DMA on which the character
		 *             transmission is performed
		 */
		serial_port(
			volatile avr32_usart_t *usart,
			aery::periph_idma &idma,
			aery::periph_odma &odma
		);

		/**
		 * Get character
		 * \return On success, the character read is returned
		 */
		int getc();

		/**
		 * Get a line from input DMA buffer into C string
		 *
		 * Extracts characters from input DMA buffer and stores them
		 * into str until the delimitation character delim is found.
		 * \param str C string where the extracted line is stored.
		 *            The generated string has a length of at most
		 *            n-1, leaving space for the additional
		 *            terminating null character.
		 * \param nread The total number of characters read. Delimiter
		 *              and '\0' aren't added to this value. OPTIONAL.
		 * \param delim The delimitation character. Can be either one
		 *              or two chars, e.g. '\n' or "\r\n". OPTIONAL.
		 * \return The same as parameter str
		 * \note The read process is limited to the size of the DMA
		 *       input buffer. The character that precedes the char
		 *       (del), decimal value 127, is discarded from the str.
		 */
		char* getline(char *str, size_t *nread, char delim);
		char* getline(char *str, size_t *nread, const char *delim);
		inline char* getline(char *str, size_t *nread) {
			if (delim_len == 2)
				return getline(str, nread, delim);
			return getline(str, nread, delim[0]);
		}
		inline char* getline(char *str) {
			size_t nread = 0;
			return getline(str, &nread);
		}

		/**
		 * Put character
		 * \param c char to be written
		 * \return Number of written chars
		 */
		int putc(char c);

		/**
		 * Write the C string to output DMA buffer
		 * \param str C string to be written
		 * \return the total number of characters written
		 */
		int puts(const char *str);

		/**
		 * Put character back
		 *
		 * Decrements the internal get pointer by one, and c becomes
		 * the character to be read at that position by the next
		 * get operation.
		 * \param c the character to be put back
		 * \return The same serial port class driver object
		 */
		serial_port& putback(char c);

		/**
		 * Print formatted data to output DMA buffer
		 * \param format C string that contains the text to be written.
		 * \return On success, the total number of characters written
		 *         is returned. If the output was truncated due to the
		 *         limited DMA buffer size then the return value is
		 *         the number of  characters which would have been
		 *         written if enough space had been available.
		 */
		int printf(const char *format, ... );

		/**
		 * Clears the input DMA buffer from all received characters
		 * \return The same serial port class driver object
		 */
		serial_port& flush();

		/**
		 * Indicates the number of bytes available in input DMA buffer
		 * \return the total number of bytes available to be read
		 */
		size_t bytes_available();

		/**
		 * Indicates whether input DMA buffer has been overflown
		 * \return True if input DMA buffer has been overflowm
		 */
		bool has_overflown();
		
		/**
		 * Enables the serial port class driver
		 * \return The same serial port class driver object
		 */
		serial_port& enable();

		/**
		 * Disables the serial port class driver
		 * \return The same serial port class driver object
		 */
		serial_port& disable();

		/**
		 * Resets the serial port class driver
		 * \return The same serial port class driver object
		 */
		serial_port& reset();

		/**
		 * Indicates whether the serial port class driver is enabled
		 * \return True if enabled
		 */
		bool is_enabled();

		/**
		 * Set serial port speed
		 * \param speed serial port speed (or baud rate)
		 * \return The same serial port class driver object
		 * \note PBA clock has been used as a source
		 */
		serial_port& set_speed(unsigned int speed);

		/**
		 * Set serial port data bits
		 * \param databits number of data bits per character
		 * \return The same serial port class driver object
		 */
		serial_port& set_databits(enum Usart_databits databits);

		/**
		 * Set parity
		 * \param parity number of parity bits
		 * \return The same serial port class driver object
		 */
		serial_port& set_parity(enum Usart_parity parity);

		/**
		 * Set stop bits
		 * \param stopbits number of stop bits
		 * \return The same serial port class driver object
		 */
		serial_port& set_stopbits(enum Usart_stopbits stopbits);

		/**
		 * Set default line delimitation character
		 * \param delim The delimitation character. Can be either one
		 *              or two chars, e.g. '\n' or "\r\n".
		 * \return The same serial port class driver object
		 */
		serial_port& set_default_delim(char delim);
		serial_port& set_default_delim(const char *delim);

		/**
		 * Enables hardware handshaking
		 * 
		 * When enabled the receiver drives the RTS pin and the level
		 * on the CTS pin modifies the behavior of the transmitter
		 */
		serial_port& enable_hw_handshaking();

		serial_port& operator<<(char);
		serial_port& operator<<(const char*);

		serial_port& operator<<(int);
		serial_port& operator<<(signed long);

		serial_port& operator<<(unsigned int);
		serial_port& operator<<(unsigned long);
		serial_port& operator<<(unsigned short);
		serial_port& operator<<(unsigned char);

	protected:
		serial_port& init();

		char delim[3];
		uint8_t delim_len;
};

} /* end of namespace aery */

#endif

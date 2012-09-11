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
\file aery32/twi.h
\brief TWI/I2C two wire serial interface 
*/

#ifndef __AERY32_TWI_H
#define __AERY32_TWI_H

#include <cstdio>
extern "C" {
	#include <inttypes.h>
	#include <avr32/io.h>
}

namespace aery {

/**
 * Pointer to the internal TWI module register
 */
extern volatile avr32_twi_t *twi;

/**
 * Last status read
 */
extern volatile uint32_t __twi_lsr;

/**
 * Initializes TWI as master
 *
 * The SLK is set 100 kHz by default. Use aery::twi_setup_clkwaveform() to
 * increase or decrease the SLK frequency.
 */
void twi_init_master(void);

/**
 * Setup TWI SLK waveform
 * \param ckdiv Clock divider
 * \param cldiv Clock low divider
 * \param chdiv Clock high divider
 * \return 0 on success, -1 on error
 *
 * For example, to set SLK to 100 kHz call
 * \code
 * aery::twi_setup_clkwaveform(4, 0x3f, 0x3f);
 * \endcode
 */
int twi_setup_clkwaveform(uint8_t ckdiv, uint8_t cldiv, uint8_t chdiv);

/**
 * Select slave
 * \param sla Slave address
 */
void twi_select_slave(uint16_t sla);

/**
 * Use device (slave's) internal address
 * \param iadr Device internal address. Means the slave's register.
 * \param n Address length in bytes: 1, 2 or 3 bytes. Zero clears the
 *          address and disables its usage.
 * \return 0 on success, -1 on error
 *
 * \note Once set the address will be used for every read/write opertaions
 * that follows calling this function. To revert this behavior call
 * aery::twi_clear_internal_address().
 */
int twi_use_internal_address(uint32_t iadr, uint8_t n);

/**
 * Clear device internal address
 *
 * The following read/write operations won't use the address anymore.
 */
void twi_clear_internal_address(void);

/**
 * Reads n number of bytes
 * \param data Pointer to data buffer
 * \param n Number of bytes to read
 * \return number of (successfully) read bytes
 */
size_t twi_read_nbytes(uint8_t *data, size_t n);

/**
 * Reads n number of bytes using the specific device internal address
 * \param data Pointer to space where to read the data
 * \param n Number of bytes to read
 * \param iadr Device internal address
 * \param iadrlen Device internal address length in bytes. Optional
 *                Default value is 1 that is 1 byte (8 bits).
 * \return number of (successfully) read bytes
 */
size_t twi_read_nbytes(uint8_t *data, size_t n, uint8_t iadr,
		uint8_t iadrlen = 1);

/**
 * Reads one byte
 * \param data Pointer to space where to read the data
 * \return number of (successfully) read bytes
 */
size_t twi_read_byte(uint8_t *data);

/**
 * Reads one byte using the specific device internal address
 * \param data Pointer to space where to read the data
 * \param iadr Device internal address. Slave's register.
 * \param iadrlen Device internal address length in bytes. Optional
 *                Default value is 1 that is 1 byte (8 bits).
 * \return number of (successfully) read bytes
 */
size_t twi_read_byte(uint8_t *data, uint8_t iadr, uint8_t iadrlen = 1);

/**
 * Writes n number of bytes
 * \param data Pointer to data buffer
 * \param n Number of bytes to write
 * \return number of (successfully) written bytes
 */
size_t twi_write_nbytes(uint8_t *data, size_t n);

/**
 * Writes n number of bytes using the specific device internal address
 * \param data Data to be written
 * \param n Number of bytes to write
 * \param iadr Device internal address. Slave's register.
 * \param iadrlen Device internal address length in bytes. Optional
 *                Default value is 1 that is 1 byte (8 bits).
 * \return number of (successfully) read bytes
 */
size_t twi_write_nbytes(uint8_t *data, size_t n, uint8_t iadr,
		uint8_t iadrlen = 1);

/**
 * Writes one byte
 * \param data Data to be written
 * \return number of (successfully) read bytes
 */
size_t twi_write_byte(uint8_t data);

/**
 * Writes one byte using the specific device internal address
 * \param data Data to be written
 * \param iadr Device internal address. Slave's register.
 * \param iadrlen Device internal address length in bytes. Optional
 *                Default value is 1 that is 1 byte (8 bits).
 * \return number of (successfully) read bytes
 */
size_t twi_write_byte(uint8_t data, uint8_t iadr, uint8_t iadrlen = 1);

/**
 * Tells if TWI bus is busy
 * \return True if busy, false if ready.
 */
bool twi_isbusy(void);

/**
 * Tells if the TWI has read/write operationr has been overrun
 * \param reread Rereads the status register. Optional. Default false.
 * \return True if has overrun. False if everything is ok.
 */
bool twi_has_overrun(bool reread = false);

} /* end of namespace */

#endif

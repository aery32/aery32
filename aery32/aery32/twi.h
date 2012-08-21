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

/* TWI error codes */
#define ETWI_WRITE_NACK -2
#define ETWI_READ_NACK  -3

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
 * The SLK is set 400 kHz by default. Use twi_setup_clkwaveform() to reset.
 */
void twi_init_master(void);

/**
 * Initializes TWI as slave
 * \param sla Slave address
 */
void twi_init_slave(uint16_t sla);

/**
 * Setup TWI SLK waveform
 * \param ckdiv Clock divider
 * \param cldiv Clock low divider
 * \param chdiv Clock high divider
 * \return 0 on success, -1 on error
 */
int twi_setup_clkwaveform(uint8_t ckdiv, uint8_t cldiv, uint8_t chdiv);

/**
 * Select slave
 * \param sla Slave address
 */
void twi_select_slave(uint16_t sla);

/**
 * Use device (slave's) internal address
 * \param iadr Device internal address
 * \param n    Address length. 1-3 bytes.
 * \return 0 on success, -1 on error
 *
 * Once set the address will be used for all of the following read/write
 * opertaions. Call twi_clear_internal_address() to clear.
 */
int twi_use_internal_address(uint32_t iadr, uint8_t n);

/**
 * Clear device internal address
 *
 * The following read/write operations won't use the address anymore.
 */
void twi_clear_internal_address(void);

/**
 * Read n pieces of bytes
 * \param data Pointer to data buffer
 * \param n    Number of bytes to read
 * \return number of read bytes
 */
size_t twi_read_nbytes(uint8_t *data, size_t n);
size_t twi_read_nbytes(uint8_t *data, size_t n, uint8_t iadr);

/**
 * Read one byte
 * \param data Pointer to data buffer
 * \return number of read bytes
 */
size_t twi_read_byte(uint8_t *data);
size_t twi_read_byte(uint8_t *data, uint8_t iadr);

/**
 * Write n pieces of bytes
 * \param data Pointer to data buffer
 * \param n    Number of bytes to read
 * \return number of written bytes
 */
size_t twi_write_nbytes(uint8_t *data, size_t n);
size_t twi_write_nbytes(uint8_t *data, size_t n, uint8_t iadr);

/**
 * Write one byte
 * \param data Data to be written
 * \return number of read bytes
 */
size_t twi_write_byte(uint8_t data);
size_t twi_write_byte(uint8_t data, uint8_t iadr);

/**
 * Tells if TWI bus is busy
 * \return True if busy. False if ready.
 */
bool twi_isbusy(void);

} /* end of namespace */

#endif

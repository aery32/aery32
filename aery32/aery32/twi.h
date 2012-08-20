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

void twi_init_master(void);
void twi_init_slave(uint16_t sla);

int twi_setup_clkwaveform(uint8_t ckdiv, uint8_t cldiv, uint8_t chdiv);

void twi_select_slave(uint16_t sla);

int twi_use_internal_address(uint32_t iadr, uint8_t n = 1);
void twi_clear_internal_address(void);

int twi_read_byte(void);
int twi_read_byte(uint8_t iadr);

int twi_write_byte(uint8_t data);
int twi_write_byte(uint8_t data, uint8_t iadr);

int twi_read_nbytes(uint8_t *buffer, uint8_t n);
int twi_write_nbytes(uint8_t *buffer, uint8_t n);

bool twi_isbusy(void);

} /* end of namespace */

#endif

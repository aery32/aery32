/*   _____             ___ ___   |
    |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
    |     | -_|  _| | |_  |  _|  |  
    |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
                  |___|          |

    Copyright (c) 2012, Muiku Oy
    All rights reserved.

    LICENSE: This source file is subject to the new BSD license that is
    bundled with this package in the file LICENSE.txt. If you did not
    receive a copy of the license and are unable to obtain it through
    the world-wide-web, please send an email to contact@muiku.com so
    we can send you a copy.
*/

/*!
\file aery32/spi.hh
\brief Serial Peripheral Interface (SPI) with aery namespace
\note C++ header file
*/

#ifndef __AERY32_SPI_HH
#define __AERY32_SPI_HH

#include "spi.h"

namespace aery {

static inline void
spi_init_master(volatile avr32_spi_t *pspi)
{
	aery_spi_init_master(pspi);
}

static inline void
spi_setup_chipselect(unsigned long csr, enum Spi_mode mode, uint8_t bits)
{
	aery_spi_setup_chipselect(csr, mode, bits);
}

static inline uint16_t
spi_transmit(volatile avr32_spi_t *pspi, uint16_t data, uint8_t csnum, bool islast)
{
	return aery_spi_transmit(pspi, data, csnum, islast);
}

static inline int
spi_txready(volatile avr32_spi_t *pspi)
{
	return aery_spi_txready(pspi);
}

static inline void
spi_enable(volatile avr32_spi_t *pspi)
{
	aery_spi_enable(pspi);
}

static inline void
spi_disable(volatile avr32_spi_t *pspi)
{
	aery_spi_disable(pspi);
}

}

#endif

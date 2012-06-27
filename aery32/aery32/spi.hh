/**
 * \file aery32/spi.hh
 * \brief Serial Peripheral Interface (SPI) with aery namespace
 * \note C++ header file
 *
 * \verbatim
 *  _____             ___ ___   |
 * |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
 * |     | -_|  _| | |_  |  _|  |  
 * |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
 *               |___|          |
 * 
 * Copyright (c) 2012, Muiku Oy
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 *    * Neither the name of Muiku Oy nor the names of its contributors may be
 *      used to endorse or promote products derived from this software without
 *      specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * \endverbatim
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

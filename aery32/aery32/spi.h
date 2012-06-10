/**
 * \file aery32/spi.h
 * \brief Serial Peripheral Interface (SPI)
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
 *
 * \example spi.c
 */

#ifndef __AERY32_SPI_H
#define __AERY32_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <inttypes.h>
#include <avr32/io.h>

#ifdef AERY_SHORTCUTS
	extern volatile avr32_spi_t *spi0;
	extern volatile avr32_spi_t *spi1;
#endif

/**
 * Spi mode
 */
enum Spi_mode { SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3 };

/**
 * Init SPI as a master
 *
 * \param pspi Pointer to the SPI peripheral which to init
 *
 * \par CS multiplexer
 * If you are using multiplexed chip selects, enable cs multiplexing
 * by bitbanging PCSDEC bit in SPI MR register after initialization:
 * \code
 * aery_spi_init_master(&AVR32_SPI0);
 * AVR32_SPI0.MR.pcsdec = 1;
 * \endcode
 */
void aery_spi_init_master(volatile avr32_spi_t*);

/**
 * Setup SPI mode and the shift register width of the Numeric
 * Processor Chip Select (NPCS, same as slave select). Every chip select
 * line can have different mode and size of shift register.
 *
 * \param pspi Pointer to the SPI peripheral
 * \param npcs Chip Select: 0-3
 * \param mode SPI mode
 * \param bits Width of SPI shift register: 8-16 bits
 *
 * \par Making SPI SCK faster
 * Chip select baudrate is hard coded to MCK/255. To make it faster
 * you can bitbang the SCRB bit in CSRX register.
 * \code
 * aery_spi_setup_npcs(&AVR32_SPI0, 0, SPI_MODE1, 16);
 * AVR32_SPI0.CSR0.scbr = 32; // baudrate is now MCK/32
 * \endcode
 */
void aery_spi_setup_npcs(volatile avr32_spi_t*, uint8_t, enum Spi_mode, uint8_t);

/**
 * Writes and reads SPI bus
 *
 * \param pspi   Pointer to the SPI peripheral which to use.
 * \param data   Binary word to be send.
 * \param npcs   Chip select line number. Can also be number of cs multiplexer.
 *               Use dummy cs number, if you like to use some other gpio pin
 *               through bitbanging.
 * \param islast Is this the last transmit? If no, set 0 to leave chip select
 *               low.
 * \return Received data bits
 *
 * \par SPI read
 * When you only want to read from external device through SPI, ignore the
 * sent data and use dummy bits instead, for example 0x00.
 * \code
 * uint16_t rd; // read data
 * rd = aery_spi_transmit(&AVR32_SPI0, 0, 0, true);
 * \endcode
 */
uint16_t aery_spi_transmit(volatile avr32_spi_t*, uint16_t, uint8_t, bool);

/**
 * Enable the SPI peripheral
 *
 * \param pspi Pointer to the SPI peripheral which to enable
 */
inline void aery_spi_enable(volatile avr32_spi_t *pspi)
{
	pspi->CR.spien = 1;
}

/**
 * Disable the SPI peripheral
 *
 * \param pspi Pointer to the SPI peripheral which to disable
 */
inline void aery_spi_disable(volatile avr32_spi_t *pspi)
{
	pspi->CR.spien = 0;
}

#ifdef __cplusplus
}
#endif

#endif

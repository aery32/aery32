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
\file aery32/spi.h
\brief Serial Peripheral Interface (SPI)
*/

#ifndef __AERY32_SPI_H
#define __AERY32_SPI_H

extern "C" {
	#include <inttypes.h>
	#include <avr32/io.h>
}

namespace aery {

/**
 * Pointer to the internal Serial peripheral interdace module register 0
 */
extern volatile avr32_spi_t *spi0;

/**
 * Pointer to the internal Serial peripheral interdace module register 1
 */
extern volatile avr32_spi_t *spi1;

/**
 * SPI mode
 */
enum Spi_mode { SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3 };

/**
 * Init SPI as a master
 * \param pspi Pointer to the SPI peripheral which to init
 *
 * \par CS multiplexer
 * If you are using multiplexed chip selects, enable cs multiplexing
 * by bitbanging PCSDEC bit in SPI MR register after initialization:
 *
 * \code
 * spi_init_master(&AVR32_SPI0);
 * spi0->MR.pcsdec = 1;
 * \endcode
 */
void spi_init_master(volatile avr32_spi_t *pspi);

/**
 * Setups the SPI mode and the shift register width of the Numeric
 * Processor Chip Select (NPCS, same as slave/chip select)
 * \param pspi Pointer to the SPI peripheral
 * \param npcs Chip Select: 0-3
 * \param mode SPI mode
 * \param bits Width of SPI shift register: 8-16 bits
 *
 * \par Making SPI SCK faster
 * Chip select baudrate is hard coded to MCK/255. To make it faster
 * you can bitbang the SCRB bit in CSRX register.
 *
 * \code
 * spi_setup_npcs(&AVR32_SPI0, 0, SPI_MODE1, 16);
 * spi0->CSR0.scbr = 32; // baudrate is now MCK/32
 * \endcode
 *
 * \note Every chip select line can have different mode and size of shift
 *       register.
 */
void spi_setup_npcs(volatile avr32_spi_t *pspi, uint8_t npcs,
		enum Spi_mode mode, uint8_t bits);

/**
 * Writes and reads SPI bus
 * \param pspi   Pointer to the SPI peripheral which to use.
 * \param npcs   Chip select line number. Can also be number of cs multiplexer.
 *               Use dummy cs number, if you like to use some other gpio pin
 *               through bitbanging.
 * \param data   Binary word to be send.
 * \param islast Is this the last transmit? If no, set 0 to leave chip select
 *               low.
 * \return Received data bits
 *
 * \par SPI read
 * When you only want to read from external device through SPI, ignore the
 * sent data and use dummy bits instead, for example 0x00.
 *
 * \code
 * uint16_t rd; // read data
 * rd = spi_transmit(spi0, 0, 0x00);
 * \endcode
 */
uint16_t spi_transmit(volatile avr32_spi_t *pspi, uint8_t npcs,
		 uint16_t data, bool islast = true);

/**
 * Enables the SPI peripheral
 * \param pspi Pointer to the SPI peripheral which to enable
 */
void spi_enable(volatile avr32_spi_t *pspi);

/**
 * Disables the SPI peripheral
 * \param pspi Pointer to the SPI peripheral which to disable
 */
void spi_disable(volatile avr32_spi_t *pspi);

/**
 * Tells if the SPI is busy
 * \param pspi Pointer to the SPI peripheral which to check
 * \return True if busy. False if ready.
 */
bool spi_isbusy(volatile avr32_spi_t *pspi);

/**
 * Tells if the SPI has been enabled
 * \param pspi Pointer to the SPI peripheral which to check
 * \return True if enabled. False if not enabled.
 */
bool spi_is_enabled(volatile avr32_spi_t *pspi);

/**
 * Tells if the SPI has data to be read
 * \param pspi Pointer to the SPI peripheral which to check
 * \param reread Rereads the status register. Optional. Default true.
 * \return True if has data. False if doesn't hava data.
 */
bool spi_has_rxdata(volatile avr32_spi_t *pspi, bool reread = true);

/**
 * Tells if the SPI has read register has been overrun
 * \param pspi Pointer to the SPI peripheral which to check
 * \param reread Rereads the status register. Optional. Default false.
 * \return True if has overrun. False if everything is ok.
 */
bool spi_has_overrun(volatile avr32_spi_t *pspi, bool reread = false);

} /* end of namespace */

#endif

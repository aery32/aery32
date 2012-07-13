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

#include "aery32/spi.h"

volatile avr32_spi_t *aery::spi0 = &AVR32_SPI0;
volatile avr32_spi_t *aery::spi1 = &AVR32_SPI1;

void aery::spi_init_master(volatile avr32_spi_t *pspi)
{
	/* Software reset before intialization */
	pspi->CR.swrst = 1;
	while (pspi->CR.swrst);

	pspi->mr =
		(6 << AVR32_SPI_MR_DLYBCS_OFFSET) |
		(1 << AVR32_SPI_MR_MODFDIS_OFFSET) |
		(1 << AVR32_SPI_MR_PS_OFFSET) |
		(1 << AVR32_SPI_MR_MSTR_OFFSET);
}

void aery::spi_setup_npcs(volatile avr32_spi_t *pspi, uint8_t npcs,
		enum Spi_mode mode, uint8_t bits)
{
	if (bits < 8 || bits > 16)
		bits = 8;

	/*
	 * This not elegant, but there is no other way to do this without
	 * modifying the AVR32 header files.
	 */
	switch (npcs) {
	case 0:
		pspi->csr0 =
			((mode & 1) << AVR32_SPI_CSR0_CPOL_OFFSET) |
			((mode & 2) << AVR32_SPI_CSR0_NCPHA_OFFSET) |
			((bits - 8) << AVR32_SPI_CSR0_BITS_OFFSET) |
			(1 << AVR32_SPI_CSR0_CSAAT_OFFSET) |
		 	(255 << AVR32_SPI_CSR0_SCBR_OFFSET) |
			(1 << AVR32_SPI_CSR0_DLYBCT_OFFSET);
		break;
	case 1:
		pspi->csr1 =
			((mode & 1) << AVR32_SPI_CSR1_CPOL_OFFSET) |
			((mode & 2) << AVR32_SPI_CSR1_NCPHA_OFFSET) |
			((bits - 8) << AVR32_SPI_CSR1_BITS_OFFSET) |
			(1 << AVR32_SPI_CSR1_CSAAT_OFFSET) |
		 	(255 << AVR32_SPI_CSR1_SCBR_OFFSET) |
			(1 << AVR32_SPI_CSR1_DLYBCT_OFFSET);
		break;
	case 2:
		pspi->csr2 =
			((mode & 1) << AVR32_SPI_CSR2_CPOL_OFFSET) |
			((mode & 2) << AVR32_SPI_CSR2_NCPHA_OFFSET) |
			((bits-8) << AVR32_SPI_CSR2_BITS_OFFSET) |
			(1 << AVR32_SPI_CSR2_CSAAT_OFFSET) |
		 	(255 << AVR32_SPI_CSR2_SCBR_OFFSET) |
			(1 << AVR32_SPI_CSR2_DLYBCT_OFFSET);
		break;
	case 3:
		pspi->csr3 =
			((mode & 1) << AVR32_SPI_CSR3_CPOL_OFFSET) |
			((mode & 2) << AVR32_SPI_CSR3_NCPHA_OFFSET) |
			((bits - 8) << AVR32_SPI_CSR3_BITS_OFFSET) |
			(1 << AVR32_SPI_CSR3_CSAAT_OFFSET) |
		 	(255 << AVR32_SPI_CSR3_SCBR_OFFSET) |
			(1 << AVR32_SPI_CSR3_DLYBCT_OFFSET);
		break;
	}
}

uint16_t aery::spi_transmit(volatile avr32_spi_t *pspi, uint8_t npcs,
		uint16_t data, bool islast)
{
	/* Wait previous transfer to complete */
	while ((pspi->sr & AVR32_SPI_SR_TXEMPTY_MASK) == 0);

	/*
	 * Setup chip select bits. If PCSDEC = 1 then PCS == npcs, otherwise
	 * map npcs nubmer to the corresponding PCS as it is defined in the
	 * datasheet p. 209 of UC3A1/0
	 */
	if ((pspi->mr & AVR32_SPI_MR_PCSDEC_MASK) == 0) {
		switch (npcs) {
		case 2:
			npcs = 3;
			break;
		case 3:
			npcs = 7;
			break;
		}
	}
		
	/* Begin transmit */
	pspi->tdr =
		(npcs << AVR32_SPI_TDR_PCS_OFFSET) |
		(data << AVR32_SPI_TDR_TD_OFFSET) |			   
		(islast << AVR32_SPI_TDR_LASTXFER_OFFSET);
	
	/* When accessible return the read data */
	while ((pspi->sr & AVR32_SPI_SR_RDRF_MASK) == 0);
	return pspi->RDR.rd;
}

void aery::spi_enable(volatile avr32_spi_t *pspi)
{
	pspi->CR.spien = 1;
}

void aery::spi_disable(volatile avr32_spi_t *pspi)
{
	pspi->CR.spien = 0;
}
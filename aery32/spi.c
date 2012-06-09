/**
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
 */

#include <stdbool.h>
#include <inttypes.h>
#include <avr32/io.h>
#include "spi.h"

#ifdef AERY_SHORTCUTS
	volatile avr32_spi_t *spi0 = &AVR32_SPI0;
	volatile avr32_spi_t *spi1 = &AVR32_SPI1;
#endif

void
aery_spi_init_master(volatile avr32_spi_t *pspi)
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

void
aery_spi_setup_npcs(volatile avr32_spi_t *pspi, uint8_t npcs, enum Spi_mode mode,
                    uint8_t bits)
{
	if (bits < 8 || bits > 16) {
		bits = 8;
	}

	/* This not elegant, but there is no other way to do this without modifying
	 * avr32 header files. */
	switch (npcs) {
	case 0:
		pspi->csr0 =
			((mode & 1) << AVR32_SPI_CSR0_CPOL_OFFSET) |
			((mode & 2) << AVR32_SPI_CSR0_NCPHA_OFFSET) |
			((bits-8) << AVR32_SPI_CSR0_BITS_OFFSET) |
			(1 << AVR32_SPI_CSR0_CSAAT_OFFSET) |
		 	(255 << AVR32_SPI_CSR0_SCBR_OFFSET) |
			(1 << AVR32_SPI_CSR0_DLYBCT_OFFSET);
		break;
	case 1:
		pspi->csr1 =
			((mode & 1) << AVR32_SPI_CSR1_CPOL_OFFSET) |
			((mode & 2) << AVR32_SPI_CSR1_NCPHA_OFFSET) |
			((bits-8) << AVR32_SPI_CSR1_BITS_OFFSET) |
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
			((bits-8) << AVR32_SPI_CSR3_BITS_OFFSET) |
			(1 << AVR32_SPI_CSR3_CSAAT_OFFSET) |
		 	(255 << AVR32_SPI_CSR3_SCBR_OFFSET) |
			(1 << AVR32_SPI_CSR3_DLYBCT_OFFSET);
		break;
	}
}

uint16_t
aery_spi_transmit(volatile avr32_spi_t *pspi, uint16_t data, uint8_t npcs,
                  bool islast)
{
	/* Wait previous transfer to complete */
	while ((pspi->sr & AVR32_SPI_SR_TXEMPTY_MASK) == 0);

	/* Setup chip select bits. If PCSDEC = 1 then PCS == npcs, otherwise
	 * map npcs numer 0, 1, 2 or 3 to corresponding PCS as defined in
	 * datasheet p. 209 */
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

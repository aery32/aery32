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
\file aery32/periph_idma.h
\brief Input DMA
*/

#ifndef __AERY32_PERIPH_IDMA_CLSDRV_H
#define __AERY32_PERIPH_IDMA_CLSDRV_H

extern "C" {
	#include <avr32/io.h>
	#include <inttypes.h>
}
#include <cstdlib>

namespace aery {

class periph_idma {

	public:
		periph_idma(int dma_chnum, int dma_pid,
			volatile uint8_t *buf, size_t n);
		periph_idma(int dma_chnum, int dma_pid,
			volatile uint16_t *buf, size_t n);
		periph_idma(int dma_chnum, int dma_pid,
			volatile uint32_t *buf, size_t n);

		size_t read(uint8_t *dest, size_t n);
		size_t read(uint16_t *dest, size_t n);
		size_t read(uint32_t *dest, size_t n);

		periph_idma& flush();

		size_t bytes_available();
		bool has_overflown();
		
		periph_idma& enable();
		periph_idma& disable();
		periph_idma& reset();

		bool is_enabled();

		volatile avr32_pdca_channel_t *dma;
		size_t dma_tcrv;

		volatile uint8_t *buffer;
		size_t bufsize;
		size_t idx;

	protected:
		periph_idma& init();
};

} /* end of namespace aery */

#endif

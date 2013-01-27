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
\file aery32/idma.h
\brief Input DMA
*/

#ifndef __AERY32_IDMA_H
#define __AERY32_IDMA_H

extern "C" {
	#include <avr32/io.h>
	#include <inttypes.h>
}
#include <cstdio>

namespace aery {

class idma {

	public:
		idma(int dma_chnum, int dma_pid,
			volatile uint8_t *buf, size_t n);
		idma(int dma_chnum, int dma_pid,
			volatile uint16_t *buf, size_t n);
		idma(int dma_chnum, int dma_pid,
			volatile uint32_t *buf, size_t n);

		idma& enable();
		idma& disable();
		bool is_enabled();

		idma& read(uint8_t *dest, size_t n);
		idma& read(uint16_t *dest, size_t n);
		idma& read(uint32_t *dest, size_t n);
		
		idma& flush();
		idma& reset();

		size_t bytes_available();
		bool has_overflown();

	protected:
		volatile avr32_pdca_channel_t *dma;
		volatile uint8_t *buffer;
		size_t bufsize;
		size_t r_idx;

		idma& init();
};

} /* end of namespace aery */

#endif

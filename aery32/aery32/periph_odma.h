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
\file aery32/periph_odma.h
\brief Output DMA
*/

#ifndef __AERY32_PERIPH_ODMA_H
#define __AERY32_PERIPH_ODMA_H

extern "C" {
	#include <avr32/io.h>
	#include <inttypes.h>
}
#include <cstdlib>

namespace aery {

class periph_odma {

	public:
		periph_odma(int dma_chnum, int dma_pid,
			volatile uint8_t *buf, size_t n);
		periph_odma(int dma_chnum, int dma_pid,
			volatile uint16_t *buf, size_t n);
		periph_odma(int dma_chnum, int dma_pid,
			volatile uint32_t *buf, size_t n);

		periph_odma& enable();
		periph_odma& disable();
		periph_odma& reset();

		periph_odma& write(uint8_t *dest, size_t n);
		periph_odma& write(uint16_t *dest, size_t n);
		periph_odma& write(uint32_t *dest, size_t n);

		periph_odma& write(uint8_t byte);
		periph_odma& write(uint16_t halfword);
		periph_odma& write(uint32_t word);
		
		periph_odma& flush();

		size_t bytes_in_buffer();
		size_t bytes_in_progress();

		bool is_enabled();

	protected:
		volatile avr32_pdca_channel_t *dma;
		volatile uint8_t *buffer;
		size_t bufsize;
		size_t w_idx;

		periph_odma& init();
};

} /* end of namespace aery */

#endif

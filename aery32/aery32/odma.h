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
\file aery32/odma.h
\brief Output DMA
*/

#ifndef __AERY32_ODMA_H
#define __AERY32_ODMA_H

extern "C" {
	#include <avr32/io.h>
	#include <inttypes.h>
}

#include <cstdio>
#include "dma.h"

namespace aery {

class odma {
	public:
		odma(int dma_chnum, int dma_pid, enum dma_datasize dma_size,
			void *buf, uint16_t bufsize);

		odma& enable();
		odma& disable();
		odma& is_enabled();

		odma& write(uint8_t *src, size_t n);
		odma& write(uint16_t *src, size_t n);
		odma& write(uint32_t *src, size_t n);

		odma& flush();
		odma& reset();

		//int status();
		bool has_overflown();

	protected:
		volatile avr32_pdca_channel_t *dma;

		uint8_t *buffer;
		size_t bufsize;
};

} /* end of namespace aery */

#endif

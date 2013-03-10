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

#include "aery32/periph_idma_clsdrv.h"

using namespace aery;

periph_idma::periph_idma(int chnum, int pid, volatile void *buf,
	size_t bufsize) : buffer((uint8_t*) buf), bufsize(bufsize)
{
	dma = &AVR32_PDCA.channel[chnum];
	dma->PSR.pid = pid;
	dma->MR.size = 0;

	dma_tcrv = bufsize;
	init();
}

periph_idma& periph_idma::init()
{
	idx = 0;
	dma->CR.eclr = true;

	dma->TCR.tcv = dma_tcrv;
	dma->TCRR.tcrv = dma_tcrv;
	dma->mar = (uint32_t) buffer;
	dma->marr = (uint32_t) buffer;

	return *this;
}

periph_idma& periph_idma::set_sizeof_transfer(enum Pdca_transfer_size size)
{
	dma->MR.size = size;
	dma_tcrv = bufsize / (1 << size);
	return init();
}


periph_idma& periph_idma::enable()
{
	dma->CR.ten = true;
	return *this;
}

periph_idma& periph_idma::disable()
{
	dma->CR.tdis = true;
	return *this;
}

bool periph_idma::is_enabled()
{
	return dma->SR.ten;
}

size_t periph_idma::read(uint8_t *dest, size_t n)
{
	size_t i = 0;
	size_t bytes_available = this->bytes_available();

	if (!bytes_available)
		return 0;

	if (n > bytes_available)
		n = bytes_available;

	for (; i < n; i++) {
		dest[i] = buffer[idx++];
		if (idx == bufsize) {
			idx = 0;
			dma->marr = (uint32_t) buffer;
			dma->TCRR.tcrv = dma_tcrv;
		}
	}

	return i;
}

size_t periph_idma::read(uint16_t *dest, size_t n)
{
	return read((uint8_t*) dest, n * sizeof(uint16_t)) / sizeof(uint16_t);
}

size_t periph_idma::read(uint32_t *dest, size_t n)
{
	return read((uint8_t*) dest, n * sizeof(uint32_t)) / sizeof(uint32_t);
}

periph_idma& periph_idma::flush()
{
	uint8_t dev_null[bytes_available()];
	read(dev_null, sizeof(dev_null));
	return *this;
}

periph_idma& periph_idma::reset()
{
	bool was_enabled = is_enabled();
	disable().init();

	if (was_enabled == true)
		enable();
	return *this;
}

size_t periph_idma::bytes_available()
{
	if (has_overflown())
		return 0;

	size_t rv = bufsize - idx;
	if (dma->TCRR.tcrv == 0)
		rv += bufsize;
 	return rv - ((1 << dma->MR.size) * dma->TCR.tcv);
}

bool periph_idma::has_overflown()
{
	if (dma->TCRR.tcrv != 0)
		return false;

	if (idx < (bufsize - (1 << dma->MR.size) * dma->TCR.tcv))
		return true;

	return false;
}
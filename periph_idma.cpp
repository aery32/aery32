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

#include "aery32/idma.h"

using namespace aery;

idma::idma(int dma_chnum, int dma_pid, volatile uint8_t *buf, size_t n)
{
	dma = &AVR32_PDCA.channel[dma_chnum];
	dma->PSR.pid = dma_pid;
	dma->MR.size = 0;

	buffer = buf;
	bufsize = n;
	dma_tcrv = n;

	init();
}

idma::idma(int dma_chnum, int dma_pid, volatile uint16_t *buf, size_t n)
{
	dma = &AVR32_PDCA.channel[dma_chnum];
	dma->PSR.pid = dma_pid;
	dma->MR.size = 1;

	buffer = (uint8_t*) buf;
	bufsize = n * sizeof(uint16_t);
	dma_tcrv = n;

	init();
}

idma::idma(int dma_chnum, int dma_pid, volatile uint32_t *buf, size_t n)
{
	dma = &AVR32_PDCA.channel[dma_chnum];
	dma->PSR.pid = dma_pid;
	dma->MR.size = 2;

	buffer = (uint8_t*) buf;
	bufsize = n * sizeof(uint32_t);
	dma_tcrv = n;

	init();
}

idma& idma::init()
{
	dma->CR.eclr = true;

	dma->TCR.tcv = dma_tcrv;
	dma->TCRR.tcrv = dma_tcrv;
	dma->mar = (uint32_t) buffer;
	dma->marr = (uint32_t) buffer;

	return *this;
}

idma& idma::enable()
{
	dma->CR.ten = true;
	return *this;
}

idma& idma::disable()
{
	dma->CR.tdis = true;
	return *this;
}

bool idma::is_enabled()
{
	return dma->SR.ten;
}

idma& idma::read(uint8_t *dest, size_t n)
{
	size_t bytes_available = this->bytes_available();

	if (n > bytes_available)
		n = bytes_available;

	for (size_t i = 0; i < n; i++) {
		dest[i] = buffer[r_idx++];
		if (r_idx == bufsize) {
			r_idx = 0;
			dma->marr = (uint32_t) buffer;
			dma->TCRR.tcrv = dma_tcrv;
		}
	}

	return *this;
}

idma& idma::read(uint16_t *dest, size_t n)
{
	read((uint8_t*) dest, n * sizeof(uint16_t));
	return *this;
}

idma& idma::read(uint32_t *dest, size_t n)
{
	read((uint8_t*) dest, n * sizeof(uint32_t));
	return *this;
}

idma& idma::flush()
{
	uint8_t dev_null[bytes_available()];
	read(dev_null, sizeof(dev_null));
	return *this;
}

idma& idma::reset()
{
	bool was_enabled = is_enabled();
	disable().init();

	if (was_enabled == true)
		enable();
	return *this;
}

size_t idma::bytes_available()
{
	if (has_overflown())
		return 0;

	size_t rv = bufsize - r_idx;
	if (dma->TCRR.tcrv == 0)
		rv += bufsize;
 	return rv - ((1 << dma->MR.size) * dma->TCR.tcv);
}

bool idma::has_overflown()
{
	if (dma->TCRR.tcrv != 0)
		return false;

	if (r_idx < (bufsize - (1 << dma->MR.size) * dma->TCR.tcv))
		return true;

	return false;
}
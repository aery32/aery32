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

idma::idma(int dma_chnum, int dma_pid, volatile uint8_t *buf, size_t size)
{
	dma = &AVR32_PDCA.channel[dma_chnum];
	dma->PSR.pid = dma_pid;
	dma->MR.size = 0;

	buffer = buf;
	bufsize = size;
	bufsize2 = bufsize / 2;

	init();
}

idma::idma(int dma_chnum, int dma_pid, volatile uint16_t *buf, size_t size)
{
	dma = &AVR32_PDCA.channel[dma_chnum];
	dma->PSR.pid = dma_pid;
	dma->MR.size = 1;

	buffer = (uint8_t*) buf;
	bufsize = size * sizeof(uint16_t);
	bufsize2 = bufsize / 2;

	init();
}

idma::idma(int dma_chnum, int dma_pid, volatile uint32_t *buf, size_t size)
{
	dma = &AVR32_PDCA.channel[dma_chnum];
	dma->PSR.pid = dma_pid;
	dma->MR.size = 2;

	buffer = (uint8_t*) buf;
	bufsize = size * sizeof(uint32_t);
	bufsize2 = bufsize / 2;

	init();
}

idma& idma::init()
{
	dma->CR.eclr = true;

	dma->mar = (uint32_t) buffer;
	dma->marr = (uint32_t) (buffer + bufsize2);
	dma->TCR.tcv = bufsize2;
	dma->TCRR.tcrv = bufsize2;

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

idma& idma::read(uint16_t *dest, size_t n)
{
	// remember to handle extra bytes that do not make a uint16_t gracefully
	read((uint8_t*) dest, n * 2);
	return *this;
}

idma& idma::read(uint32_t *dest, size_t n)
{
	// remember to handle extra bytes that do not make a uint32_t gracefully
	read((uint8_t*) dest, n * 4);
	return *this;
}

idma& idma::read(uint8_t *dest, size_t n)
{
	size_t bytes_availabl = bytes_available();

	if (n > bytes_availabl)
		n = bytes_availabl;

	for (int i = 0; i < n; i++) {
		dest[i] = buffer[r_idx++];
		if (r_idx == bufsize2) {
			dma->marr = (uint32_t) buffer;
			dma->TCRR.tcrv = bufsize2;
		}
		if (r_idx == bufsize) {
			r_idx = 0;
			dma->marr = (uint32_t) (buffer + bufsize2);
			dma->TCRR.tcrv = bufsize2;
		}
	}

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
	return bufsize - r_idx - (dma->TCR.tcv + dma->TCRR.tcrv);
}

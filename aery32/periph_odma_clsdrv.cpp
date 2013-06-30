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

#include "aery32/periph_odma_clsdrv.h"

using namespace aery;

periph_odma::periph_odma(int chnum, int pid, volatile void *buf,
	size_t bufsize) : buffer((uint8_t*) buf), bufsize(bufsize)
{
	dma = &AVR32_PDCA.channel[chnum];
	dma->PSR.pid = pid;
	dma->MR.size = 0;

	init();
}

periph_odma& periph_odma::init()
{
	idx = 0;
	dma->CR.eclr = true;
	return *this;
}

periph_odma& periph_odma::enable()
{
	dma->CR.ten = true;
	return *this;
}

periph_odma& periph_odma::disable()
{
	dma->CR.tdis = true;
	return *this;
}

periph_odma& periph_odma::write(uint8_t *src, size_t n)
{
	for (size_t i = 0; i < n; i++) {
		buffer[idx++] = src[i];
		if (idx == bufsize) {
			flush();
			while (bytes_in_progress());				
		}
	}
	return *this;
}

periph_odma& periph_odma::write(uint16_t *src, size_t n)
{
	write((uint8_t*) src, n * sizeof(uint16_t));
	return *this;
}

periph_odma& periph_odma::write(uint32_t *src, size_t n)
{
	write((uint8_t*) src, n * sizeof(uint32_t));
	return *this;
}

periph_odma& periph_odma::flush()
{
	while (bytes_in_progress());
	dma->mar = (uint32_t) buffer;
	dma->TCR.tcv = idx / (1 << dma->MR.size);
	idx = 0;
	return *this;
}

periph_odma& periph_odma::set_sizeof_transfer(enum Pdca_transfer_size size)
{
	dma->MR.size = size;
	return *this;
}

periph_odma& periph_odma::reset()
{
	bool was_enabled = is_enabled();
	disable().init();

	if (was_enabled == true)
		enable();
	return *this;
}

size_t periph_odma::bytes_in_buffer()
{
	return idx;
}

size_t periph_odma::bytes_in_progress()
{
	return dma->TCR.tcv;
}

bool periph_odma::is_enabled()
{
	return dma->SR.ten;
}

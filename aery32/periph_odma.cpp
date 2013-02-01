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

#include "aery32/periph_odma.h"

using namespace aery;

periph_odma::periph_odma(int dma_chnum, int dma_pid, volatile uint8_t *buf, size_t n)
{
	dma = &AVR32_PDCA.channel[dma_chnum];
	dma->PSR.pid = dma_pid;
	dma->MR.size = 0;

	buffer = buf;
	bufsize = n;

	init();
}

periph_odma::periph_odma(int dma_chnum, int dma_pid, volatile uint16_t *buf, size_t n)
{
	dma = &AVR32_PDCA.channel[dma_chnum];
	dma->PSR.pid = dma_pid;
	dma->MR.size = 1;

	buffer = (uint8_t*) buf;
	bufsize = n * sizeof(uint16_t);

	init();
}

periph_odma::periph_odma(int dma_chnum, int dma_pid, volatile uint32_t *buf, size_t n)
{
	dma = &AVR32_PDCA.channel[dma_chnum];
	dma->PSR.pid = dma_pid;
	dma->MR.size = 2;

	buffer = (uint8_t*) buf;
	bufsize = n * sizeof(uint32_t);

	init();
}

periph_odma& periph_odma::init()
{
	dma->CR.eclr = true;
	dma->mar = (uint32_t) buffer;
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

periph_odma& periph_odma::write(uint8_t byte)
{
	write(&byte, 1);
	return *this;
}

periph_odma& periph_odma::write(uint16_t halfword)
{
	write(&halfword, 1);
	return *this;
}

periph_odma& periph_odma::write(uint32_t word)
{
	write(&word, 1);
	return *this;
}

periph_odma& periph_odma::write(uint8_t *src, size_t n)
{
	for (size_t i = 0; i < n; i++) {
		buffer[w_idx++] = src[i];
		if (w_idx == bufsize) {
			flush();
			while (bytes_in_progress() < i);				
			w_idx = 0;
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
	dma->TCR.tcv = bufsize / (1 << dma->MR.size);
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
	return w_idx;
}

size_t periph_odma::bytes_in_progress()
{
	return dma->TCR.tcv;
}

bool periph_odma::is_enabled()
{
	return dma->SR.ten;
}

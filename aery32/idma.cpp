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

idma::idma(int dma_chnum, int dma_pid, enum dma_datasize dma_size,
	void *buf, uint16_t bufsize)
{
	this->dma = &AVR32_PDCA.channel[dma_chnum];
	this->dma->PSR.pid = dma_pid;
	this->dma->MR.size = dma_size;

	this->buffer = (uint8_t*) buf;
	switch (dma_size) {
	case DMA_DATA_SIZE_BYTE:
		this->bufsize = bufsize;
		break;
	case DMA_DATA_SIZE_HALFWORD:
		this->bufsize = bufsize * sizeof(uint16_t);
		break;
	case DMA_DATA_SIZE_WORD:
		this->bufsize = bufsize * sizeof(uint32_t);
		break;
	}
	this->bufsize2 = this->bufsize / 2;

	this->reset();
}

idma& idma::enable()
{
	this->dma->CR.ten = true;
	return *this;
}

idma& idma::disable()
{
	this->dma->CR.tdis = true;
	return *this;
}

bool idma::is_enabled()
{
	return this->dma->SR.ten;
}

idma& idma::read(uint16_t *dest, size_t n)
{
	// remember to handle extra bytes that do not make a uint16_t gracefully
	this->read((uint8_t*) dest, n * 2);
	return *this;
}

idma& idma::read(uint32_t *dest, size_t n)
{
	// remember to handle extra bytes that do not make a uint32_t gracefully
	this->read((uint8_t*) dest, n * 4);
	return *this;
}

idma& idma::read(uint8_t *dest, size_t n)
{
	if (n > this->bytes_available())
		n = this->bytes_available();

	for (int i = 0; i < n; i++) {
		dest[i] = this->buffer[this->r_idx++];
		if (this->r_idx == this->bufsize) {
			this->r_idx = 0;
			this->dma->marr = (uint32_t) (this->buffer + this->bufsize2);
			this->dma->tcrr = this->bufsize2;
		}
		if (this->r_idx == this->bufsize2) {
			this->dma->marr = (uint32_t) this->buffer;
			this->dma->tcrr = this->bufsize2;
		}
	}

	return *this;
}

idma& idma::flush()
{
	int bytes_available = this->bytes_available();
	uint8_t dev_null[bytes_available];
	this->read(dev_null, bytes_available);
	return *this;
}

idma& idma::reset()
{
	bool was_enabled = this->is_enabled();

	this->disable();
	this->dma->CR.eclr = true;

	this->dma->mar = (uint32_t) this->buffer;
	this->dma->marr = (uint32_t) (this->buffer + this->bufsize2);
	this->dma->tcr = this->bufsize2;
	this->dma->tcrr = this->bufsize2;
	
	if (was_enabled == true)
		this->enable();

	return *this;
}

size_t idma::bytes_available()
{

	return 0;
}


// idma& idma::read(void *dest, size_t n, size_t *count)
// {
// 	dest = static_cast<char*> (dest);




	// if ((this->r_ptr + n) > this->buffersize) {
	// 	ntail = this->buffersize - this->r_ptr;
	// 	nhead = n - ntail;
	// } else {
	// 	ntail = n;
	// }

	// switch (this->data_size) {
	// case DMA_DATA_SIZE_8_BITS: {
	// 	uint8_t *source = ((uint8_t*) this->buffer) + this->r_ptr;
	// 	memcpy(dest, source, ntail * 8);
	// 	memcpy((uint8_t*) dest + ntail, this->buffer, nhead * 8);
	// 	break;
	// }
	// case DMA_DATA_SIZE_16_BITS: {
	// 	uint16_t *source = ((uint16_t*) this->buffer) + this->r_ptr;
	// 	memcpy(dest, source, ntail * 16);
	// 	memcpy((uint16_t*) dest + ntail, this->buffer, nhead * 16);
	// 	break;
	// }
	// case DMA_DATA_SIZE_32_BITS: {
	// 	uint32_t *source = ((uint32_t*) this->buffer) + this->r_ptr;
	// 	memcpy(dest, source, ntail * 32);
	// 	memcpy((uint32_t*) dest + ntail, this->buffer, nhead * 32);
	// 	break;
	// }
	// }

	// if (nhead > 0) {
	// 	this->dma.marr = 1;
	// }

// 	return *this;

// }

// idma& idma::read_bytes(void *dest, size_t num)
// {
// 	memcpy(dest, this->r_ptr, num);
// 	this->r_ptr = (void*) ((char*) this->r_ptr + num);
// 	if (this->r_idx() == this->buffersize)
// 		this->r_ptr = this->buffer;
// 	return *this;
// }

// int32_t idma::bytes_available()
// {
// 	int64_t n = 0;

// 	if (this->data_size == 1)
// 		n = (uint8_t*) this->dma.mar - (uint8_t*) this->r_ptr;
// 	if (this->data_size == 2)
// 		n = (uint16_t*) this->dma.mar - (uint16_t*) this->r_ptr;
// 	if (this->data_size == 4)
// 		n = (uint32_t*) this->dma.mar - (uint32_t*) this->r_ptr;

// 	if (n < 0)
// 		return this->buffersize + n;
// 	return n;
// }

// size_t idma::r_idx()
// {
// 	if (this->data_size == 1)
// 		return (uint8_t*) this->r_ptr - (uint8_t*) this->buffer;
// 	if (this->data_size == 2)
// 		return (uint16_t*) this->r_ptr - (uint16_t*) this->buffer;
// 	return (uint32_t*) this->r_ptr - (uint32_t*) this->buffer;
// }
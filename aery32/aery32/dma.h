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
\file aery32/dma.h
\brief DMA
*/

#ifndef __AERY32_DMA_H
#define __AERY32_DMA_H

namespace aery {

enum dma_datasize {
	DMA_DATA_SIZE_BYTE,
	DMA_DATA_SIZE_HALFWORD,
	DMA_DATA_SIZE_WORD
};

} /* end of namespace aery */

#endif

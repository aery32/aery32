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

#ifndef __AERY32_PDCA_H
#define __AERY32_PDCA_H

namespace aery {

enum Pdca_transfer_size {
	PDCA_TRANSFER_SIZE_BYTE,
	PDCA_TRANSFER_SIZE_HALFWORD,
	PDCA_TRANSFER_SIZE_WORD
};

} /* end of namespace aery */

#endif

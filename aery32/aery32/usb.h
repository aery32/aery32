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

#ifndef __AERY32_USB_H
#define __AERY32_USB_H

#include <avr32/io.h>
#include "usb/descriptor/cdc.h"


enum Usb_epbank {
	USB_EPBANK_SINGLE,
	USB_EPBANK_DOUBLE,
	USB_EPBANK_TRIPLE
};

enum Usb_epsize {
	USB_EPSIZE_8,
	USB_EPSIZE_16,
	USB_EPSIZE_32,
	USB_EPSIZE_64,
	USB_EPSIZE_128,
	USB_EPSIZE_256,
	USB_EPSIZE_512,
	USB_EPSIZE_1024
};


enum Usb_eptype {
	USB_EPTYPE_CONTROL,
	USB_EPTYPE_ISOCHRONOUS,
	USB_EPTYPE_BULK,
	USB_EPTYPE_INTERRUPT
};

enum Usb_epdirection {
	USB_EPDIR_OUT,
	USB_EPDIR_IN /* not for control epns */
};

namespace aery {

void usb_init_device(bool low_speed = false);

/**
 * Allocates the endpoint number n, epn
 *
 * USB Endpoint packet lengths:
 * - [Control] The packet length of control transfers in low speed devices
 *   must be 8 bytes, high speed devices allow a packet size of 8, 16, 32
 *   or 64 bytes and full speed devices must have a packet size of 64 bytes.
 * - [INTERRUPT] The maximum data payload size for low-speed devices is
 *   8 bytes. For full-speed devices it is 64 bytes and for high-speed devices
 *   1024 bytes.
 * - [ISOCHRONOUS] The maximum data payload can be up to 1023 bytes for a
 *   full speed device and 1024 bytes for a high speed device.
 * - [BULK] Bulk transfers are only supported by full and high speed devices.
 *   For full speed epns, the maximum bulk packet size is either 8, 16,
 *   32 or 64 bytes long. For high speed epns, the maximum packet size
 *   can be up to 512 bytes long.
 *
 * \note
 * - USB and epns have to be enabled before epns can be allocated.
 *   You can call usb_enable() and usb_enable_all_epns() to do this.
 * - Endpoints can only be allocated in ascending order from epn 0
 *   to the last epn to be allocated.
 * - Endpoint 0 has to be control type.
 * - Endpoints have to be allocated before enabling USB Device Interrupts
 *   in Endpoint Control Register (UECONXSET), see datasheet p. 570
 * 
 * \param epn  Endpoint number which to allocate: 0-6
 * \param bank USB_EPBANK_SINGLE | USB_EPBANK_DOUBLE | USB_EPBANK_TRIPLE
 * \param size USB_EPSIZE_8 | USB_EPSIZE_16 | USB_EPSIZE_32 | USB_EPSIZE_64 |
 *             USB_EPSIZE_128 | USB_EPSIZE_256 | USB_EPSIZE_512 | USB_EPSIZE_1024
 * \param type USB_EPTYPE_CONTROL | USB_EPTYPE_ISOCHRONOUS | USB_EPTYPE_BULK |
 *             USB_EPTYPE_INTERRUPT
 * \param dir  USB_EPDIR_IN | USB_EPDIR_OUT
 */
void usb_alloc_epn(int epn, enum Usb_epbank bank,
	enum Usb_epsize size, enum Usb_eptype type,
	enum Usb_epdirection dir);

void usb_free_epn(int epn);
void usb_free_all_eps(void);

void usb_enable_epn(int epn);
void usb_enable_all_eps(void);

void usb_reset_epn(int epn);
void usb_reset_all_eps(void);

bool usb_epn_is_active(int epn);

void usb_enable(void);
void usb_disable(void);
bool usb_is_enabled(void);

int usb_attach(bool wakeup = false);
void usb_detach(void);
}

#endif

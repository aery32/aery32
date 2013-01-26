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

#include "aery32/usb.h"
volatile avr32_usbb_t *__usb = &AVR32_USBB;

void aery::usb_init_device(bool low_speed)
{
	aery::usb_detach();
	aery::usb_disable();

	avr32_usbb_usbcon_t usbcon = {};
	usbcon.uimod   = 1; /* USB device mode selected */
	usbcon.uide    = 0; /* Do not use USB_ID pin to select mode */
	usbcon.frzclk  = 1; /* Keep clock freezed before the usb_enable() is called */
	usbcon.vbuspo  = 0; /* VBus polarity is active high (default USB functionality) */
	usbcon.otgpade = 0; /* Disable on-the-go OTG pad */
	usbcon.srpreq  = 1;
	usbcon.vbushwc = 1; /* Disable hardware control over USB VBus. Only used in host and OTG applications */
	usbcon.stoe    = 1;
	usbcon.hnperre = 1;
	usbcon.roleexe = 1;
	usbcon.bcerre  = 1;
	usbcon.vberre  = 1;
	usbcon.srpe    = 1;
	usbcon.vbuste  = 1;
	usbcon.idte    = 1;

	/* Write USB Control Register */
	__usb->usbcon = *((unsigned long*) &usbcon);

	/*
	 * Select full or low speed. This bit should be configured before
	 * attaching the device. Thus detached for sure above.
	 */
	__usb->UDCON.ls = low_speed;
}

void aery::usb_alloc_epn(int epn,
	enum Usb_epbank bank, enum Usb_epsize size,
	enum Usb_eptype type, enum Usb_epdirection dir)
{
	volatile unsigned long *uecfg = &__usb->uecfg0 + epn;
	*uecfg = (bank << AVR32_USBB_UECFG0_EPBK_OFFSET) |
	         (size << AVR32_USBB_UECFG0_EPSIZE_OFFSET) |
	         (type << AVR32_USBB_UECFG0_EPTYPE_OFFSET) |
	         (dir << AVR32_USBB_UECFG0_EPDIR_OFFSET);
	*uecfg |= AVR32_USBB_UECFG0_ALLOC_MASK;
}

void aery::usb_free_epn(int epn)
{
	volatile unsigned long *uecfg = &__usb->uecfg0 + epn;
	*uecfg &= ~AVR32_USBB_UECFG0_ALLOC_MASK;
}

void aery::usb_free_all_eps(void)
{
	for (int i = 0; i < AVR32_USBB_EPT_NUM; i++) {
		aery::usb_free_epn(i);
	}
}

void aery::usb_enable_epn(int epn)
{
	__usb->uerst |= (1 << epn) << AVR32_USBB_UERST_EPEN0_OFFSET;
}

void aery::usb_enable_all_eps()
{
	for (int i = 0; i < AVR32_USBB_EPT_NUM; i++) {
		aery::usb_enable_epn(i);
	}
}

void aery::usb_reset_epn(int epn)
{
	__usb->uerst |= (1 << epn) << AVR32_USBB_UERST_EPRST0_OFFSET;
}

void aery::usb_reset_all_eps()
{
	for (int i = 0; i < AVR32_USBB_EPT_NUM; i++) {
		aery::usb_reset_epn(i);
	}
}

bool aery::usb_epn_is_active(int epn)
{
	const volatile unsigned long *uesta = &__usb->uesta0 + epn;
	return *uesta & AVR32_USBB_UESTA0_CFGOK_MASK;
}

void aery::usb_enable(void)
{
	__usb->USBCON.frzclk = 0;
	__usb->USBCON.usbe = 1;
}

void aery::usb_disable(void)
{
	__usb->USBCON.usbe = 0;
	__usb->USBCON.frzclk = 1;
}

int aery::usb_attach(bool wakeup)
{
	if(__usb->USBCON.usbe == 0 || __usb->USBCON.frzclk == 1)
		return -1;

	__usb->UDCON.detach = 1;
	__usb->UDCON.rmwkup = wakeup;
	__usb->UDCON.detach = 0;
	return 0;
}

void aery::usb_detach(void)
{
	__usb->UDCON.detach = 1;
}

bool aery::usb_is_enabled()
{
	return __usb->USBCON.usbe;
}
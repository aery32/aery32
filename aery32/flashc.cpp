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

#include <cstring>
#include "aery32/flashc.h"

volatile avr32_flashc_t *aery::flashc = &AVR32_FLASHC;
volatile uint32_t aery::__flashc_lsr = AVR32_FLASHC.fsr;

void aery::flashc_init(enum Flash_ws ws, bool ensas)
{
	AVR32_FLASHC.FCR.fws = ws;
	//AVR32_FLASHC.FCR.sasd = ensas; /* http://asf.atmel.com/bugzilla/show_bug.cgi?id=2647 */
	AVR32_FLASHC.FCR.frdy = 0;
	AVR32_FLASHC.FCR.locke = 0;
	AVR32_FLASHC.FCR.proge = 0;

	switch (FLASH_SIZE_IN_KILOBYTES) {
	case 32:
		AVR32_FLASHC.FSR.fsz = 0;
		break;
	case 64:
		AVR32_FLASHC.FSR.fsz = 1;
		break;
	case 128:
		AVR32_FLASHC.FSR.fsz = 2;
		break;
	case 256:
		AVR32_FLASHC.FSR.fsz = 3;
		break;
	case 384:
		AVR32_FLASHC.FSR.fsz = 4;
		break;
	case 512:
		AVR32_FLASHC.FSR.fsz = 5;
		break;
	case 768:
		AVR32_FLASHC.FSR.fsz = 6;
		break;
	case 1024:
		AVR32_FLASHC.FSR.fsz = 7;
		break;
	}
}

void aery::flashc_instruct(uint16_t pagenum, enum Flash_cmd cmd)
{
	while (aery::flashc_isbusy());
	AVR32_FLASHC.FCMD.pagen = pagenum;
	AVR32_FLASHC.FCMD.cmd = cmd;
	AVR32_FLASHC.FCMD.key = AVR32_FLASHC_FCMD_KEY_KEY;
}

void *aery::flashc_read_page(uint16_t pagenum, void *buf)
{
	unsigned char *dest = (unsigned char*) buf;
	const unsigned char *src = AVR32_FLASH +
			(pagenum * FLASH_PAGE_SIZE_IN_BYTES);

	while (aery::flashc_isbusy());
	return memcpy(dest, src, FLASH_PAGE_SIZE_IN_BYTES);
}

int aery::flashc_save_page(uint16_t pagenum, const void *buf)
{
	using namespace aery;
	const unsigned char *src = (unsigned char*) buf;
	unsigned char *dest = AVR32_FLASH +
			(pagenum * FLASH_PAGE_SIZE_IN_BYTES);

	/* Erase page */
	flashc_instruct(pagenum, FLASH_CMD_EP);

	/* Clear page buffer */
	flashc_instruct(pagenum, FLASH_CMD_CPB);
	 
	/* Fill page using only 32-bit access (write 4 bytes at once) */
	for (int n = 0; n < 16; n++) {
		src += (n * 4);
		dest += (n * 4);
		memcpy((uint32_t*) dest, (uint32_t*) src, 4);
	}

	/* Write (or save) page with the new data */
	flashc_instruct(pagenum, FLASH_CMD_WP);

	while (flashc_isbusy());
	if (__flashc_lsr & AVR32_FLASHC_FSR_LOCKE_MASK)
		return EFLASH_PAGE_LOCKED;
	if (__flashc_lsr & AVR32_FLASHC_FSR_PROGE_MASK)
		return EFLASH_PROG_ERR;
	return 0;
}

bool aery::flashc_page_isempty(uint16_t pagenum)
{
	aery::flashc_instruct(pagenum, FLASH_CMD_QPR);
	while (aery::flashc_isbusy());
	return AVR32_FLASHC.FSR.qprr;
}

bool aery::flashc_page_haslock(uint16_t pagenum)
{
	uint32_t mask = (pagenum / AVR32_FLASHC_PAGES_PR_REGION) + 1;
	mask = mask << AVR32_FLASHC_FSR_LOCK0_OFFSET;

	aery::__flashc_lsr = AVR32_FLASHC.fsr;
	return (aery::__flashc_lsr & mask);
}

bool aery::flashc_isbusy(void)
{
	aery::__flashc_lsr = AVR32_FLASHC.fsr;
	return (aery::__flashc_lsr & AVR32_FLASHC_FSR_FRDY_MASK) == 0;
}
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
#include "aery32/errno.h"

volatile avr32_flashc_t *aery::flashc = &AVR32_FLASHC;

void aery::flashc_init(enum Flash_ws ws, bool ensas)
{
	AVR32_FLASHC.FCR.fws = ws;
	//AVR32_FLASHC.FCR.sasd = ensas;
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
	while (aery::flashc_status() == BUSY);
	AVR32_FLASHC.FCMD.pagen = pagenum;
	AVR32_FLASHC.FCMD.cmd = cmd;
	AVR32_FLASHC.FCMD.key = AVR32_FLASHC_FCMD_KEY_KEY;
}

void *aery::flashc_read_page(uint16_t pagenum, void *buf)
{
	unsigned char *dest = (unsigned char*) buf;
	const unsigned char *src = AVR32_FLASH + (pagenum * FLASH_PAGE_SIZE_IN_BYTES);

	while (aery::flashc_status() == BUSY);
	return memcpy(dest, src, FLASH_PAGE_SIZE_IN_BYTES);
}

int aery::flashc_save_page(uint16_t pagenum, const void *buf)
{
	enum Flashc_status status;
	unsigned char *dest = AVR32_FLASH + (pagenum * FLASH_PAGE_SIZE_IN_BYTES);
	const unsigned char *src = (unsigned char*) buf;

	aery::flashc_instruct(pagenum, FLASH_CMD_EP); /* erase page */
	aery::flashc_instruct(pagenum, FLASH_CMD_CPB); /* clear page buffer */
	 
	/* fill page using only 32-bit access (write 4 bytes at once) */
	for (int n = 0; n < 16; n++) {
		src += (n * 4);
		dest += (n * 4);
		memcpy((uint32_t*) dest, (uint32_t*) src, 1);
	}
	aery::flashc_instruct(pagenum, FLASH_CMD_WP); /* write page */

	while ((status = aery::flashc_status()) == BUSY);

	switch (status) {
	case ELOCK:
		aery::errno = EFLASH_ELOCK;
		break;
	case EPROG:
		aery::errno = EFLASH_EPROG;
		break;
	case READY:
		return 0;
	}
	return -1;
}

bool aery::flashc_page_isempty(uint16_t pagenum)
{
	aery::flashc_instruct(pagenum, FLASH_CMD_QPR);
	while (aery::flashc_status() == BUSY);
	return AVR32_FLASHC.FSR.qprr;
}

bool aery::flashc_page_haslock(uint16_t pagenum)
{
	return true;
}

enum Flashc_status aery::flashc_status(void)
{
	enum Flashc_status status;
	uint32_t fsr = AVR32_FLASHC.fsr;

	status = (fsr & AVR32_FLASHC_FSR_FRDY_MASK) : READY ? BUSY;
	if (status == READY) {
		status = (fsr & AVR32_FLASHC_FSR_LOCKE_MASK) : ELOCK ? status;
		status = (fsr & AVR32_FLASHC_FSR_PROGE_MASK) : EPROG ? status;
	}
	return status;
}
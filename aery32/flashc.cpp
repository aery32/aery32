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

#include "aery32/flashc.h"

volatile avr32_flashc_t *aery::flashc = &AVR32_FLASHC;

void aery::flashc_init(enum Flash_ws ws, bool ensas)
{
	AVR32_FLASHC.FCR.fws = ws;
	//AVR32_FLASHC.FCR.sasd = ensas;
	AVR32_FLASHC.FCR.frdy = 0;
	AVR32_FLASHC.FCR.locke = 0;
	AVR32_FLASHC.FCR.proge = 0;

	switch (AVR32_FLASHC_FLASH_SIZE/1024 /* in kilobytes */) {
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

bool aery::flashc_page_isempty(uint16_t pagenum)
{
	aery::flashc_instruct(pagenum, FLASH_CMD_QPR);
	while (aery::flashc_isbusy());
	return AVR32_FLASHC.FSR.qprr;
}
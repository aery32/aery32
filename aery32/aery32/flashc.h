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
\file aery32/flashc.h
\brief Flash Controller (flashc)
*/

#ifndef __AERY32_FLASHC_H
#define __AERY32_FLASHC_H

extern "C" {
	#include <inttypes.h>
	#include <avr32/io.h>
}

#define FLASH_WS0_MAX_CLKSPEED   (AVR32_FLASHC_FWS_0_MAX_FREQ)
#define FLASH_SIZE_IN_KILOBYTES  (AVR32_FLASHC_FLASH_SIZE / 1024)
#define FLASH_PAGE_SIZE_IN_BYTES (AVR32_FLASHC_PAGE_SIZE)
#define FLASH_PAGENUM_MAX \
	((AVR32_FLASHC_FLASH_SIZE / AVR32_FLASHC_PAGE_SIZE) - 1)

/* Flash error codes */
#define EFLASH_PAGE_LOCKED -1
#define EFLASH_PROG_ERR    -2

namespace aery {

/**
 * Pointer to the internal Flash Controller module register
 */
extern volatile avr32_flashc_t *flashc;

/**
 * Last status read
 */
extern volatile uint32_t _flashc_lsr;

/**
 *  wait state
 */
enum Flash_ws {
	FLASH_0WS,
	FLASH_1WS
};

/**
 * Flash commands
 */
enum Flash_cmd {
	FLASH_CMD_NOP,
	FLASH_CMD_WP,
	FLASH_CMD_EP,
	FLASH_CMD_CPB,
	FLASH_CMD_LP,
	FLASH_CMD_UP,
	FLASH_CMD_EA,
	FLASH_CMD_WGPB,
	FLASH_CMD_EGPB,
	FLASH_CMD_SSB,
	FLASH_CMD_PGPFB,
	FLASH_CMD_EAGPF,
	FLASH_CMD_QPR,
	FLASH_CMD_WUP,
	FLASH_CMD_EUP,
	FLASH_CMD_QPRUP
};

/**
 * Initializes flash controller
 * \param ws    Flash wait state number
 * \param ensas Permanently enable sense amplifiers. Consumes more power.
 */
void flashc_init(enum Flash_ws ws, bool ensas);

/**
 * Instructs Flash controller to apply the given command to the page number
 * \param pagenum Flash page number
 * \param command Flash command
 * \return
 */
void flashc_instruct(uint16_t pagenum, enum Flash_cmd command);

int flashc_save_page(uint16_t pagenum, const void *buf);

void *flashc_read_page(uint16_t pagenum, void *buf);

bool flashc_page_isempty(uint16_t pagenum);

bool flashc_page_haslock(uint16_t pagenum);

bool flashc_isbusy(void);

} /* end of namespace */

#endif

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
\file aery32/usart.h
\brief Universal Synchronous/Asynchronous Receiver/Transmitter (USART)
*/

#ifndef __AERY32_USART_H
#define __AERY32_USART_H

#include <cstdio>
extern "C" {
	#include <inttypes.h>
	#include <avr32/io.h>
}

namespace aery {

extern volatile avr32_usart_t *usart0;
extern volatile avr32_usart_t *usart1;
extern volatile avr32_usart_t *usart2;
extern volatile avr32_usart_t *usart3;

enum Usart_parity {
	USART_PARITY_EVEN,
	USART_PARITY_ODD,
	USART_PARITY_MARKED,
	USART_PARITY_SPACE,
	USART_PARITY_NONE
};

enum Usart_stopbits {
	USART_STOPBITS_1,
	USART_STOPBITS_1p5,
	USART_STOPBITS_2
};

enum Usart_databits {
	USART_DATABITS_5,
	USART_DATABITS_6,
	USART_DATABITS_7,
	USART_DATABITS_8,
	USART_DATABITS_9
};

enum Usart_clock {
	USART_CLK_PBA,
	USART_CLK_PBADIV,
	USART_CLK_EXTPIN = 3
};

enum Usart_spimode {
	USART_SPI_MODE0,
	USART_SPI_MODE1,
	USART_SPI_MODE2,
	USART_SPI_MODE3
};

/**
 * Init USART for a serial communication
 * \param pusart Pointer to the USART peripheral which to init
 */
void usart_init_serial(volatile avr32_usart_t *usart,
	enum Usart_parity parity = USART_PARITY_NONE,
	enum Usart_stopbits = USART_STOPBITS_1);

void usart_init_spim(volatile avr32_usart_t *usart,
	enum Usart_spimode mode, enum Usart_databits databits);

void usart_init_spis(volatile avr32_usart_t *usart,
	enum Usart_spimode mode, enum Usart_databits databits);

void usart_setup_speed(volatile avr32_usart_t *usart,
	enum Usart_clock clk, uint16_t cd, uint8_t fp = 0, bool over = true);

void usart_set_databits(volatile avr32_usart_t *usart,
	enum Usart_databits databits);

int usart_set_spimode(volatile avr32_usart_t *usart,
	enum Usart_spimode mode);

int usart_read(volatile avr32_usart_t *usart, int *data);
int usart_read(volatile avr32_usart_t *usart, int *buf, size_t n);

int usart_write(volatile avr32_usart_t *usart, int data);
int usart_write(volatile avr32_usart_t *usart, const int *buf, size_t n);

int usart_puts(volatile avr32_usart_t *usart, const char *str);

int usart_putc(volatile avr32_usart_t *usart, char c);

int usart_getc(volatile avr32_usart_t *usart);

char* usart_gets(volatile avr32_usart_t *usart,
	char *str, size_t n, char terminator = '\n');

uint32_t usart_wait_txready(volatile avr32_usart_t *usart);

uint32_t usart_wait_rxready(volatile avr32_usart_t *usart);

void usart_reset_status(volatile avr32_usart_t *usart);

bool usart_has_overrun(volatile avr32_usart_t *usart);

void usart_enable_rx(volatile avr32_usart_t *usart);

void usart_disable_rx(volatile avr32_usart_t *usart);

void usart_enable_tx(volatile avr32_usart_t *usart);

void usart_disable_tx(volatile avr32_usart_t *usart);

} /* end of namespace */

#endif

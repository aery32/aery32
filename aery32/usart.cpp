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
#include "aery32/usart.h"

namespace aery {
	volatile avr32_usart_t *usart0 = &AVR32_USART0;
	volatile avr32_usart_t *usart1 = &AVR32_USART1;
	volatile avr32_usart_t *usart2 = &AVR32_USART2;
	volatile avr32_usart_t *usart3 = &AVR32_USART3;
}

void aery::usart_init_serial(volatile avr32_usart_t *usart,
	enum Usart_parity parity, enum Usart_databits databits,
	enum Usart_stopbits, bool sync, bool msbf)
{
	usart->CR.rsttx = 1;
	usart->CR.rstrx = 1;

	usart->MR.mode = 0;
	usart->MR.msbf = msbf;
	usart->MR.sync = sync;
	usart->MR.clko = false;

	if (databits == USART_DATABITS_9) {
		usart->MR.mode9 = 1;
	} else {
		usart->MR.mode9 = 0;
		usart->MR.chrl = databits;
	}

	usart->MR.par = parity;
}

void aery::usart_setup_speed(volatile avr32_usart_t *usart,
	enum Usart_clock clk, uint16_t cd, uint8_t fp, bool over)
{
	usart->MR.usclks = clk;
	usart->MR.over = over;
	usart->BRGR.cd = cd;
	usart->BRGR.fp = fp;
}

int aery::usart_read(volatile avr32_usart_t *usart, int *buf, size_t n)
{
	uint32_t status;
	for (size_t i = 0; i < n; i++) {
		status = aery::usart_wait_rxready(usart);
		if (status & (AVR32_USART_PARE_MASK|AVR32_USART_FRAME_MASK))
			return i;
		buf[i] = usart->RHR.rxchr;
	}
	return n;
}

int aery::usart_write(volatile avr32_usart_t *usart, const int *buf, size_t n)
{
	uint32_t status;
	aery::usart_wait_txready(usart);

	for (size_t i = 0; i < n; i++) {
		usart->THR.txchr = buf[i];
		status = aery::usart_wait_txready(usart);
		if (status & (AVR32_USART_PARE_MASK|AVR32_USART_FRAME_MASK))
			return i;
	}
	return n;
}

char aery::usart_putc(volatile avr32_usart_t *usart, char c)
{
	int c2 = c;
	if (aery::usart_write(usart, &c2, 1) == 0)
		return EOF;
	return c;
}

int aery::usart_puts(volatile avr32_usart_t *usart, const char *str)
{
	size_t n = strlen(str);
	for (size_t i = 0; i < n; i++) {
		if (aery::usart_putc(usart, str[i]) == EOF)
			return EOF;
	}
	return n;
}

int aery::usart_getc(volatile avr32_usart_t *usart)
{
	int c;
	if (aery::usart_read(usart, &c, 1) == 0)
		return EOF;
	return c;
}

char* aery::usart_gets(volatile avr32_usart_t *usart, char *str,
	size_t n, char terminator)
{
	size_t i = 0;
	for (; i < n; i++) {
		if ((str[i] = aery::usart_getc(usart)) == terminator)
			break;
	}
	str[i] = '\0';
	return str;
}

uint32_t aery::usart_wait_txready(volatile avr32_usart_t *usart)
{
	uint32_t status = usart->csr;
	while ((status & AVR32_USART_TXRDY_MASK) == 0)
		status = usart->csr;
	return status;
}

uint32_t aery::usart_wait_rxready(volatile avr32_usart_t *usart)
{
	uint32_t status = usart->csr;
	while ((status & AVR32_USART_RXRDY_MASK) == 0)
		status = usart->csr;
	return status;
}

void aery::usart_enable_rx(volatile avr32_usart_t *usart)
{
	usart->CR.rxen = true;
}

void aery::usart_enable_tx(volatile avr32_usart_t *usart)
{
	usart->CR.txen = true;
}

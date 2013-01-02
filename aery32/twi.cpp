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

#include "aery32/twi.h"
#include "aery32/devnull.h"

namespace aery {
	volatile avr32_twi_t *twi = &AVR32_TWI;
	volatile uint32_t __twi_lsr = AVR32_TWI.sr;
}

#define TWI_WAIT_TO_COMPLETE() do { \
	while ((aery::__twi_lsr & AVR32_TWI_SR_TXCOMP_MASK) == 0) \
		aery::__twi_lsr = aery::twi->sr; \
} while (0)

void aery::twi_init_master(void)
{
	/* Software reset. */
	aery::twi->CR.swrst = 1;
	while (aery::twi->CR.swrst);

	/* See errata, datasheet p. 794 */
	aery::__devnull = aery::twi->rhr;

	/* Setup SLK to 100 kHz by default with 50% duty cycle */
	aery::twi_setup_clkwaveform(4, 0x3f, 0x3f);
	aery::twi_clear_internal_address();

	/* Disable slave mode and enable as master */
	aery::twi->CR.svdis = 1;
	aery::twi->CR.msen = 1;
}

// void aery::twi_init_slave(uint16_t sla)
// {
// 	/* Software reset. */
// 	aery::twi->CR.swrst = 1;
// 	while (aery::twi->CR.swrst);

// 	/* Disable master mode and enable as slave with SLA */
// 	aery::twi->SMR.sadr = sla;
// 	aery::twi->CR.msdis = 1;
// 	aery::twi->CR.sven = 1;
// }

void aery::twi_setup_clkwaveform(uint8_t ckdiv, uint8_t cldiv, uint8_t chdiv)
{
	aery::twi->CWGR.ckdiv = ckdiv;
	aery::twi->CWGR.cldiv = cldiv;
	aery::twi->CWGR.chdiv = chdiv;
}

void aery::twi_select_slave(uint16_t sla)
{
	aery::twi->MMR.dadr = sla;
}

void aery::twi_use_internal_address(uint32_t iadr, uint8_t n)
{
	aery::twi->IADR.iadr = iadr;
	aery::twi->MMR.iadrsz = n;
}

void aery::twi_clear_internal_address(void)
{
	aery::twi_use_internal_address(0, 0);
}

size_t aery::twi_read_nbytes(uint8_t *data, size_t n)
{
	using namespace aery;
	size_t i = 0;

	if (n == 1)
		return twi_read_byte(data);

begin:
	/* Switch to read mode */
	twi->MMR.mread = 1;

	/* Start multiple byte read operation */
	twi->cr |= AVR32_TWI_CR_START_MASK;

	/* Check arbitration */
	if (((__twi_lsr = twi->sr) & AVR32_TWI_SR_ARBLST_MASK) != 0)
		goto begin;

	for (; i < n - 1; i++) {
		while (twi_isbusy());
		if ((__twi_lsr & AVR32_TWI_SR_NACK_MASK) != 0)
			goto error;
		data[i] = twi->RHR.rxdata;
	}

	/* Send STOP */
	twi->cr |= AVR32_TWI_CR_STOP_MASK;

	/* Read last byte */
	while (twi_isbusy());
	if ((__twi_lsr & AVR32_TWI_SR_NACK_MASK) == 0)
		data[i++] = twi->RHR.rxdata;

	TWI_WAIT_TO_COMPLETE();
	return i;

error:
	twi->cr |= AVR32_TWI_CR_STOP_MASK;
	TWI_WAIT_TO_COMPLETE();
	return i;
}

size_t aery::twi_read_nbytes(uint8_t *data, size_t n, uint8_t iadr,
		uint8_t iadrlen)
{
	aery::twi_use_internal_address(iadr, iadrlen);
	return aery::twi_read_nbytes(data, n);
}

size_t aery::twi_read_byte(uint8_t *data)
{
	size_t i = 0;

	/* Switch to read mode */
	aery::twi->MMR.mread = 1;

	/* Start one byte read operation */
	aery::twi->cr |= AVR32_TWI_CR_START_MASK | AVR32_TWI_CR_STOP_MASK;

	while (aery::twi_isbusy());
	if ((aery::__twi_lsr & AVR32_TWI_SR_NACK_MASK) == 0)
		data[i++] = aery::twi->RHR.rxdata;

	TWI_WAIT_TO_COMPLETE();
	return i;
}

size_t aery::twi_read_byte(uint8_t *data, uint8_t iadr, uint8_t iadrlen)
{
	aery::twi_use_internal_address(iadr, iadrlen);
	return aery::twi_read_byte(data);
}

size_t aery::twi_write_nbytes(uint8_t *data, size_t n)
{
	size_t i = 0;

	/* Switch to write mode */
	aery::twi->MMR.mread = 0;
	
	for (; i < n; i++) {
		aery::twi->THR.txdata = data[i];
		while (aery::twi_isbusy());
		if ((aery::__twi_lsr & AVR32_TWI_SR_NACK_MASK) != 0)
			break;
	}
	
	TWI_WAIT_TO_COMPLETE();
	return i;
}

size_t aery::twi_write_nbytes(uint8_t *data, size_t n, uint8_t iadr,
		uint8_t iadrlen)
{
	aery::twi_use_internal_address(iadr, iadrlen);
	return aery::twi_write_nbytes(data, n);
}

size_t aery::twi_write_byte(uint8_t data)
{
	return aery::twi_write_nbytes(&data, 1);
}

size_t aery::twi_write_byte(uint8_t data, uint8_t iadr, uint8_t iadrlen)
{
	return aery::twi_write_nbytes(&data, 1, iadr, iadrlen);
}

bool aery::twi_isbusy(void)
{
	aery::__twi_lsr = aery::twi->sr;
	if (aery::twi->MMR.mread == 1)
		return (aery::__twi_lsr & AVR32_TWI_SR_RXRDY_MASK) == 0;
	return (aery::__twi_lsr & AVR32_TWI_SR_TXRDY_MASK) == 0;
}

bool aery::twi_is_enabled()
{
	#define TWI_PINS ((1 << 29) | (1 << 30))
	volatile avr32_gpio_port_t *twi_port = &AVR32_GPIO.port[0];

	/* 
	 * TWI is enabled when dedicate TWD and TWCK have been assigned to
	 * the peripheral lines and defined to be as open-drain. */
	if ((twi_port->gper & TWI_PINS) != 0)
		return false;
	if ((twi_port->pmr0 & TWI_PINS) != 0)
		return false;
	if ((twi_port->pmr1 & TWI_PINS) != 0)
		return false;
	if ((twi_port->odmer & TWI_PINS) != 0)
		return false;
	return true;
}

bool aery::twi_has_overrun(bool reread)
{
	if (reread)
		aery::__twi_lsr = aery::twi->sr;
	return (aery::__twi_lsr & AVR32_TWI_SR_OVRE_MASK) != 0;
}
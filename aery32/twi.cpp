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

namespace aery {
	volatile avr32_twi_t *twi = &AVR32_TWI;
	volatile uint32_t __twi_lsr = AVR32_TWI.sr;
}

void aery::twi_init_master(void)
{
	/* Software reset. Just in case. */
	aery::twi->CR.swrst = 1;
	while (aery::twi->CR.swrst);

	/* Setup SLK to 400 kHz by default with 50% duty cycle */
	aery::twi_setup_clkwaveform(0x01, 0x3f, 0x3f);
	
	/* Disable slave mode and enable as master */
	aery::twi->CR.svdis = 0;
	aery::twi->CR.msen = 1;
}

int aery::twi_init_slave(uint16_t sla)
{
	return 0; /* TODO */
}

int aery::twi_setup_clkwaveform(uint8_t ckdiv, uint8_t cldiv, uint8_t chdiv)
{
	if (ckdiv > 7)
		return -1;
	aery::twi->CWGR.ckdiv = ckdiv;
	aery::twi->CWGR.cldiv = cldiv;
	aery::twi->CWGR.chdiv = chdiv;
	return 0;
}

void aery::twi_select_slave(uint16_t sla)
{
	aery::twi->MMR.dadr = sla;
}

int aery::twi_read_byte(void)
{
	aery::twi->MMR.mread = 1; /* Switch to read mode */
	aery::twi->cr |= 3; /* START and STOP have to been written at once */

	while (aery::twi_isbusy());
	if (aery::__twi_lsr & AVR32_TWI_SR_NACK_MASK)
		return ETWI_READ_NACK;
	return aery::twi->RHR.rxdata;
}

int aery::twi_read_byte(uint16_t regaddr)
{
	return 0; /* TODO */
}

int aery::twi_write_byte(uint8_t data)
{
	aery::twi->MMR.mread = 0; /* Switch to write mode */
	aery::twi->THR.txdata = data;

	while (aery::twi_isbusy());
	if (aery::__twi_lsr & AVR32_TWI_SR_NACK_MASK)
		return ETWI_WRITE_NACK;
	while (aery::twi->SR.txcomp == 0);
	return 0;
}

int aery::twi_write_byte(uint8_t data, uint16_t regaddr)
{
	aery::twi->MMR.iadrsz = 1;
	aery::twi->IADR.iadr = regaddr;
	return aery::twi_write_byte(data);
}

bool aery::twi_isbusy(void)
{
	aery::__twi_lsr = aery::twi->sr;
	if (aery::twi->MMR.mread == 1) {
		return (aery::__twi_lsr & AVR32_TWI_SR_RXRDY_MASK) == 0;
	}
	return (aery::__twi_lsr & AVR32_TWI_SR_TXRDY_MASK) == 0;
}

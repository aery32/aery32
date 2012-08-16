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

int aery::twi_init_master(bool addressing_7bit)
{
	/* Software reset. Just in case. */
	AVR32_TWI.CR.swrst = 1;

	if (addressing_7bit == false) {
		/* Setup this correctly */
	}

	/* Setup SLK to 400 kHz by default with 50% duty cycle */
	aery::twi_setup_clkwaveform(0x01, 0x3f, 0x3f);
	
	/* Disable slave mode and enable as master */
	AVR32_TWI.CR.svdis = 0;
	AVR32_TWI.CR.msen = 1;

	return 0;
}

int aery::twi_init_master(uint16_t sla, bool addressing_7bit)
{
	int rv = 0;
	if ((rv = aery::twi_init_master(addressing_7bit)) < 0)
		return rv;
	aery::twi_select_slave(sla);
	return rv;
}

int aery::twi_init_slave(uint16_t sla)
{
	return 0; /* TODO */
}

int aery::twi_setup_clkwaveform(uint8_t ckdiv, uint8_t cldiv, uint8_t chdiv)
{
	if (ckdiv > 7)
		return -1;
	AVR32_TWI.CWGR.ckdiv = ckdiv;
	AVR32_TWI.CWGR.cldiv = cldiv;
	AVR32_TWI.CWGR.chdiv = chdiv;
	return 0;
}

void aery::twi_select_slave(uint16_t sla)
{
	AVR32_TWI.MMR.dadr = sla;
}

int aery::twi_read_byte(void)
{
	AVR32_TWI.MMR.mread = 1; /* Switch to read mode */
	AVR32_TWI.cr |= 3; /* START and STOP have to been written at once */

	while (aery::twi_isbusy());
	if (aery::__twi_lsr & AVR32_TWI_SR_NACK_MASK)
		return ETWI_READ_NACK;
	return AVR32_TWI.RHR.rxdata;
}

int aery::twi_read_byte(uint16_t regaddr)
{
	return 0; /* TODO */
}

int aery::twi_write_byte(uint8_t data)
{
	AVR32_TWI.MMR.mread = 0; /* Switch to write mode */
	AVR32_TWI.THR.txdata = data;

	while (aery::twi_isbusy());
	if (aery::__twi_lsr & AVR32_TWI_SR_NACK_MASK)
		return ETWI_WRITE_NACK;
	while (AVR32_TWI.SR.txcomp == 0);
	return 0;
}

int aery::twi_write_byte(uint8_t data, uint16_t regaddr)
{
	AVR32_TWI.MMR.iadrsz = 1;
	AVR32_TWI.IADR.iadr = regaddr;
	return aery::twi_write_byte(data);
}

bool aery::twi_isbusy(void)
{
	aery::__twi_lsr = AVR32_TWI.sr;
	if (AVR32_TWI.MMR.mread == 1) {
		return (aery::__twi_lsr & AVR32_TWI_SR_RXRDY_MASK) == 0;
	}
	return (aery::__twi_lsr & AVR32_TWI_SR_TXRDY_MASK) == 0;
}

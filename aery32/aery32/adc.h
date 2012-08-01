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
\file aery32/adc.h
\brief Analog-to-digital converter (adc)

\note ADC clock frequency has its upper limit that is related to the Periheral
Bus A (PBA) clock. Thus care has to be taken to choose the proper prescaler for
the PBA during the ADC initialization. For UC3A0/A1, consult the ADC
charasteristics table from the datasheet page 772 for maximum ADC clock.
*/

#ifndef __AERY32_ADC_H
#define __AERY32_ADC_H

extern "C" {
	#include <inttypes.h>
	#include <avr32/io.h>
}

#define ADCLK_HIRES_MAX 5000000UL
#define ADCLK_LORES_MAX 8000000UL

namespace aery {

/**
 * Pointer to the internal Analog-to-digital converter peripheral module register
 */
extern volatile avr32_adc_t *adc;

/**
 * Last read status for the internal Analog-to-digital converter peripheral module
 */
extern volatile uint32_t __adc_lsr;

/**
 * ADC hardware trigger selection
 */
enum Adc_trigger {
	INTERNAL_TRG0,
	INTERNAL_TRG1,
	INTERNAL_TRG3,
	INTERNAL_TRG4,
	INTERNAL_TRG5,
	EXTERNAL_TRG
};

/**
 * Initializes Analog-to-digital converter
 * \param prescal Prescaler that has used to divide PBA clock for proper ADC
 *                clock, adclk = pba_clk / (2 * (prescal+1))
 * \param hires   Use 10-bit resolution, define false if 8-bit is preferred
 * \param shtime  ADC sample and hold time, time = (shtim + 1) / adclk
 * \param startup ADC startup time, time = (startup + 1) * 8 / adclk
 * \return 0 on success, -1 if the maximum ADC frequency has been exceeded
 *
 * \note Calls pm_get_fclkdomain(CLKDOMAIN_PBA)
 */
int adc_init(uint8_t prescal, bool hires, uint8_t shtime, uint8_t startup);

/**
 * Setups hardware trigger for Analog-to-digital converter
 * \param trigger Hardware trigger selection.
 */
void adc_setup_trigger(enum Adc_trigger trigger);

/**
 * Starts the conversion
 */
void adc_start_cnv(void);

/**
 * Reads the conversion result for the given channel
 * \param chanum Channel number, 0-7.
 * \return Conversion result in binary form
 *
 * Before reading the conversion, check that the conversion is ready
 * by using adc_isbusy(1 << chanum).
 */
uint16_t adc_read_cnv(uint8_t chanum);

/**
 * Reads the last conversion result. Whatever was the channel.
 * \return the conversion result in binary form
 *
 * Before reading the last conversion, check that the conversion is
 * ready by using adc_isbusy() function.
 */
uint16_t adc_read_lastcnv(void);

/**
 * Enables Analog-to-digital converter for the given channel or channels
 * \param chamask Channel mask for which channels should be enabled
 *
 * \code
 * adc_enable(0xff); // Enables all channels
 * adc_enable(1<<2); // Enables channel two
 * \endcode
 */
void adc_enable(uint8_t chamask);

/**
 * Disables Analog-to-digital converter for the given channel or channels
 * \param chamask Channel mask for which channels should be disabled.
 */
void adc_disable(uint8_t chamask);

/**
 * Tells if the conversion for the given channel is ready
 * \param chamask Channel mask. Default mask value is 0.
 * \return 1 if the conversion for the given channel is not ready, 0 if
 *         if the convesion is ready to read, and -1 if the given channel
 *         or channels wasn't even enabled. In case where the chamask has
 *         been omitted or defined 0, the function will discard the channel
 *         info and return 1 when the next conversion is ready to read.
 */
int adc_isbusy(uint8_t chamask = 0);

/**
 * Tells if the Analog-to-digital conversion has overrun
 * \param chamask Channel mask for which channels to check. Default mask
 *                value is 0.
 * \return True if any of the channels defined in the chamask has been
 *         overrun. If the chamask has been omitted, the function will
 *         return true in case of general overrun, being essentially the
 *         same than the call of adc_hasoverrun(0xff).
 */
bool adc_hasoverrun(uint8_t chamask = 0);

} /* end of namespace */

#endif

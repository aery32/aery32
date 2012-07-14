#ifndef __BOARD_H
#define __BOARD_H

extern "C" {
	#include <inttypes.h>
}

// ----------------------------------------------------------------------
// Board specific settings
// ----------------------------------------------------------------------
#define F_CPU 66000000UL

#define LED AVR32_PIN_PC04

#define ADC_VREF 3.0
#define ADC_BITS 10

#define HSBMASK_DEFAULT 0xFFFFFFFF
#define PBAMASK_DEFAULT 0xFFFFFFFF
#define PBBMASK_DEFAULT 0xFFFFFFFF


// ----------------------------------------------------------------------
// Board functions
// ----------------------------------------------------------------------
void init_board(void);

static inline double cnv2volt(uint32_t cnv)
{
	return cnv * (ADC_VREF / (1UL << ADC_BITS));
}

#endif

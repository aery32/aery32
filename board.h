#ifndef __BOARD_H
#define __BOARD_H

#include <aery32/all.h>

extern "C" {
	#include <inttypes.h>
}

#define LED AVR32_PIN_PC04

#define ADC_VREF 3.0
#define ADC_BITS 10

#define HSBMASK_DEFAULT 0xFFFFFFFF
#define PBAMASK_DEFAULT 0xFFFFFFFF
#define PBBMASK_DEFAULT 0xFFFFFFFF

namespace board {

void init(void);

static inline double cnv2volt(uint32_t cnv)
{
	return cnv * ((double) ADC_VREF / (1UL << ADC_BITS));
}

} /* end of namespace */

#endif

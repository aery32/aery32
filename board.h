#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

// ----------------------------------------------------------------------
// Board specific settings
// ----------------------------------------------------------------------

#define ADC_VREF 3.0
#define ADC_BITS 10

#define F_CPU 66000000UL
#define F_OSC0 12000000UL
#define F_OSC1 0UL

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

#ifdef __cplusplus
}
#endif

#endif

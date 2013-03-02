#ifndef __BOARD_H
#define __BOARD_H

#include <inttypes.h>

#define F_OSC0 12000000UL
#define F_OSC1 16000000UL
#define F_CPU  66000000UL

#define ADC_VREF 3.0
#define ADC_BITS 10

namespace board {

	/* Board initializer */
	void init(void);

	/* Converts ADC result to double */
	static inline double cnv2volt(uint32_t cnv)
	{
		return cnv * ((double) ADC_VREF / (1UL << ADC_BITS));
	}

} /* end of namespace */

#endif

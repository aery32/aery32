#include <aery32/all.h>
#include "board.h"

#define LED			AVR32_PIN_PC04
#define ADC_PINMASK_ALLCHAN	(0xff << 21)

using namespace aery;
using namespace boad;

int main(void)
{
	uint16_t result; /* conversion result in bits */
	double volt;     /* conversion in volts */

	/*
	 * Put your application initialization sequence here. The default
	 * board initializer defines all pins as input and sets the CPU clock
	 * speed to 66 MHz.
	 */
	board::init();

	gpio_init_pins(porta, ADC_PINMASK_ALLCHAN, GPIO_FUNCTION_A);

	adc_init(
		7,    /* prescal, adclk = pba_clk / (2 * (prescal+1)) */
		true, /* hires, 10-bit (false would be 8-bit) */
		0,    /* shtim, sample and hold time = (shtim + 3) / adclk */
		0     /* startup, startup time = (startup + 1) * 8 / adclk */
	);

	/* Enable ADC channel 3. That's PA24. */
	adc_enable(1 << 3);

	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */

		adc_start_cnv();
		
		while (adc_isbusy(1 << 3));
		result = adc_read_cnv(3);
		volt = cnv_to_volt(result);
	}

	return 0;
}

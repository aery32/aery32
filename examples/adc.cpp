#include "board.h"
using namespace aery;

#define ADC_PINMASK_ALLCHAN (0xff << 21)

int main(void)
{
	uint16_t result; /* conversion result in bits */
	double volt;     /* conversion in volts */

	board::init();
	gpio_init_pins(porta, ADC_PINMASK_ALLCHAN, GPIO_FUNCTION_A);

	adc_init(
		7,    /* prescal, adclk = pba_clk / (2 * (prescal+1)) */
		true, /* hires, 10-bit (false would be 8-bit) */
		0,    /* shtim, sample and hold time = (shtim + 3) / adclk */
		0     /* startup, startup time = (startup + 1) * 8 / adclk */
	);

	/* Enable ADC channel 3, PA24 */
	adc_enable((1 << 3) /* 8-bit channel mask */);


	/* All done. Turn the LED on. */
	gpio_set_pin_high(LED);

	for(;;) {
		adc_start_cnv();
		while (adc_isbusy(1 << 3) /* 8-bit channel mask */);
		result = adc_read_cnv(3);
		volt = board::cnv2volt(result);
	}

	return 0;
}

#include "board.h"
#include <aery32/all.h>

using namespace aery;

#define ADC_PINMASK_ALLCHAN (0xff << 21)

int main(void)
{
	int errno;
	uint16_t result; /* conversion result in bits */
	double volt;     /* conversion in volts */

	init_board();
	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_init_pins(porta, ADC_PINMASK_ALLCHAN, GPIO_FUNCTION_A);

	errno = adc_init(
		7,    /* prescal, adclk = pba_clk / (2 * (prescal+1)) */
		true, /* hires, 10-bit (false would be 8-bit) */
		0,    /* shtim, sample and hold time = (shtim + 1) / adclk */
		0     /* startup, startup time = (startup + 1) * 8 / adclk */
	);

	if (errno != -1)
		adc_enable(1 << 3); /* enables the channel 3 */

	/* init done, turn the LED on */
	gpio_set_pin_high(LED);

	for(;;) {
		adc_start_cnv();
		while (adc_isbusy(1 << 3));
		result = adc_read_cnv(3);
		volt = cnv2volt(result);
	}

	return 0;
}

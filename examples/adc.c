#include <stdbool.h>
#include "board.h"
#include <aery32/gpio.h>
#include <aery32/adc.h>

#define LED AVR32_PIN_PC04
#define ADC_PINMASK_ALLCHANNELS (0xff << 21)

int main(void)
{
	uint16_t result;

	init_board();
	aery_gpio_init_pin(LED, GPIO_OUTPUT);
	aery_gpio_init_pins(porta, ADC_PINMASK_ALLCHANNELS, GPIO_FUNCTION_A);

	aery_adc_init(
		8,    /* prescaler, adclk = pba_clk / (2 * (prescal+1)) */
		true, /* hires, 10-bit (false would be 8-bit) */
		0,    /* startup time */
		0     /* sample and hold time */
	);

	/* Enable adc channel 3 */
	aery_adc_enable((1 << 3) /* 8-bit channel mask */);

	/* Init done, turn the LED on */
	aery_gpio_set_pin_high(LED);

	for(;;) {
		aery_adc_start_cnv();
		while (!aery_adc_cnv_isrdy(1 << 3));
		result = aery_adc_get_cnv_result(3);
	}

	return 0;
}

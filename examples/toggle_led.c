#include <stdbool.h>
#include <aery32/gpio.h>
#include <aery32/rtc.h>
#include "board.h"

#define LED	AVR32_PIN_PC04

int main(void)
{
	init_board();
	aery_gpio_init_pin(
		LED,        /* gpio pin number: 0-69 */
		GPIO_OUTPUT /* options */
	);

	/* Init RTC to count into ~infinity. When the top value is reached
	 * (approx. after 20h), the counter will start again from 0 */
	aery_rtc_init(
		0,            /* value where to start counting */
		0xffffffff,   /* top value where to count */
		0,            /* prescaler for RTC clock */
		RTC_SOURCE_RC /* source oscillator */
	);

	aery_rtc_enable(false /* enable interrupts */);

	for(;;) {
		aery_gpio_toggle_pin(LED /* gpio pin number: 0-69 */);
		aery_rtc_delay_cycle(57500 /* number of rtc clock cycles, ~1s */);
	}

	return 0;
}


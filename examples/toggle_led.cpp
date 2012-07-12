#include <stdbool.h>
#include "board.h"
#include <aery32/gpio.h>
#include <aery32/delay.h>

#define LED AVR32_PIN_PC04

int main(void)
{
	init_board();
	aery_gpio_init_pin(LED, GPIO_OUTPUT);

	for(;;) {
		/* Toggles the LED every 0.5 second */

		aery_gpio_toggle_pin(LED);
		aery_delay_ms(500);
	}

	return 0;
}

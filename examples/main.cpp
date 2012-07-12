#include <stdbool.h>
#include "board.h"
#include <aery32/gpio.h>

#define LED AVR32_PIN_PC04

int main(void)
{
	/* Put your application initialization sequence here */
	init_board();
	aery_gpio_init_pin(LED, GPIO_OUTPUT);

	/* All done, turn the LED on */
	aery_gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */

	}

	return 0;
}

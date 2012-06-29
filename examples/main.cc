#include <stdbool.h>
#include "board.h"
#include <aery32/gpio.hh>

#define LED AVR32_PIN_PC04

using namespace aery;

int main(void)
{
	/* Put your application initialization sequence here */
	init_board();
	gpio_init_pin(LED, GPIO_OUTPUT);

	/* All done, turn the LED on */
	gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */

	}

	return 0;
}

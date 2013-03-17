#include <aery32/all.h>
#include "board.h"

#define LED AVR32_PIN_PC04

using namespace aery;

int main(void)
{
	/*
	 * The default board initializer defines all pins as input and
	 * sets the CPU clock speed to 66 MHz.
	 */
	board::init();

	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */

	}

	return 0;
}
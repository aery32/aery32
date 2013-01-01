#include "board.h"
using namespace aery;

int main(void)
{
	/*
	 * Put your application initialization sequence here. The default
	 * board initializer defines the LED pin as output and sets the CPU
	 * clock speed to 66 MHz.
	 */
	board::init();
	gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */

	}

	return 0;
}
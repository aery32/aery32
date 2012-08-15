#include "board.h"
#include <aery32/all.h>

using namespace aery;

int main(void)
{
	/*
	 * Put your application initialization sequence here. The default
	 * board_init() setups the LED pin and the CPU clock (66 MHz).
	 */
	init_board();

	/* All done. Turn the LED on. */
	gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */

	}

	return 0;
}

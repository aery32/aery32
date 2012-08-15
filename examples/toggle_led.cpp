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

	for(;;) {
		gpio_toggle_pin(LED);
		delay_ms(500);
	}

	return 0;
}

#include "board.h"
#include <aery32/all.h>

using namespace aery;

int main(void)
{
	/* Put your application initialization sequence here */

	init_board(); /* Setup hardware and CPU clock to 66 MHz */
	gpio_init_pin(LED, GPIO_OUTPUT);

	/* All done, turn the LED on */
	gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */

	}

	return 0;
}

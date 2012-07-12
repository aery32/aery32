#include "board.h"
#include <aery32/all.h>

using namespace aery;

int main(void)
{
	init_board();
	gpio_init_pin(LED, GPIO_OUTPUT);

	for(;;) {
		gpio_toggle_pin(LED);
		delay_ms(500);
	}

	return 0;
}

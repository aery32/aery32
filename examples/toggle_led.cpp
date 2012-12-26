#include "board.h"
#include <aery32/gpio.h>
#include <aery32/delay.h>

using namespace aery;

int main(void)
{
	init_board();
	for(;;) {
		gpio_toggle_pin(LED);
		delay_ms(500);
	}

	return 0;
}

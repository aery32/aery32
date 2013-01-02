#include "board.h"
using namespace aery;

int main(void)
{
	board::init();
	for(;;) {
		gpio_toggle_pin(LED);
		delay_ms(500);
	}

	return 0;
}

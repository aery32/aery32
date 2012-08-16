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
	gpio_init_pin(AVR32_PIN_PA17, GPIO_OUTPUT);

	/* All done. Turn the LED on. */
	gpio_set_pin_high(LED);

	gpio_enable_localbus();
	for(;;) {
		/*
		 * Keep in mind that the pin toggling frequency measured with the
		 * oscilloscope equals the CPU frequency divided two. If you scope out
		 * this example, you will see three periods of 33 MHz squarewave and
		 * then a longer pause because of the call of the for(;;) loop.
		 */
		lporta->ovrt = 1 << GPIO_NUM2PIN(AVR32_PIN_PA17);
		lporta->ovrt = 1 << GPIO_NUM2PIN(AVR32_PIN_PA17);
		lporta->ovrt = 1 << GPIO_NUM2PIN(AVR32_PIN_PA17);
		lporta->ovrt = 1 << GPIO_NUM2PIN(AVR32_PIN_PA17);
		lporta->ovrt = 1 << GPIO_NUM2PIN(AVR32_PIN_PA17);
		lporta->ovrt = 1 << GPIO_NUM2PIN(AVR32_PIN_PA17);
	}

	return 0;
}

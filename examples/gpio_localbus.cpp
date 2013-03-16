#include <aery32/all.h>
#include "board.h"

#define LED		AVR32_PIN_PC04
#define HISPEED_OUTPUT	AVR32_PIN_PA00

using namespace aery;

int main(void)
{
	/*
	 * Put your application initialization sequence here. The default
	 * board initializer defines all pins as input and sets the CPU clock
	 * speed to 66 MHz.
	 */
	board::init();

	gpio_init_pin(HISPEED_OUTPUT, GPIO_OUTPUT);
	gpio_init_pin(LED, GPIO_OUTPUT);

	gpio_set_pin_high(LED);
	gpio_enable_localbus();

	for(;;) {
		/* Put your application code here */
		
		/*
		 * Keep in mind that the pin toggling frequency measured with the
		 * oscilloscope equals the CPU frequency divided two. If you scope out
		 * this example, you will see three periods of 33 MHz squarewave and
		 * then a longer pause because of the call of the for(;;) loop.
		 */
		lporta->ovrt = 1 << GPIO_NUM2PIN(HISPEED_OUTPUT);
		lporta->ovrt = 1 << GPIO_NUM2PIN(HISPEED_OUTPUT);
		lporta->ovrt = 1 << GPIO_NUM2PIN(HISPEED_OUTPUT);
		lporta->ovrt = 1 << GPIO_NUM2PIN(HISPEED_OUTPUT);
		lporta->ovrt = 1 << GPIO_NUM2PIN(HISPEED_OUTPUT);
		lporta->ovrt = 1 << GPIO_NUM2PIN(HISPEED_OUTPUT);
	}

	return 0;
}

#include <aery32/all.h>
#include "board.h"

using namespace aery;

#define LED AVR32_PIN_PC04

usb usb = usb(new usb_descriptor_cdc());

int main(void)
{
	/*
	 * Put your application initialization sequence here. The default
	 * board initializer defines all pins as input and sets the CPU clock
	 * speed to 66 MHz.
	 */
	board::init();

	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */

		usb << "hello\r\n";
		delay_ms(500);
	}

	return 0;
}
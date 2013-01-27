#include <aery32/all.h>
#include "board.h"

using namespace aery;

#define LED AVR32_PIN_PC04

void isrhandler_group2(void)
{
	gpio_toggle_pin(LED);
	delay_ms(100); /* Reduce glitches */
	porta->ifrc = (1 << 0); /* Remember to clear the interrupt */
}

int main(void)
{
	/*
	 * Put your application initialization sequence here. The default
	 * board initializer defines all pins as input and sets the CPU clock
	 * speed to 66 MHz.
	 */
	board::init();
	gpio_init_pin(LED, GPIO_OUTPUT|GPIO_HIGH);

	/* GPIO pins 0-13 can be "wired" to int group 2, see datasheet p. 42 */
	gpio_init_pin(AVR32_PIN_PA00, GPIO_INPUT|GPIO_PULLUP|GPIO_INT_PIN_CHANGE);

	/* Init interrupt controller */
	intc_init();	
	intc_register_isrhandler(
		&isrhandler_group2, /* Function pointer to the ISR handler */
		2, /* Interrupt group number */
		0  /* Priority level */
	);

	/* Enable interrupts globally */
	intc_enable_globally();

	for(;;) {
		/*
		 * Now try to connect PA00 to GND and then disconnecting it
		 * to let the pull-up to set the pin state high again. The LED
		 * should toggle on the pin change.
		 */

	}

	return 0;
}
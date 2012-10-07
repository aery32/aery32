#include "board.h"
#include <aery32/gpio.h>
#include <aery32/delay.h>
#include <aery32/intc.h>

using namespace aery;

void isrhandler(void)
{
	gpio_toggle_pin(LED);
	delay_ms(100); /* Reduce glitch */
	porta->ifrc = (1 << 0); /* Remember to clear the interrupt */
}

int main(void)
{
	init_board();

	/* GPIO pins 0-13 can be "wired" to int group 2, see datasheet p. 42 */
	gpio_init_pin(AVR32_PIN_PA00, GPIO_INPUT|GPIO_PULLUP|GPIO_INT_PIN_CHANGE);

	/* Init interrupt controller */
	intc_init();	
	intc_register_isrhandler(
		&isrhandler, /* Function pointer to the ISR handler */
		2,           /* Interrupt group number */
		0            /* Priority level */
	);

	/* Enable interrupts globally */
	intc_enable_globally();

	for(;;) {
		/*
		 * Now try to connect PA00 to GND and then disconnecting it
		 * to let pull-up make the pin state high again. The LED should
		 * toggle on the pin change.
		 */

	}

	return 0;
}
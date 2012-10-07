#include "board.h"
#include <aery32/gpio.h>
#include <aery32/eic.h>
#include <aery32/intc.h>

using namespace aery;

void isrhandler(void)
{
	gpio_toggle_pin(LED);
	eic_clear_event(1 << 0);
}

int main(void)
{
	init_board();

	/* Init EXTINT[0]. That's PA22, see datasheet p. 46 */
	gpio_init_pin(AVR32_PIN_PA22, GPIO_INPUT|GPIO_FUNCTION_B);

	/* Init external interrupt controller */
	eic_init(1 << 0, EIC_EDGE_TRIGD_FALLING|EIC_ENABLE_FILTER);
	eic_enable(1 << 0);

	/* Init interrupt controller */
	intc_init();	
	intc_register_isrhandler(
		&isrhandler, /* Function pointer to the ISR handler */
		1,           /* Interrupt group number */
		0            /* Priority level */
	);

	/* Enable interrupts globally */
	intc_enable_globally();

	gpio_set_pin_high(LED);

	for(;;) {

	}

	return 0;
}
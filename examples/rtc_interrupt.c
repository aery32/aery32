#include <stdbool.h>
#include <aery32/gpio.h>
#include <aery32/rtc.h>
#include <aery32/interrupts.h>
#include "board.h"

#define LED AVR32_PIN_PC04

void isrhandler_group1(void)
{
	aery_gpio_toggle_pin(LED);
	aery_rtc_clear_interrupt();		// Remember to clear RTC interrupt
}

int main(void)
{
	init_board();
	aery_gpio_init_pin(AVR32_PIN_PA01, GPIO_OUTPUT);

	/* Init real time counter to count till 2000
	 * Params: Value where to start counting
	 *         Top value where to count
	 *         Prescaler
	 *         Source oscillator
	 */
	aery_rtc_init(0, 2000, 0, RTC_SOURCE_RC);

	/* Register ISR handler
	 * Params: Function pointer to interrupt service routine handler
	 *         Interrupt group number, see datasheet p. 41
	 *         Interrupt priority level
	 */
	aery_intc_register_isrhandler(&isrhandler_group1, 1, 0);

	/* Init interrupt controller and enable interrupts globally */
	aery_intc_init();
	aery_intc_enable_globally();

	/* RTC and Interrupts are initialized so we are ready to start RTC
	 * with interrupts enabled
	 * Params: Interrupts enabled or disabled (1 for enabled)
	 */
	aery_rtc_enable(true);

	for(;;) {
		aery_gpio_toggle_pin(AVR32_PIN_PA01);
	}

	return 0;
}

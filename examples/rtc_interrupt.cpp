#include "board.h"
using namespace aery;

void isrhandler_rtc(void)
{
	gpio_toggle_pin(LED);
	rtc_clear_interrupt(); /* Remember to clear RTC interrupt */
}

int main(void)
{
	board::init();
	gpio_init_pin(LED, GPIO_OUTPUT|GPIO_HIGH);

	/* Init RTC to count from zero to 60*115000/2 */
	rtc_init(RTC_SOURCE_RC, 0, 0, 60*115000/2);

	/* Register ISR handler */
	intc_init();
	intc_register_isrhandler(&isrhandler_rtc, 1, 0);

	/* Enable interrupts globally */
	intc_enable_globally();

	/* Enable RTC as we are nor ready to handle the interrupts */
	rtc_enable(true);

	for(;;) {
		/* Put your application code here */

	}

	return 0;
}
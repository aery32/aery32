#include "board.h"
#include <aery32/all.h>

using namespace aery;

void isrhandler_rtc(void)
{
	gpio_toggle_pin(LED);
	rtc_clear_interrupt(); /* Remember to clear RTC interrupt */
}

int main(void)
{
	init_board();
	gpio_init_pin(LED, GPIO_OUTPUT|GPIO_HIGH);
	rtc_init(RTC_SOURCE_RC, 0, 0, 60*115000/2);

	intc_init();
	intc_register_isrhandler(&isrhandler_rtc, 1, 0);
	intc_enable_globally();

	rtc_enable(true);

	for(;;) {
	}

	return 0;
}
#include <stdio.h>
#include "aery32/gpio.h"
#include "aery32/rtc.h"
#include "board.h"

int main(void)
{
	init_board();
	aery_gpio_init_pin(AVR32_PIN_PA00, GPIO_OUTPUT);

	aery_gpio_enable_localbus();
	aery_rtc_init(0, 0xffffffff, 0, RTC_SOURCE_RC);
	aery_rtc_enable(false);

	for(;;) {
		aery_rtc_delay_cycle(1150); // ~20ms
		lporta->ovrt = 1; // Toggle PA00 through local gpio bus
	}

	return 0;
}

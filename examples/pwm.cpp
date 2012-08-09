#include "board.h"
#include <aery32/all.h>

using namespace aery;

int main(void)
{
	double D = 0.0;

	init_board();
	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_init_pin(AVR32_PIN_PB21, GPIO_FUNCTION_A);

	pwm_init_channel(2, MCK_DIVIDED_BY_64);
	pwm_setup_chamode(2, LEFT_ALIGNED, START_LOW);
	pwm_enable(1 << 2);

	/* All done. Turn the LED on. */
	gpio_set_pin_high(LED);

	for(;;) {
		for (; D <= 1.0; D += 0.1) {
			pwm_update_dutycl(2, D);
			pwm_wait_periods(2, 1);
		}
		for (; D >= 0.0; D -= 0.1) {
			pwm_update_dutycl(2, D);
			pwm_wait_periods(2, 1);
		}
	}

	return 0;
}
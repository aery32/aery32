#include "board.h"
#include <aery32/all.h>

using namespace aery;

int main(void)
{
	uint32_t duration = 0;
	uint32_t period = 0x100000;

	init_board();
	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_init_pin(AVR32_PIN_PB21, GPIO_FUNCTION_A);

	pwm_init_channel(2, MCK_DIVIDED_BY_64, duration, period);
	pwm_setup_chamode(2, LEFT_ALIGNED, START_LOW);
	pwm_enable(1 << 2);

	/* All done. Turn the LED on. */
	gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */

		for (; duration <= period; duration++) {
			pwm_update_channel(2, PWM_DURATION, duration);
			delay_ms(10);
		}
		for (; duration >= 0; duration--) {
			pwm_update_channel(2, PWM_DURATION, duration);
			delay_ms(10);
		}
	}

	return 0;
}
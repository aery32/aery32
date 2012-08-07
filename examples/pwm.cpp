#include "board.h"
#include <aery32/all.h>

using namespace aery;

int main(void)
{
	int duration = 0;
	int period = 0x100000;

	init_board();
	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_init_pin(AVR32_PIN_PB21, GPIO_FUNCTION_A);

	pwm_select_clk(pwm2, MCK_DIVIDED_BY_64);
	pwm_setup_mode(pwm2, LEFT_ALIGNED, START_LOW);
	pwm_enable(1 << 2);

	/* All done. Turn the LED on. */
	gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */

		for (; duration <= period; duration++) {
			pwm_set_dutycl(pwm2, duration, period);
			delay_ms(10);
		}
		for (; duration >= 0; duration--) {
			pwm_set_dutycl(pwm2, duration, period);
			delay_ms(10);
		}
	}

	return 0;
}
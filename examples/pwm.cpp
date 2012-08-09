#include "board.h"
#include <aery32/all.h>

using namespace aery;

int main(void)
{
	uint8_t pwmcha = 2;
	double dutycl = 0.0;

	init_board();
	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_init_pin(AVR32_PIN_PB21, GPIO_FUNCTION_A);

	/* Init and enable PWM channel two */
	pwm_init_channel(pwmcha, MCK_DIVIDED_BY_128);
	pwm_setup_chamode(pwmcha, LEFT_ALIGNED, START_LOW);
	pwm_enable(1 << pwmcha);

	/* All done. Turn the LED on. */
	gpio_set_pin_high(LED);

	for(;;) {
		/* Connect LED to pin PB21 and it blinks smoothly */
		for (; dutycl <= 1.0; dutycl += 0.05) {
			pwm_update_dutycl(pwmcha, dutycl);
			pwm_wait_periods(pwmcha, 2);
		}
		for (; dutycl >= 0.0; dutycl -= 0.05) {
			pwm_update_dutycl(pwmcha, dutycl);
			pwm_wait_periods(pwmcha, 2);
		}
	}

	return 0;
}
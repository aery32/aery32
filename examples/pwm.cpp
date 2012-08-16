#include "board.h"
#include <aery32/all.h>

using namespace aery;

int main(void)
{
	uint8_t channel = 2;
	uint32_t duration = 0;
	uint32_t period = 0x1000;

	/*
	 * Put your application initialization sequence here. The default
	 * board_init() setups the LED pin and the CPU clock (66 MHz).
	 */
	init_board();

	/* If you change PWM channel, remember to change pin too */
	gpio_init_pin(AVR32_PIN_PB21, GPIO_FUNCTION_A);

	/* Init and enable PWM channel two */
	pwm_init_channel(channel, MCK, duration, period);
	pwm_setup_chamode(channel, LEFT_ALIGNED, START_HIGH);
	pwm_enable(1 << channel);

	/* All done. Turn the LED on. */
	gpio_set_pin_high(LED);

	for(;;) {
		/* Connect external LED to pin PB21 and it blinks smoothly */
		for (; duration < period; duration++) {
			pwm_update_duration(channel, duration);
			pwm_wait_periods(channel, 500);
		}
		for (; duration > 0; duration--) {
			pwm_update_duration(channel, duration);
			pwm_wait_periods(channel, 500);
		}
	}

	return 0;
}
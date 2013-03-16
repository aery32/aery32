#include <aery32/all.h>
#include "board.h"

#define LED			AVR32_PIN_PC04
#define PWM_CHANNEL2_OUTPUT	AVR32_PIN_PB21

using namespace aery;

int main(void)
{
	uint8_t channel = 2;
	uint32_t period = 0x1000;
	uint32_t duration = 0;

	/*
	 * Put your application initialization sequence here. The default
	 * board initializer defines all pins as input and sets the CPU clock
	 * speed to 66 MHz.
	 */
	board::init();

	/* If you change PWM channel, remember to change the pin too. */
	gpio_init_pin(PWM_CHANNEL2_OUTPUT, GPIO_FUNCTION_A);

	/* Initialize and enable PWM channel two. */
	pwm_init_channel(channel, MCK, duration, period);
	pwm_setup_chamode(channel, LEFT_ALIGNED, START_HIGH);
	pwm_enable(1 << channel);

	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_set_pin_high(LED);

	for(;;) {
		/*
		 * Put your application code here. For example, connect
		 * external LED to pin PB21 and it blinks smoothly.
		 */

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
#include <aery32/all.h>
#include "board.h"

using namespace aery;

#define LED		AVR32_PIN_PC04
#define TWI_PINMASK	((1 << 29) | (1 << 30))

int main(void)
{
	bool twi_slave_found = false;
	uint8_t twi_slave_address = 0;
	uint8_t read_data = 0;


	/*
	 * Put your application initialization sequence here. The default
	 * board initializer defines all pins as input and sets the CPU clock
	 * speed to 66 MHz.
	 */
	board::init();

	gpio_init_pins(porta, TWI_PINMASK, GPIO_FUNCTION_A|GPIO_OPENDRAIN);
	twi_init_master();

	/* Scan for the first device on the twi-bus */
	for (; twi_slave_address <= 0x7f; twi_slave_address++) {
		twi_select_slave(twi_slave_address);
		if (twi_write_byte(0x00) == 1) {
			twi_slave_found = true;
			break;
		}
	}

	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */

		if (twi_slave_found) {
			twi_write_byte(0x90 /*data*/, 0x80 /*int. device addr. */);
			twi_read_byte(&read_data);
			delay_ms(500);
		}
	}

	return 0;
}

#include "board.h"
#include <aery32/all.h>

using namespace aery;

#define TWI_MASK ((1 << 29) | (1 << 30))

int main(void)
{
	bool twi_slave_found = false;
	uint8_t twi_slave_address = 0;
	uint8_t read_data = 0;

	/*
	 * Put your application initialization sequence here. The default
	 * board_init() setups the LED pin and the CPU clock (66 MHz).
	 */
	init_board();
	gpio_init_pins(porta, TWI_MASK, GPIO_FUNCTION_A);
	twi_init_master();

	/* All done. Turn the LED on. */
	gpio_set_pin_high(LED);

	/* Scan for the first device on the twi-bus */
	for (; twi_slave_address <= 0x7f; twi_slave_address++) {
		twi_select_slave(twi_slave_address);
		if (twi_write_byte(0x00) == 0) {
			twi_slave_found = true;
			break;
		}
	}

	if (twi_slave_found) {
		twi_write_byte(0x90 /* data */, 0x80 /* internal device address */);
		read_data = twi_read_byte(0x80 /* internal device address */);
	}

	for(;;) {
		/* Put your application code here */

	}

	return 0;
}

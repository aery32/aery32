#include "board.h"
using namespace aery;

#define TWI_MASK ((1 << 29) | (1 << 30))

int main(void)
{
	bool twi_slave_found = false;
	uint8_t twi_slave_address = 0;
	uint8_t read_data = 0;

	board::init();
	gpio_init_pins(porta, TWI_MASK, GPIO_FUNCTION_A|GPIO_OPENDRAIN);
	twi_init_master();

	/* All done. Turn the LED on. */
	gpio_set_pin_high(LED);

	/* Scan for the first device on the twi-bus */
	for (; twi_slave_address <= 0x7f; twi_slave_address++) {
		twi_select_slave(twi_slave_address);
		if (twi_write_byte(0x00) == 1) {
			twi_slave_found = true;
			break;
		}
	}

	if (twi_slave_found) {
		/* Writes 0x90 using 0x80 as internal device address */
		twi_write_byte(0x90, 0x80);

		/* Reads from the same address (0x80) */
		twi_read_byte(&read_data);
	}

	for(;;) {
		/* Put your application code here */

	}

	return 0;
}

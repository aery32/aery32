#include "board.h"
#include <aery32/all.h>

using namespace aery;

#define SPI0_PINMASK ((1 << 10) | (1 << 11) | (1 << 12) | (1 << 13))
#define SPI1_PINMASK ((1 << 15) | (1 << 16) | (1 << 17) | (1 << 19))

int main(void)
{
	uint16_t rd0, rd1; /* received data */

	/*
	 * Put your application initialization sequence here. The default
	 * board_init() setups the LED pin and the CPU clock (66 MHz).
	 */
	init_board();
	gpio_init_pins(porta, SPI0_PINMASK, GPIO_FUNCTION_A);
	gpio_init_pins(porta, SPI1_PINMASK, GPIO_FUNCTION_B);

	/*
	 * Init SPI0 as master. The chip select pin 0 of SPI0 is set to work in
	 * spi mode 0 and 16 bits wide shift register.
	 */
	spi_init_master(spi0);
	spi_setup_npcs(spi0, 0, SPI_MODE0, 16);
	spi_enable(spi0);

	/*
	 * Init SPI1 as master. The chip select pin 2 of SPI1 is set to work in
	 * spi mode 1 and 8 bits wide shift register.
	 */
	spi_init_master(spi1);
	spi_setup_npcs(spi1, 2, SPI_MODE1, 8);
	spi_enable(spi1);

	/* All done. Turn the LED on. */
	gpio_set_pin_high(LED);

	for (;;) {
		/*
		 * Single 16 bit spi transmission. The word 0x5155 is transmitted
		 * by using SPI0 peripheral and chip select pin 0. The return value
		 * is the received data.
		 */
		rd0 = spi_transmit(spi0, 0, 0x5155);

		/*
		 * Partially transfer 24 bit word in 8 bits wide chunks by using
		 * SPI1 peripheral and chip select pin 2. Note that two of the
		 * first transmission indicate that they weren't the last ones to
		 * keep the chip select pin low.
		 */
		rd1 = spi_transmit(spi1, 2, 0x51, false);
		rd1 |= spi_transmit(spi1, 2, 0x55, false) << 8;
		rd1 |= spi_transmit(spi1, 2, 0xF3, true) << 16;
	}

	return 0;
}

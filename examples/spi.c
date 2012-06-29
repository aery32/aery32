#include <stdbool.h>
#include "board.h"
#include <aery32/gpio.h>
#include <aery32/spi.h>

#define LED AVR32_PIN_PC04
#define SPI0_GPIO_MASK ((1 << 10) | (1 << 11) | (1 << 12) | (1 << 13))
#define SPI1_GPIO_MASK ((1 << 15) | (1 << 16) | (1 << 17) | (1 << 19))

int main(void)
{
	volatile uint16_t rd0, rd1;		// Received data 0 and 1

	init_board();
	aery_gpio_init_pin(LED, GPIO_OUTPUT|GPIO_HIGH);

	aery_gpio_init_pins(
		porta,          /* pointer to port register address */
		SPI0_GPIO_MASK, /* pin mask */
		GPIO_FUNCTION_A /* options */
	);
	aery_gpio_init_pins(porta, SPI1_GPIO_MASK, GPIO_FUNCTION_B);

	/* Init SPI0 as master. The chip select pin 0 of SPI0 is set to work in
	 * spi mode 0 and 16 bits wide shift register. */
	aery_spi_init_master(spi0 /* pointer to spi register address */);
	aery_spi_setup_npcs(
		spi0,      /* pointer to spi register address */
		0,         /* chip select line number */
		SPI_MODE0, /* spi mode */
		16         /* spi shift register width in bits */
	);
	aery_spi_enable(spi0);

	/* Init SPI1 as master. The chip select pin 2 of SPI1 is set to work in
	 * spi mode 1 and 8 bits wide shift register.
	 */
	aery_spi_init_master(spi1);
	aery_spi_setup_npcs(spi1, 2, SPI_MODE1, 8);
	aery_spi_enable(spi1);

	for (;;) {
		/* Single 16 bit spi transmission. The word 0x5155 is transmitted
		 * by using SPI0 peripheral and chip select pin 0. The return value
		 * is the received data.
		 */
		rd0 = aery_spi_transmit(
			spi0,   /* pointer to the spi register address */
			0x5155, /* word to be transmitted */
			0,      /* chip select pin which to use */
			true    /* is the last transmission */
		);

		/* Partially transfer 24 bit word in 8 bits wide chunks by using
		 * SPI1 peripheral and chip select pin 2. Note that two of the
		 * first transmission indicate that they weren't the last ones to
		 * keep the chip select pin low.
		 */
		rd1 = aery_spi_transmit(spi1, 0x51, 2, false);
		rd1 |= aery_spi_transmit(spi1, 0x55, 2, false) << 8;
		rd1 |= aery_spi_transmit(spi1, 0xF3, 2, true) << 16;
	}

	return 0;
}

#include <stdbool.h>
#include "board.h"
#include <aery32/gpio.h>
#include <aery32/spi.h>
#include <aery32/delay.h>

// ----------------------------------------------------------------------
// HD44780 instruction set
// ----------------------------------------------------------------------
#define HD44780_CLEAR_DISPLAY       0x01
#define HD44780_RETURN_HOME         0x02

#define HD44780_EMODE_INCREMENT     0x06
#define HD44780_EMODE_DECREMENT     0x04
#define HD44780_EMODE_INCRNSHIFT    0x07
#define HD44780_EMODE_DECRNSHIFT    0x05

#define HD44780_DISPLAY_ON          0x0C
#define HD44780_DISPLAY_OFF         0x08
#define HD44780_CURSOR_ON           0x0E
#define HD44780_CURSOR_ONBLINK      0x0F

#define HD44780_LSHIFT_CURSOR       0x10
#define HD44780_RSHIFT_CURSOR       0x14
#define HD44780_LSHIFT_DISPLAY      0x18
#define HD44780_RSHIFT_DISPLAY      0x1C

#define HD44780_DL4BIT              0x28
#define HD44780_DL8BIT              0x38
#define HD44780_FONTBL_ENJA         0x28
#define HD44780_FONTBL_WE1          0x29
#define HD44780_FONTBL_ENRU         0x2A
#define HD44780_FONTBL_WE2          0x2B

#define HD44780_DDRAM_ADDR          0x80
#define HD44780_CGRAM_ADDR          0x40

#define HD44780_BUSYBIT_MASK        0x80


// ----------------------------------------------------------------------
// Additional board settings
// ----------------------------------------------------------------------
#define LED                 AVR32_PIN_PC04
#define SPI0_GPIO_MASK      ((1 << 10) | (1 << 11) | (1 << 12) | (1 << 13))

#define DISPLAY_SPI         spi0
#define DISPLAY_SPI_NPCS    0   
#define DISPLAY_SPI_MODE    SPI_MODE3


// ----------------------------------------------------------------------
// Display functions
// ----------------------------------------------------------------------
bool
display_isbusy(void)
{
	uint16_t rd; /* read data */

	aery_spi_transmit(DISPLAY_SPI, 0x100, DISPLAY_SPI_NPCS, false);
	rd = aery_spi_transmit(DISPLAY_SPI, 0x100, DISPLAY_SPI_NPCS, true);
	return ((HD44780_BUSYBIT_MASK << 2) & rd) != 0;
}

void
display_wait(void)
{
	while (display_isbusy()) {
		aery_delay_us(600);
	}
}

void
display_instruct(uint16_t instruction)
{
	display_wait();
	aery_spi_transmit(DISPLAY_SPI, instruction, DISPLAY_SPI_NPCS, true);
}

void
display_wrbyte(uint8_t byte)
{
	display_wait();
	aery_spi_transmit(DISPLAY_SPI, 0x200|byte, DISPLAY_SPI_NPCS, true);
}

void
display_putc(char c)
{
	display_wrbyte((uint8_t) c);
}

int
display_puts(const char *buf)
{
	int i = 0;
	for (; *(buf+i); i++) {
		display_putc(*(buf+i));
	}
	return i;
}

int
display_nputs(const char *buf, int n)
{
	int i = 0;
	for (; *(buf+i) && i < n; i++) {
		display_putc(*(buf+i));
	}
	return i;
}

void
display_goto(uint8_t x, uint8_t y)
{
	uint16_t position = HD44780_DDRAM_ADDR + x;
	if (y == 1) {
		position |= 0x40;
	}
	display_instruct(position);
}


// ----------------------------------------------------------------------
// Main function
// ----------------------------------------------------------------------
int
main(void)
{
	init_board();
	aery_gpio_init_pin(LED, GPIO_OUTPUT);
	aery_gpio_init_pins(porta, SPI0_GPIO_MASK, GPIO_FUNCTION_A);

	aery_spi_init_master(DISPLAY_SPI);
	aery_spi_setup_npcs(DISPLAY_SPI, DISPLAY_SPI_NPCS, DISPLAY_SPI_MODE, 10);
	aery_spi_enable(DISPLAY_SPI);

	// Display initialization sequence
	aery_delay_ms(2);
	display_instruct(HD44780_DL8BIT|HD44780_FONTBL_WE1);
	display_instruct(HD44780_DISPLAY_OFF);
	display_instruct(HD44780_CLEAR_DISPLAY);
	display_instruct(HD44780_RETURN_HOME);
	display_instruct(HD44780_EMODE_INCREMENT);
	display_instruct(HD44780_CURSOR_ONBLINK);

	// Init OK
	aery_gpio_set_pin_high(LED);

	// Greet Aery32 community
	display_puts("Hello Aery32 devs!");

	for(;;) {
		/* Put your application code here */

	}

	return 0;
}
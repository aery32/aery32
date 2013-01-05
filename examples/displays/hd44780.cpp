#include "board.h"
using namespace aery;

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
#define SPI0_PINMASK        ((1 << 10) | (1 << 11) | (1 << 12) | (1 << 13))

#define DISPLAY_SPI         spi0
#define DISPLAY_SPI_NPCS    0
#define DISPLAY_SPI_MODE    SPI_MODE3


// ----------------------------------------------------------------------
// Display functions
// ----------------------------------------------------------------------
bool display_isbusy(void);
void display_wait(void);
void display_instruct(uint16_t instruction);
int display_putchar(int c);
int display_puts(const char *buf);
int display_puts(const char *buf, size_t n);
void display_goto(uint8_t x, uint8_t y);


// ----------------------------------------------------------------------
// Main function
// ----------------------------------------------------------------------
int main(void)
{
	board::init();
	gpio_init_pins(porta, SPI0_PINMASK, GPIO_FUNCTION_A);

	spi_init_master(DISPLAY_SPI);
	spi_setup_npcs(DISPLAY_SPI, DISPLAY_SPI_NPCS, DISPLAY_SPI_MODE, 10);
	spi_enable(DISPLAY_SPI);

	/* Display initialization sequence */
	delay_ms(2);
	display_instruct(HD44780_DL8BIT|HD44780_FONTBL_WE1);
	display_instruct(HD44780_DISPLAY_OFF);
	display_instruct(HD44780_CLEAR_DISPLAY);
	display_instruct(HD44780_RETURN_HOME);
	display_instruct(HD44780_EMODE_INCREMENT);
	display_instruct(HD44780_DISPLAY_ON|HD44780_CURSOR_ONBLINK);

	/* Initialization done. Turn the LED on. */
	gpio_set_pin_high(LED);

	/* Greet Aery32 community */
	display_puts("Hello Aery32 devs!");

	for(;;) {
		/* Put your application code here */

	}

	return 0;
}

bool display_isbusy(void)
{
	uint16_t rd; /* read data */

	spi_transmit(DISPLAY_SPI, DISPLAY_SPI_NPCS, 0x100, false);
	rd = spi_transmit(DISPLAY_SPI, DISPLAY_SPI_NPCS, 0x100);
	return ((HD44780_BUSYBIT_MASK << 2) & rd) != 0;
}

void display_wait(void)
{
	while (display_isbusy())
		delay_us(600);
}

void display_instruct(uint16_t instruction)
{
	display_wait();
	spi_transmit(DISPLAY_SPI, DISPLAY_SPI_NPCS, instruction);
}

int display_putchar(int c)
{
	display_wait();
	spi_transmit(DISPLAY_SPI, DISPLAY_SPI_NPCS, 0x200|c);
	return c;
}

int display_puts(const char *buf)
{
	return nputs(buf, strlen(buf), display_putchar);
}

int display_puts(const char *buf, size_t n)
{
	return nputs(buf, n, display_putchar);
}

void display_goto(uint8_t x, uint8_t y)
{
	uint16_t position = x|HD44780_DDRAM_ADDR;
	if (y == 1)
		position |= 0x40;
	display_instruct(position);
}
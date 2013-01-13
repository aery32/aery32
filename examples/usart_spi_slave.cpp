#include <aery32/all.h>
#include "board.h"

using namespace aery;

#define LED 			AVR32_PIN_PC04
#define UART0_SPIS_PINMASK	(0xe | (1 << 4))

int main(void)
{
	char input;

	/*
	 * Put your application initialization sequence here. The default
	 * board initializer defines all pins as input and sets the CPU clock
	 * speed to 66 MHz.
	 */
	board::init();

	/*
	 * PA00 => MOSI, USART RXD
	 * PA01 => MISO, USART TXD
	 * PA02 => CLK
	 * PA04 => SC, USART CTS
	 */
	gpio_init_pins(porta, UART0_SPIS_PINMASK, GPIO_FUNCTION_A);
	
	/*
	 * Initializes USART0 as SPI slave with SPI mode 0, 8-bit wide
	 * shift register.
	 */
	usart_init_spis(usart0, USART_SPI_MODE0, USART_DATABITS_8);
	usart_enable_rx(usart0);
	usart_enable_tx(usart0);

	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */
		
		input = usart_getc(usart0);
		usart_putc(usart0, input);
	}

	return 0;
}

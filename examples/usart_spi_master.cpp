#include "board.h"
using namespace aery;

int main(void)
{
	board::init();

	/*
	 * In UC3A:
	 * PA00 => MISO, USART RXD
	 * PA01 => MOSI, USART TXD
	 * PA02 => CLK
	 * PA03 => SC, USART RTS
	 */
	#define UART0_SPIM_PINMASK (0xf)
	gpio_init_pins(porta, UART0_SPIM_PINMASK, GPIO_FUNCTION_A);
	
	/*
	 * Initializes USART0 as SPI master with SPI mode 0, 8-bit wide
	 * shift register and 100 000 kHz clock (assuming that F_PBA is
	 * 66 MHz).
	 */
	usart_init_spim(usart0, USART_SPI_MODE0, USART_DATABITS_8);
	usart_setup_speed(usart0, USART_CLK_PBA, 660);
	usart_enable_rx(usart0);
	usart_enable_tx(usart0);

	gpio_set_pin_high(LED);

	for(;;) {
		usart_putc(usart0, 0x51);
	}

	return 0;
}

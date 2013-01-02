#include "board.h"
using namespace aery;

int main(void)
{
	board::init();
	
	/*
	 * In UC3A:
	 * PA00 => RXD
	 * PA01 => TXD
	 */
	#define UART0_SERIAL_PINMASK (0b11)
	gpio_init_pins(porta, UART0_SERIAL_PINMASK, GPIO_FUNCTION_A);
	
	/* 
	 * Initializes USART0 in async mode with 8 data bits, 1 stop bit
	 * and no parity. After then sets up baud rate to 115200 bit/s
	 * (error 0.8%) and enables rx/tx.
	 *
	 * Baud rate divider has been selected in assumption that
	 * F_PBA is 66 MHz.
	 */
	usart_init_serial(usart0);
	usart_setup_speed(usart0, USART_CLK_PBA, 71);
	usart_enable_rx(usart0);
	usart_enable_tx(usart0);

	gpio_set_pin_high(LED);

	#define BUFSIZE 100
	char buf[BUFSIZE] = "";

	for(;;) {
		usart_puts(usart0, "in: ");
		usart_gets(usart0, buf, BUFSIZE);

		usart_puts(usart0, "\r\nout: ");
		usart_puts(usart0, buf);
		usart_puts(usart0, "\r\n\n");
	}

	return 0;
}

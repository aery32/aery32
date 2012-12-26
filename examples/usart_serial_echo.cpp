#include "board.h"
#include <aery32/all.h>

using namespace aery;

#define UART0_PINMASK ((1 << 0) | (1 << 1))

int main(void)
{
	init_board();
	gpio_init_pins(porta, UART0_PINMASK, GPIO_FUNCTION_A);
	
	gpio_set_pin_high(LED);

	/* 
	 * Initializes USART0 in async mode with 8 data bits, 1 stop bit
	 * and no parity. After then sets up baud rate to 115200 bit/s
	 * (error 0.8%) and enables rx/tx.
	 */
	usart_init_serial(usart0);
	usart_setup_speed(usart0, USART_CLK_PBA, 71);
	usart_enable_rx(usart0);
	usart_enable_tx(usart0);

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

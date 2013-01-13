#include <aery32/all.h>
#include "board.h"

using namespace aery;

#define LED 			AVR32_PIN_PC04
#define UART0_SERIAL_PINMASK	0x3

int main(void)
{
	#define BUFSIZE 100
	char buf[BUFSIZE] = "";

	/*
	 * Put your application initialization sequence here. The default
	 * board initializer defines all pins as input and sets the CPU clock
	 * speed to 66 MHz.
	 */
	board::init();
	
	/* PA00 => RXD, PA01 => TXD */
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

	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */
		
		usart_puts(usart0, "in: ");
		usart_gets(usart0, buf, BUFSIZE);

		usart_puts(usart0, "\r\nout: ");
		usart_puts(usart0, buf);
		usart_puts(usart0, "\r\n\n");
	}

	return 0;
}

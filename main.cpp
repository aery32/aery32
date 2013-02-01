#include <aery32/all.h>
#include "board.h"

using namespace aery;

#define LED			AVR32_PIN_PC04
#define UART0_SERIAL_PINMASK	0x3

#define DMA0_BUFSIZE		128
#define DMA1_BUFSIZE		128

volatile uint8_t bufdma0[DMA0_BUFSIZE];
volatile uint8_t bufdma1[DMA1_BUFSIZE];

int main(void)
{
	/*
	 * Put your application initialization sequence here. The default
	 * board initializer defines all pins as input and sets the CPU clock
	 * speed to 66 MHz.
	 */
	board::init();

	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_set_pin_high(LED);

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

	periph_idma dma0 = periph_idma(0, AVR32_PDCA_PID_USART0_RX, bufdma0, DMA0_BUFSIZE);
	periph_odma dma1 = periph_odma(1, AVR32_PDCA_PID_USART0_TX, bufdma1, DMA1_BUFSIZE);
	dma0.enable();
	dma1.enable();

	gpio_set_pin_high(LED);

	#define BUFSIZE 128
	uint8_t buf[BUFSIZE];

	int n = 0;

	for(;;) {
		if ((n = dma0.bytes_available())) {
			dma0.read(buf, n);
			dma1.write((uint8_t) '\n');
			dma1.write((uint8_t) '\r');
			dma1.write(buf, n).flush();
		}
		delay_ms(5000);
	}
 
	return 0;
}
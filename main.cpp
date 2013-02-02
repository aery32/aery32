#include <aery32/all.h>
#include "board.h"

using namespace aery;

#define LED			AVR32_PIN_PC04
#define UART0_SERIAL_PINMASK	0x3		// PA0 = RX, PA01 = TX

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
	gpio_init_pin(LED, GPIO_OUTPUT|GPIO_HIGH);
	gpio_init_pins(porta, UART0_SERIAL_PINMASK, GPIO_FUNCTION_A); 

	periph_idma dma0 = periph_idma(0, AVR32_PDCA_PID_USART0_RX, bufdma0, DMA0_BUFSIZE);
	periph_odma dma1 = periph_odma(1, AVR32_PDCA_PID_USART0_TX, bufdma1, DMA1_BUFSIZE);

	serial_port serial = serial_port(usart0, 115200);
	serial.set_idma(dma0).set_odma(dma1);
	serial.enable();

	serial.puts("hello ");
	serial << "world\n\r";

	char line[32] = "";
	for(;;) {
		/* Put your application code here */

		serial.puts("in: ");
		serial.getline(line);
		serial << "\r\nout: " << line << " (" << strlen(line) << ")\r\n";
		serial.print("\r\nout: %s (%d)", strlen(line));
	}

	return 0;
}
#include <aery32/all.h>
#include "board.h"

using namespace aery;

#define LED			AVR32_PIN_PC04
#define UART0_SERIAL_PINMASK	0x3		// PA0 = RX, PA01 = TX

volatile uint8_t bufdma0[128];
volatile uint8_t bufdma1[128];

int main(void)
{
	char line[32] = "";
	size_t nread = 0;

	int i = 0;
	double d = 0.0;

	/*
	 * Put your application initialization sequence here. The default
	 * board initializer defines all pins as input and sets the CPU clock
	 * speed to 66 MHz.
	 */
	board::init();
	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_init_pins(porta, UART0_SERIAL_PINMASK, GPIO_FUNCTION_A); 

	/*
	 * Instantiate input and output DMAs. You can consider these as
	 * input/output buffers.
	 */
	periph_idma dma0 = periph_idma(0, AVR32_PDCA_PID_USART0_RX, bufdma0, sizeof(bufdma0));
	periph_odma dma1 = periph_odma(1, AVR32_PDCA_PID_USART0_TX, bufdma1, sizeof(bufdma1));

	/*
	 * Instantiate serial port class driver with DMAs. Default speed
	 * is 115200, parity none, 8 data bits and one stop bit.
	 */
	serial_port pc = serial_port(usart0, dma0, dma1);

	/*
	 * Change the default line delimitation (or line ending recognition)
	 * from "\r\n", which is the default, to '\n'.
	 */
	pc.set_default_delim('\n');
	pc.enable();

	gpio_set_pin_high(LED);
	pc << "Hello Aery" << 32 << '\n';

	for(;;) {
		/* Put your application code here */

		pc.getline(line, &nread);
		if (nread == 0) continue; /* skip blank line */
		pc << "'" << line << "' is " << nread << " characters long.\n";

		sscanf(line, "%d %lf", &i, &d);
		pc.print("i = %d, d = %.2lf\n", i, d);
	}

	return 0;
}
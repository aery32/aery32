#include <aery32/all.h>
#include "board.h"

#define LED AVR32_PIN_PC04

using namespace aery;

int main(void)
{
	int errno;
	char buf[512] = {'\0'};
	uint16_t page = FLASH_LAST_PAGE;

	/*
	 * Put your application initialization sequence here. The default
	 * board initializer defines all pins as input and sets the CPU clock
	 * speed to 66 MHz.
	 */
	board::init();

	/*
	 * If the page is empty, save "foo" into it, else read the page
	 * in to buffer.
	 */
	if (flashc_page_isempty(page)) {
		strcpy(buf, "foo");
		errno = flashc_save_page(page, buf);

		switch (errno) {
		case EFLASH_PAGE_LOCKED:
			break;
		case EFLASH_PROG_ERR:
			break;
		}
	} else {
		flashc_read_page(page, buf);
	}

	gpio_init_pin(LED, GPIO_OUTPUT);
	gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */

	}

	return 0;
}
#include <cstring>
#include "board.h"
#include <aery32/all.h>

using namespace aery;

int main(void)
{
	int errno;
	uint16_t page = FLASH_LAST_PAGE;
	char buf[512] = {'\0'};

	init_board();
	gpio_init_pin(LED, GPIO_OUTPUT);

	/* If page is empty, write "foo". Else read the page. */
	if (flashc_page_isempty(page)) {
		strcpy(buf, "foo");
		errno = flashc_save_page(page, buf);

		switch (errno) {
		case EFLASH_PAGE_LOCKED:
			/* Page was locked */
			break;
		case EFLASH_PROG_ERR:
			/* Programming error was occurred */
			break;
		}
	} else {
		flashc_read_page(page, buf);
	}

	gpio_set_pin_high(LED);

	for(;;) {
		/* Put your application code here */

	}

	return 0;
}
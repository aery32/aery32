#include <cstring>
#include "board.h"
#include <aery32/all.h>

using namespace aery;

void lock_flash_programspace(void)
{
	int i = FLASH_LAST_PAGE;
	for (; flashc_page_isempty(i); i--);
	for (; i >= 0; i--) {
		flashc_lock_page(i);
	}
}

int main(void)
{
	int errno;
	uint16_t page = FLASH_LAST_PAGE;
	char buf[512] = {'\0'};

	init_board();
	gpio_init_pin(LED, GPIO_OUTPUT);

	/* Lock flash region for the uploaded program. Just to be in safe. */
	lock_flash_programspace();

	/* If page is empty, write "foo". Else read page. */
	if (flashc_page_isempty(page)) {
		strcpy(buf, "foo");
		errno = flashc_save_page(page, buf);

		switch (errno) {
		case EFLASH_PAGE_LOCKED:
			/* Page was locked */
			break;
		case EFLASH_PROG_ERR:
			/* Programming error was accured */
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
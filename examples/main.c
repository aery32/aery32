#include <stdbool.h>
#include <aery32/gpio.h>
#include "board.h"

#define LED AVR32_PIN_PC04

int main(void)
{
	init_board();
	aery_gpio_init_pin(
		LED,                  /* gpio pin number: 0-69 */
		GPIO_OUTPUT|GPIO_HIGH /* options */
	);

	for(;;) {
		/* Put your application code here */

	}

	return 0;
}

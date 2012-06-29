#include <stdbool.h>
#include "board.h"
#include <aery32/gpio.h>
#include <aery32/intc.h>
#include <aery32/rtc.h>

#define LED AVR32_PIN_PC04

void isrhandler_rtc(void)
{
    aery_gpio_toggle_pin(LED);
    aery_rtc_clear_interrupt(); // Remember to clear RTC interrupt
}

int main(void)
{
    init_board();
    aery_gpio_init_pin(LED, GPIO_OUTPUT|GPIO_HIGH);
    aery_rtc_init(0, 10*115000/2, 0, RTC_SOURCE_RC);

    aery_intc_init();
    aery_intc_register_isrhandler(&isrhandler_rtc, 1, 0);
    aery_intc_enable_globally();

    aery_rtc_enable(true);

    for(;;) {
    }

    return 0;
}
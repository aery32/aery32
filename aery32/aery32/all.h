/*
 *  _____             ___ ___   |
 * |  _  |___ ___ _ _|_  |_  |  |  C/C++ framework for 32-bit AVRs
 * |     | -_|  _| | |_  |  _|  |  
 * |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
 *               |___|          |
 *
 * Copyright (c) 2012-2013, Muiku Oy
 * All rights reserved.
 *
 * LICENSE
 *
 * New BSD License, see the LICENSE.txt bundled with this package. If you did
 * not receive a copy of the license and are unable to obtain it through the
 * world-wide-web, please send an email to contact@muiku.com so we can send
 * you a copy.
 */

/*!
\file aery32/all.h
\brief Includes all the headers files at once
\namespace aery Aery

\example adc.cpp
\example flashc_save_foo.cpp
\example gpio_localbus.cpp
\example gpio_toggle_led.cpp
\example pwm.cpp
\example rtc_interrupt.cpp
\example spi.cpp
\example twi_scan.cpp
\example usart_serial_echo.cpp
\example usart_spi_master.cpp
\example usart_spi_slave.cpp

\example displays/hd44780.cpp
\example displays/hd44780_print_adc.cpp
*/


#ifndef __AERY32_ALL_H
#define __AERY32_ALL_H

/* Low level module functions */
#include "adc.h"
#include "delay.h"
#include "flashc.h"
#include "gpio.h"
#include "intc.h"
#include "pm.h"
#include "pwm.h"
#include "rtc.h"
#include "spi.h"
#include "twi.h"
#include "usart.h"

/* High level class drivers */
#include "periph_iodma_clsdrv.h"
#include "serial_port_clsdrv.h"

#include "string.h"
#include "util.h"

#endif

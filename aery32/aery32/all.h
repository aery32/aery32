/*
 *  _____             ___ ___   |
 * |  _  |___ ___ _ _|_  |_  |  |  C/C++ framework for 32-bit AVRs
 * |     | -_|  _| | |_  |  _|  |  
 * |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
 *               |___|          |
 *
 * Copyright (c) 2012, Muiku Oy
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

\example main.cpp
\example adc.cpp
\example rtc_interrupt.cpp
\example spi.cpp
\example toggle_led.cpp

\example displays/hd44780.cpp
*/


#ifndef __AERY32_ALL_H
#define __AERY32_ALL_H

#include "aery32/adc.h"
#include "aery32/delay.h"
#include "aery32/flashc.h"
#include "aery32/gpio.h"
#include "aery32/intc.h"
#include "aery32/pm.h"
#include "aery32/pwm.h"
#include "aery32/rtc.h"
#include "aery32/spi.h"
#include "aery32/twi.h"

#include "aery32/string.h"

#endif

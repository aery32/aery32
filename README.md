      _____             ___ ___   |
     |  _  |___ ___ _ _|_  |_  |  |  C/C++ framework for 32-bit AVRs
     |     | -_|  _| | |_  |  _|  |  
     |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
                   |___|          |


Aery32 is a starting point for new AVR32-based projects. It provides a
project structure and library enabling rapid prototyping and development.
*Aery32 aims to be professionally fun* that makes it ideal framework for
Hobbyists, R&D Engineers and Academics.

[![Build Status](https://secure.travis-ci.org/aery32/aery32.png?branch=master)](http://travis-ci.org/aery32/aery32)

## Installation

You don't have to install Aery32 Software Framework as you would do with
regular software. Just download the framework, unzip it and start working
on it.

## Requirements

- Atmel AVR Toolchain
- Batchisp (for Windows) or DFU-programmer (for Mac OS X & Linux)

For Windows download and install [Atmel AVR Toolchain for Windows](http://www.atmel.com/tools/ATMELAVRTOOLCHAIN3_3_2FORWINDOWS.aspx). For Mac OS X & Linux use [AVR32 Toolchain Builder](https://github.com/jsnyder/avr32-toolchain). Batchisp comes with the [FLIP](http://www.atmel.com/tools/FLIP.aspx) installer. OS X and Linux users can install DFU-programmer with the AVR32 Toolchain Builder.
 
## Quick start

To start Aery32 project [download the framework from GitHub](https://github.com/aery32/aery32/tags). Or use git clone

    git clone git://github.com/aery32/aery32.git myproject
    cd myproject

If you tend to use Sublime Text 2, there is an [Aery32 plug-in](https://github.com/aery32/sublime-aery32) available that simplifies the project creation for you.

When you are in Aery32 project directory, start by editing `main.cpp` and `board.cpp` source files. The default board initialization sequence that can be found from `board.cpp`, starts the external oscillators and sets the master (or main) clock to 66 MHz. Other settings and board related inline functions are defined inside the `board.h` header file.

It is intended that you work under the root directory most of the time as that is the place where you keep adding your `.c` and `.cpp` source files and `.h` header files.

To compile the project just call

    make

All .cpp files under the project root will be compiled and linked with Aery32 Software Framework library. When you want to recompile the project call

    make re

To program (or upload) the application to the board, connect the USB cable and command

    make program
    
DFU is a USB class that allows board to be programmed through USB bus without external programmer board. After programming the board, you have to start it

    make start

If you like to start the board immediately after the programming, chain the program target with the start target, like this

    make program start
    
Or less verbosely `make programs`. The latter is also quicker with batchisp (in Windows).

## Learning the Aery32 Software Framework

Read the [reference guide](http://aery32.readthedocs.org) and go through the articles from [Aery32 DevZone](http://devzone.aery32.com). There is also [API documentation](http://devzone.aery32.com/aery32/apidoc/) providing a quick reference to the functions.

### Optional supportive readings

- [Learn C The Hard Way](http://c.learncodethehardway.org/)
- [The Definitive C++ Book Guide and List](http://stackoverflow.com/questions/388242/the-definitive-c-book-guide-and-list)
- [AVR32000 : Introduction to AVR32 header files](http://www.atmel.com/Images/doc32005.pdf)
- [AVR32006 : Getting started with GCC for AVR32](http://www.atmel.com/Images/doc32074.pdf)

## Release notes

### Changelog for the next release

- Minor change in Sublime Text 2 project file. No need to change build system
  manually any more (from Tools > Build System).
- It's now possible to define project wide (global) settings in one file.
  See `settings.h` header file. This file is passed to gcc via `-include`
  option.
- Build system modifications
  - Uses now -O0 optimization for debugging. This allows to set breakpoints
    in Atmel Studio 6. Silences the following error: "The breakpoint will not
    currently be hit. Unable to set requested breakpoint on target." However,
    be aware that some programs may not work properly with -O0 optimization.
    In that case try -O1.
  - The creation of `obj/` directory has been removed. It was just adding
    unnecessary complexity to the Makefile. Thus removing it.
- New linker script, which sets stack size to 4KB by default.
  - The previous linker script, adapted from the AVR32 GNU Toolchain (the one
    that avr32-gcc uses if none was given), was only slightly modified to
    start program from correct address. It was not known that in the default
    version of this linker script, the heap and stack sections weren't
    defined. This is now corrected with additional major clean up for the
    file.

### v0.4.2

- New functions:
  - `twi_is_enabled`
  - `flashc_read_userpage`, `flashc_save_userpage`, `flashc_userpage_isempty`,
    `flashc_read_fusebits`, `flashc_write_fusebit`, `flashc_write_fusebyte`
- New examples:
  - Scroll text on hd44780 type display,
    `examples/displays/hd44780_scroll_text.cpp`.
  - External interrupt on pin change, `examples/gpio_extint_pin_change.cpp`.
- Explicitly define twi pins as GPIO_OPENDRAIN in the twi scan example.
- Define GCC built-in functions as extern to silence the warnings with
  SublimeClang ST2 plug-in.
- Bug fixes:
  - Handle the division by zero gracefully in `aery::pwm_update_dutycl()`.
    Closes gh-13.

### v0.4.1

- The dependency of the PM module has been removed from the ADC module. In addition, an unnecessary error checking has been removed from `aery::adc_init()`. In future Driver Classes take responsible of those. Module functions should be low level stuff.
- `aery::adc_init()` and `aery::spi_init_master()` are now aware of reinits (means that if the module was enabled, reinit should keep it enabled).
- Added `aery::adc_is_enabled()`
- Added new example, `example/display/hd44780_print_adc.cpp`, that uses hd44780 example to print the result of A/D conversion.
- `<aery32/string.h>` now includes `<cstring>` for convenience.
- Resolved is/has naming convention. `isbusy()` makes the exception. Otherwise `is_foo()` is used. Closes gh-11.
  - Introduced few backward compatibility breaks, see below.
- Bug fixes:
  - `aery::adc_isbusy()` didn't work with channel masks.
  - Typo in `aery::nputs()` that prevented it to work.
- Backward compatibility breaks:
  - `aery::pwm_isenabled()` renamed to `aery::pwm_is_enabled()`.
  - `aery::spi_has_enabled()` renamed to `aery::spi_is_enabled()`.
  - `aery::adc_init()` does not return anymore.

### v0.4.0

- Added Twi-wire (I2C) module functions, see device scanning example `twi_scan.cpp`.
- SPI functions now take advance of C++. Allows more flexible API.
- Documentations have been expanded to cover string functions. With string functions, `itoa()` and `dtoa()`, you can convert integers and doubles to string.
- Added instructions [how to use Aery32 Framework with Sublime Text 2](http://aery32.readthedocs.org/en/latest/use_with_st2.html) and SublimeClang plugin.
- Added instructions [how to use Aery32 Framework wtih Eclipse Juno](http://aery32.readthedocs.org/en/latest/use_with_eclipse.html).
- Bug fixes:
  - `aery::gpio_read_pin()` didn't work.
  - Global variables were not placed under aery namespace.
  - GH-10. Aery32 build system for Sublime Text 2 didn't build the project, if you weren't in the project root, e.g. in main.cpp.

### v0.3.0

- New modules:
  - Flash Controller. How to operate with the chip internal flash, see `examples/writing_flash.cpp`.
  - Pulse Width Modulation (PWM).
- New module functions:
  - `adc_hasoverrun()` tells if the conversion has been overrun.
- Project file added for Sublime Text 2
  - Open ST2, select `Project/Open Project...` and open `aery32.sublime-project`. Then select `Tools/Build System` and check Aery32. Now you can build the project by pressing Ctrl+B. Ctrl+Shift+B programs the board.
- Backward compatibility breaks in module functions
  - ADC `isready()` functions removed. Use `adc_isbusy()`. Closes gh-6.
  - `rtc_init()` parameter list was reorganized. Closes gh-7.
- Changes to build system:
  - Link with the libm (math lib) by default. This has been considered to be a good practice in AVRFreak forum.
  - Enhanced to compile .c sources.
  - Do not force -O0 optimization for debug.
  - $(MAKE) wrapped with "" to achieve robust import to Atmel Studio 6.

### v0.2.1

- Hotfix. Flash wait state set to 1 by default, because by default Aery32 Framework sets CPU clockspeed to 66 MHz.

### v0.2.0

- Switch to `avr32-g++` and hence to C++
- Module functions wrapped inside `aery` namespace. No more `aery_` function prefix.
- Improvements in analog-to-digital conversion module functions.
  - `adc_setup_trigger()` allows to setup ADC hardware trigger
  - `adc_nextcnv_isrdy()` tells if the next conversion is ready. Whatever was the channel.
  - `adc_read_lastcnv()` returns the latest conversion. Whatever was the channel.
- Backward compatibility breaks
  - `adc_get_cnv()` renamed to `adc_read_cnv()`
  - `spi_transmit(pspi, data, npcs, islast)`'s parameter order changed to pspi, npcs, data, islast.
- Fixes
  - gh-5, `pm_setup_clkdomain()` in cpp branch does not work anymore as documented

### v0.1.2

- New modules
  - Analog-to-digital conversion (adc)
- Change coding style to Linux kernel style
- Backward compatibility breaks
  - `aery_pm_get_mck()` renamed to `aery_pm_get_fmck()`
  - `aery_pm_get_clkdomain_freq()` renamed to `aery_pm_get_fclkdomain()`
  - `PM_` prefix removed from Pm enums to reduce the verbosity
- Fixes
  - gh-3, general clock reinitialization does not take care of enabling the clock

### v0.1.1

- PM module has been expanded with few functions
  - `aery_pm_setup_clkdomain()` setups the precaler of the different clock domains (CPU, PBA and PBB)
  - `aery_pm_get_clkdomain_freq()` returns the clock frequency of the specific clock domain
  - `aery_pm_get_mck()` returns the master (or main) clock frequency
- Experimental "to string" functions, which can be used to convert integer and double type values to strings
- Added <aery32/all.h> header file to include the whole library at once
- Makefile tunings

### v0.1.0

- Project structure that works on Windows, Linux and Mac.
  - "make program", selects the batchisp in Windows and dfu-programmer in Linux and Mac.
- Convenient delay functions for mcu cycles, microseconds and milliseconds.
- Modules for General peripheral Input/Output (gpio), Interrupt controller (intc),
  Power manager (pm), Real-time counter (rtc) and Serial peripheral interface (spi).

## License

Aery32 Software Framework and its library is licensed under the new BSD license:

> Copyright (c) 2012, Muiku Oy
> All rights reserved.
>
> Redistribution and use in source and binary forms, with or without modification,
> are permitted provided that the following conditions are met:
>
>    * Redistributions of source code must retain the above copyright notice,
>      this list of conditions and the following disclaimer.
>
>    * Redistributions in binary form must reproduce the above copyright notice,
>      this list of conditions and the following disclaimer in the documentation
>      and/or other materials provided with the distribution.
>
>    * Neither the name of Muiku Oy nor the names of its contributors may be
>      used to endorse or promote products derived from this software without
>      specific prior written permission.
>
> THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
> ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
> WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
> DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
> ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
> (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
> LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
> ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
> (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
> SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Example applications placed under `examples/` directory are public domain.

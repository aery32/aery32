    
      _____             ___ ___   |
     |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
     |     | -_|  _| | |_  |  _|  |  
     |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
                   |___|          |

Aery32 is a starting point for new AVR32-based projects. It provides a project structure and library enabling rapid prototyping and development. *Aery32 aims to be professionally fun* that makes it ideal framework for Hobbyist, R&D Engineers and Academics.

[![Build Status](https://secure.travis-ci.org/aery32/aery32.png?branch=master)](http://travis-ci.org/aery32/aery32)

## Installation

You don't have to install Aery32 Software Framework as you would do with regular software. Just download the framework, unzip it and start working on it.

## Requirements

- Atmel AVR Toolchain
- Batchisp (for Windows) or DFU-programmer (for Mac OS X & Linux)

For Windows download and install [Atmel AVR Toolchain for Windows](http://www.atmel.com/tools/ATMELAVRTOOLCHAIN3_3_2FORWINDOWS.aspx). For Mac OS X & Linux use [AVR32 Toolchain Builder](https://github.com/jsnyder/avr32-toolchain). Batchisp comes with the [FLIP](http://www.atmel.com/tools/FLIP.aspx) installer. OS X and Linux users can install DFU-programmer with the AVR32 Toolchain Builder.
 

## Quick start

To start Aery32 project [download the framework from GitHub](https://github.com/aery32/aery32/tags). Or use git clone

    git clone git://github.com/aery32/aery32.git myproject
    cd myproject
    
Start by editing `main.c`. The default board initialization sequence, that starts the external oscillator and sets the master clock to 66 MHz, can be found from `board.c`. Other settings are defined inside the `board.h` header file. To compile the project just call

    make

When you want to recompile the project call

    make re

To program (or upload) the application to the board, connect the USB cable and command

    make program
    
DFU is a USB class that allows board to be programmed through USB bus without external programmer board. After programming the board, you have to start it

    make start

If you like to start the board immediately after the programming, chain the program target with the start target, like this

    make program start
    
Or less verbosely `make programs`. The latter is also quicker with batchisp (in Windows).

---

If you prefer C++ over C, change the `CC` in the `Makefile` to 'avr32-g++'. In this case you also have to change the specific language standard used by GCC to 'gnu++98'. This is made by modifying the `CSTANDARD` variable. You can also use environment variables with the make like this

    make re CC="avr32-g++" CSTANDARD="gnu++98"

If you have [Doxygen](http://www.stack.nl/~dimitri/doxygen/) installed, you may like to make a local version of the API documentation. This can be done by running Doxygen at the `aery32/` directory

    cd aery32/
    doxygen
    
Otherwise navigate to http://devzone.aery32.com/aery32/apidoc/.

## Release notes
### Changelog for the next release (master branch)

- New module, analog-to-digital conversion (adc)
- Change coding style to Linux kernel style
- Fix gh-3

### v0.1.1

- PM module has been expanded with few functions
  - `aery_pm_setup_clkdomain()` setups the precaler of the different clock domains (CPU, PBA and PBB)
  - `aery_pm_get_clkdomain_freq()` returns the clock frequency of the specific clock domain
  - `aery_pm_get_mck()` returns the master (or main) clock frequency
- Experimental "to string" functions, which can be used to convert integer and double type values to strings
- Added <aery32/all.h> header file to include the whole library at once
- Makefile tunings

### v0.1

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

## Supportive readings

- [AVR32000 : Introduction to AVR32 header files](http://www.atmel.com/Images/doc32005.pdf)
- [AVR32006 : Getting started with GCC for AVR32](http://www.atmel.com/Images/doc32074.pdf)
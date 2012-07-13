    
      _____             ___ ___   |
     |  _  |___ ___ _ _|_  |_  |  |  C/C++ framework for 32-bit AVRs (AVR32)
     |     | -_|  _| | |_  |  _|  |  
     |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
                   |___|          |


Aery32 is a starting point for new AVR32-based projects. It provides a project structure and library enabling rapid prototyping and development. *Aery32 aims to be professionally fun* that makes it ideal framework for Hobbyists, R&D Engineers and Academics.[![Build Status](https://secure.travis-ci.org/aery32/aery32.png?branch=master)](http://travis-ci.org/aery32/aery32)

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
    
Start by editing `main.cpp`. The default board initialization sequence, that starts the external oscillators and sets the master (or main) clock to 66 MHz, can be found from `board.c`. Other settings and board/platform related functions are defined inside the `board.h` header file. To compile the project just call

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

## Learning the Aery32 Software Framework

Read the [reference guide](http://aery32.readthedocs.org) and go through the articles from [Aery32 DevZone](http://devzone.aery32.com). There is also [API documentation](http://devzone.aery32.com/aery32/apidoc/) providing a quick reference to the functions.

### Optional supportive readings

- [Learn C The Hard Way](http://c.learncodethehardway.org/)
- [The Definitive C++ Book Guide and List](http://stackoverflow.com/questions/388242/the-definitive-c-book-guide-and-list)
- [AVR32000 : Introduction to AVR32 header files](http://www.atmel.com/Images/doc32005.pdf)
- [AVR32006 : Getting started with GCC for AVR32](http://www.atmel.com/Images/doc32074.pdf)

## Release notes
### Changelog for the next release

- Change to avr32-g++ compiler and hence to C++
- C++ allowed to add `aery` namespace, no more `aery_` function prefix
- Improvements in Analog-to-digital conversion module functions, see gh-4
- Backward compatiblity breaks
  - `adc_get_cnv` renamed to `adc_read_cnv`
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

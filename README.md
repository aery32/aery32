    
      _____             ___ ___   |
     |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
     |     | -_|  _| | |_  |  _|  |  
     |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
                   |___|          |

Aery32 is a starting point for new AVR32-based projects. It provides a complete project structure and library enabling rapid prototyping and development. Aery32 aims to be professionally fun making it ideal framework for Hobbyist, R&D Engineers and Academics.

## Installation

You don't have to install Aery32 as you would do with regular software. Just download the framework, unzip, rename the created aery32 directory according to your project and start working under that directory.

## Requirements

- Atmel AVR Toolchain
- Batchisp or DFU-programmer

For Windows download and install Atmel AVR Toolchain from [Atmel](http://www.atmel.com/tools/ATMELAVRTOOLCHAIN3_3_2FORWINDOWS.aspx). For Mac OS X & Linux use [AVR32 Toolchain Builder](https://github.com/jsnyder/avr32-toolchain).

Batchisp (for Windows) comes with the [FLIP](http://www.atmel.com/tools/FLIP.aspx) installer. In OS X and Linux use DFU-programmer that can be installed with the AVR32 Toolchain Builder.
 

## Quick start

To start a new project, git clone the Aery32 framework into the desired directory (or use the download link above)

    git clone git://github.com/aery32/aery32.git myproject
    cd myproject
    
Put your C source files under `src/` directory and define those in [sources variable in Makefile](https://github.com/aery32/aery32/blob/master/Makefile#L55). Header files you can put under `include/` or `src/`. You can start by editing `src/main.c`. If you prefer C++ over C, change the [CC variable in Makefile](https://github.com/aery32/aery32/blob/master/Makefile#L66) to avr32-g++.

To compile the project just call

    make
    
and `make re` when you want to recompile all.

To program or upload the compiled and linked binary to your printed circuit board command

    make program
    
If you also like to start the board immediately, chain with the start target

    make program start
    
Or less verbosely: `make programs`. The latter is also quicker with batchisp (in Windows).

## License

- Aery32 software framework is licensed under the new BSD license
- Example applications placed under examples/ directory are public domain
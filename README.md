    
      _____             ___ ___   |
     |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
     |     | -_|  _| | |_  |  _|  |  
     |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
                   |___|          |

Aery32 is a starting point for new AVR32-based projects. It provides a complete project structure and library enabling rapid prototyping and development. Aery32 aims to be professionally fun making it ideal framework for Hobbyist, R&D Engineers and Academics.

## Installation

You don't have to install Aery32 as you would do with regular software. Just download the framework, unzip and start working.

## Requirements

- Atmel AVR Toolchain
- Batchisp (for Windows) or DFU-programmer (for OS X & Linux)

For Windows download and install [Atmel AVR Toolchain for Windows](http://www.atmel.com/tools/ATMELAVRTOOLCHAIN3_3_2FORWINDOWS.aspx). For Mac OS X & Linux use [AVR32 Toolchain Builder](https://github.com/jsnyder/avr32-toolchain). Batchisp comes with the [FLIP](http://www.atmel.com/tools/FLIP.aspx) installer. OS X and Linux users can install DFU-programmer with the AVR32 Toolchain Builder.
 

## Quick start

To start a new project, git clone Aery32 into the desired directory (or use the download link above)

    git clone git://github.com/aery32/aery32.git myproject
    cd myproject
    
Put your C source files under `src/` directory and define those in [sources variable in Makefile](https://github.com/aery32/aery32/blob/master/Makefile#L55). Header files you can put under `include/` or `src/`. Start by editing `src/main.c`. Board initialization sequence can be found from `board.c` and the other defined settings from `board.h`. If you prefer C++ over C, change the [CC variable in Makefile](https://github.com/aery32/aery32/blob/master/Makefile#L66) to 'avr32-g++'. In this case you also have to change the specific language standard used by GCC to 'gnu++98'. This is made by modifying the '-std' option in [CFLAGS variable](https://github.com/aery32/aery32/blob/master/Makefile#L68).

To compile the project just call

    make

When you want to recompile all use

    make re

To program or upload the compiled and linked binary to your printed circuit board command

    make program
    
If you also like to start the board immediately, chain the program target with the start target

    make program start
    
Or less verbosely `make programs`. The latter is also quicker with batchisp (in Windows).

If you have [Doxygen](http://www.stack.nl/~dimitri/doxygen/) installed you can make a local api documentation by just running Doxygen at the project root

    doxygen
    
Otherwise if felt lazy, navigate to http://devzone.aery32.com/aery32/apidoc/.

## License

- Aery32 software framework is licensed under the new BSD license
- Example applications placed under examples/ directory are public domain
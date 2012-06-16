Aery32 library
==============

This folder contains the library of Aery32 Software Framework for AVR32 microcontrollers. Source files are placed directly here under the root directory. Header files (.h and .hh) are located at the ``aery32/`` subdirectory.

To compile the library just call

    make

The default MCU part that has been used is uc3a1128. To change this, use the environment variable after the make command

    make MPART="uc3b1256"

When done a ``libaery32_<mpart>.a" archive file has been created, where <mpart> is the defined MCU part name. If you have a standalone project, copy this file under your project root and then you can pass it to the avr32-gcc when compiling your project, like this

    avr32-gcc -std=gnu99 -mpart=uc3b1256 -I. main.c foo.c bar.c libaery32_uc3b1256.a   -o binary.elf

The include ``-I.`` assumes that you copied Aery32 header files, that's the ``aery32/`` directory, under your project root directory too. For the sake of clarity this imaginary project root directory could look like this

    your_project/
	    aery32/
	        delay.h
	        ...
	    bar.c
	    bar.h
	    foo.c
	    foo.h
	    libaery32_uc3b1256.a

Other make targets are

    make clean

to remove the compiled .o, .d and .a files from this root directory.

    make re

recompiles the library

    make qa

recompiles the library with additional warning options.

LICENSE
-------

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

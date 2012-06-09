#
#  _____             ___ ___   |
# |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
# |     | -_|  _| | |_  |  _|  |  
# |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
#               |___|          |
#
# Usage of this Makefile:
# 
#   - Put your C source files under src/ directory and define those in
#     sources variable.
#   - Remember to change mpart if necessary.
#   - That's it.
#
#
# Copyright (c) 2012, Muiku Oy
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
#    * Redistributions of source code must retain the above copyright notice,
#      this list of conditions and the following disclaimer.
#
#    * Redistributions in binary form must reproduce the above copyright notice,
#      this list of conditions and the following disclaimer in the documentation
#      and/or other materials provided with the distribution.
#
#    * Neither the name of Muiku Oy nor the names of its contributors may be
#      used to endorse or promote products derived from this software without
#      specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

vpath %.c src
vpath %.S src
vpath %.h src include
vpath %.a lib

projectname=myaery32
version=0.1
mpart=uc3a1128

includes=include
sources=main.c board.c # Define your source files here
sources_aery32=exception.S gpio.c interrupts.c pm.c rtc.c spi.c

objdir=obj
libdir=lib


# ----------------------------------------------------------------------
# Standard user variables
# ----------------------------------------------------------------------

CC=avr32-gcc
CSTD=gnu99

CFLAGS=-std=$(CSTD) -Wall -O2 -mpart=$(mpart) \
       -fdata-sections -ffunction-sections $(addprefix -I,$(includes))
# Enables global shortcuts, e.g. porta is a pointer to AVR32_GPIO.port[0]
CFLAGS+=-DAERY_SHORTCUTS
# Provides Atmel ASF compatibility
#CFLAGS+=-DUSER_BOARD

LDFLAGS=-mpart=$(mpart) \
        -Tldscripts/avr32elf_$(mpart).x

# Linker relaxing - if gcc is used as a frontend for the linker, this option
# is automaticly passed to the linker when using -O2 or -O3 (AVR32006 p. 4)
#LDFLAGS += -mrelax


# ----------------------------------------------------------------------
# Preparations for the build
# ----------------------------------------------------------------------

# Resolve object files from source files
objects=$(sources:.c=.o)
objects:=$(objects:.S=.o)
objects_aery32=$(sources_aery32:.c=.o)
objects_aery32:=$(objects_aery32:.S=.o)

# Direct all object files into the directory given above, $(objdir)
objects:=$(addprefix $(objdir)/,$(objects))
objects_aery32:=$(addprefix $(objdir)/aery32/,$(objects_aery32))

# Define the Aery32 library file
libaery32=libaery32_$(mpart).a
libaery32:=$(addprefix $(libdir)/,$(libaery32))


# ----------------------------------------------------------------------
# Build targets
# ----------------------------------------------------------------------

all: $(projectname).hex
	@echo Program size:
	@make -s size

$(objdir)/%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(objdir)/%.o: %.S
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(projectname).hex: $(projectname).elf
	avr32-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

$(projectname).elf: $(objects) $(libaery32)
	$(CC) $(LDFLAGS) -o $@ $^

$(projectname).lst: $(projectname).elf
	avr32-objdump -h -S $< > $@

$(libaery32): $(objects_aery32) | $(libdir)
	avr32-ar rv $@ $^
	avr32-ranlib $@

# Object file dependencies
$(objdir)/board.o: board.h
$(objects): | $(objdir)
$(objects_aery32): | $(objdir)

# Create directories
$(objdir):
	-mkdir $@
	-cd $@ && mkdir aery32
$(libdir):
	-mkdir $@


# ----------------------------------------------------------------------
# Chip programming
# ----------------------------------------------------------------------
.PHONY: program start programs dump-userdata update-userdata

# Grab the name of the Operating System
OS = $(shell uname)

# Select the programmer according to OS
ifeq ($(OS), Linux)
PROGRAMMER=dfu
else
PROGRAMMER=batchisp
endif

program: $(PROGRAMMER)-program
start: $(PROGRAMMER)-start
programs: $(PROGRAMMER)-program-start
dump-userdata: $(PROGRAMMER)-dump-userdata


# ----------------------------------------------------------------------
# Chip programming targets for batchisp/batchisp (Windows)
# ----------------------------------------------------------------------
.PHONY: batchisp-program batchisp-start batchisp-program-start \
		batchisp-dump-userdata batchisp-update-userdata

batchisp-program: $(projectname).hex
	batchisp -device at32$(mpart) -hardware usb -operation erase f \
	memory flash blankcheck loadbuffer $< program verify

batchisp-start:
	batchisp -device at32$(mpart) -hardware usb -operation start reset 0

batchisp-program-start: $(projectname).hex
	batchisp -device at32$(mpart) -hardware usb -operation erase f \
	memory flash blankcheck loadbuffer $< program verify start reset 0

batchisp-dump-userdata:
	batchisp -device at32$(mpart) -hardware usb -operation memory user \
	read savebuffer user.data hex386
	cat user.data

# Programs ISP_IO_COND_PIN to be PC04, which is GPIO pin number 69,
# which is 0x45 in hex. Thus "fillbuffer 0x45". The 0x94 is CRC for
# the whole userdata which is 0x929E45.
batchisp-update-userdata:
	batchisp -device at32$(mpart) -hardware usb -operation erase f \
	memory user addrange 0x1FC 0x1FC fillbuffer 0x92 program
	batchisp -device at32$(mpart) -hardware usb -operation erase f \
	memory user addrange 0x1FD 0x1FD fillbuffer 0x9E program
	batchisp -device at32$(mpart) -hardware usb -operation erase f \
	memory user addrange 0x1FE 0x1FE fillbuffer 0x45 program
	batchisp -device at32$(mpart) -hardware usb -operation erase f \
	memory user addrange 0x1FF 0x1FF fillbuffer 0x94 program


# ----------------------------------------------------------------------
# Chip programming targets for dfu-programmer (Linux)
# ----------------------------------------------------------------------
.PHONY: dfu-program dfu-start dfu-program-start dfu-dump-userdata

dfu-program: $(projectname).hex
	dfu-programmer at32$(mpart) erase
	dfu-programmer at32$(mpart) flash $<

dfu-start:
	dfu-programmer at32$(mpart) start

dfu-program-start: dfu-program dfu-start

dfu-dump-userdata:
	dfu-programmer at32$(mpart) dump-user


# ----------------------------------------------------------------------
# Other supportive tasks
# ----------------------------------------------------------------------
.PHONY: list size clean re debug qa apidoc refguide strip dist

list: $(projectname).lst

size: $(projectname).elf $(projectname).hex
	avr32-size -B $^

clean:
	-rm -f $(addprefix $(projectname),.elf .hex .lst) user.data
	-rm -f $(libaery32)
	-rm -rf $(objdir)

re: clean all

debug: clean all
debug: CFLAGS += -g3 -DDEBUG

qa: clean all
qa: CFLAGS += -pedantic -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings

dist: clean
	bsdtar -C ../ -czvf $(projectname)_v$(version).tar.gz \
			$(shell basename $(shell pwd))

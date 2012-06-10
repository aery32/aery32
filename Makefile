#
#  _____             ___ ___   |
# |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
# |     | -_|  _| | |_  |  _|  |  
# |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
#               |___|          |
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

# Project name and version
PROJECT=myaery32
VERSION=0.1

# MCU part name
MPART=uc3a1128

# Project's .c source files, grab all under the project root
SOURCES=$(wildcard *.c)

# Additional include paths
INCLUDES=aery32

# Where to put .o object files
OBJDIR=obj

# Grab the name of the Operating System
OS=$(shell uname)


# ----------------------------------------------------------------------
# Standard user variables
# ----------------------------------------------------------------------

CC=avr32-gcc
CSTD=gnu99

CFLAGS=-std=$(CSTD) -Wall -O2 -mpart=$(MPART) \
       -fdata-sections -ffunction-sections $(addprefix -I,$(INCLUDES))
# Enables global shortcuts, e.g. porta is a pointer to AVR32_GPIO.port[0]
CFLAGS+=-DAERY_SHORTCUTS
# Provides Atmel ASF compatibility
#CFLAGS+=-DUSER_BOARD

LDFLAGS=-mpart=$(MPART) \
        -Taery32/ldscripts/avr32elf_$(MPART).x

# Linker relaxing - if gcc is used as a frontend for the linker, this option
# is automaticly passed to the linker when using -O2 or -O3 (AVR32006 p. 4)
#LDFLAGS += -mrelax


# ----------------------------------------------------------------------
# Build targets
# ----------------------------------------------------------------------

# Resolve object files from source files
OBJECTS=$(SOURCES:.c=.o)
OBJECTS:=$(OBJECTS:.S=.o)

# Append object files with $(OBJDIR)
OBJECTS:=$(addprefix $(OBJDIR)/,$(OBJECTS))

# Resolve the nested object directories that has to be created
OBJDIRS=$(sort $(dir $(OBJECTS)))

.PHONY: all
all: $(PROJECT).hex
	@echo Program size:
	@make -s size

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $<   -c -o $@

$(OBJDIR)/%.o: %.S
	$(CC) $(CFLAGS) $(CPPFLAGS) $<   -c -o $@

$(PROJECT).hex: $(PROJECT).elf
	avr32-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

$(PROJECT).elf: $(OBJECTS) aery32/libaery32_$(MPART).a
	$(CC) $(LDFLAGS) $^   -o $@

$(PROJECT).lst: $(PROJECT).elf
	avr32-objdump -h -S $< > $@

aery32/libaery32_$(MPART).a:
	$(MAKE) -C aery32 CC=$(CC) CSTD=$(CSTD) CFLAG_OPTS="-DAERY_SHORTCUTS" MPART=$(MPART)

# Object file dependencies
$(OBJDIR)/board.o: board.h
$(OBJECTS): | $(OBJDIRS)

# Create directories where to place object files
$(OBJDIRS):
ifneq (, $(filter $(OS), windows32))
	-mkdir $(subst /,\,$(filter-out ./, $@))
else
	-mkdir -p $(filter-out ./, $@)
endif


# ----------------------------------------------------------------------
# Chip programming
# ----------------------------------------------------------------------
.PHONY: program start programs dump-userdata update-userdata

# Select the programmer according to OS
ifneq (, $(filter $(OS), windows32))
PROGRAMMER=batchisp
else
PROGRAMMER=dfu
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

batchisp-program: $(PROJECT).hex
	batchisp -device at32$(MPART) -hardware usb -operation erase f \
	memory flash blankcheck loadbuffer $< program verify

batchisp-start:
	batchisp -device at32$(MPART) -hardware usb -operation start reset 0

batchisp-program-start: $(PROJECT).hex
	batchisp -device at32$(MPART) -hardware usb -operation erase f \
	memory flash blankcheck loadbuffer $< program verify start reset 0

batchisp-dump-userdata:
	batchisp -device at32$(MPART) -hardware usb -operation memory user \
	read savebuffer user.data hex386
	cat user.data

# Programs ISP_IO_COND_PIN to be PC04, which is GPIO pin number 69,
# which is 0x45 in hex. Thus "fillbuffer 0x45". The 0x94 is CRC for
# the whole userdata which is 0x929E45.
batchisp-update-userdata:
	batchisp -device at32$(MPART) -hardware usb -operation erase f \
	memory user addrange 0x1FC 0x1FC fillbuffer 0x92 program
	batchisp -device at32$(MPART) -hardware usb -operation erase f \
	memory user addrange 0x1FD 0x1FD fillbuffer 0x9E program
	batchisp -device at32$(MPART) -hardware usb -operation erase f \
	memory user addrange 0x1FE 0x1FE fillbuffer 0x45 program
	batchisp -device at32$(MPART) -hardware usb -operation erase f \
	memory user addrange 0x1FF 0x1FF fillbuffer 0x94 program


# ----------------------------------------------------------------------
# Chip programming targets for dfu-programmer (Linux)
# ----------------------------------------------------------------------
.PHONY: dfu-program dfu-start dfu-program-start dfu-dump-userdata

dfu-program: $(PROJECT).hex
	dfu-programmer at32$(MPART) erase
	dfu-programmer at32$(MPART) flash $<

dfu-start:
	dfu-programmer at32$(MPART) start

dfu-program-start: dfu-program dfu-start

dfu-dump-userdata:
	dfu-programmer at32$(MPART) dump-user


# ----------------------------------------------------------------------
# Other supportive tasks
# ----------------------------------------------------------------------
.PHONY: list size debug qa dist clean cleanll re reall

list: $(PROJECT).lst

size: $(PROJECT).elf $(PROJECT).hex
	avr32-size -B $^

clean:
	-rm -f *.o $(addprefix $(PROJECT),.elf .hex .lst) user.data
	-rm -rf $(filter-out ./, $(OBJDIRS))

cleanall: clean
	-$(MAKE) -C aery32 clean

re: clean all

reall: cleanall all

debug: clean all
debug: CFLAGS += -g3 -DDEBUG

qa: clean all
qa: CFLAGS += -pedantic -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings

dist: clean
	bsdtar -C ../ -czvf $(PROJECT)_v$(version).tar.gz \
	$(shell basename $(shell pwd))

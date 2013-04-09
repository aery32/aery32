#
#  _____             ___ ___   |
# |  _  |___ ___ _ _|_  |_  |  |  C/C++ framework for 32-bit AVRs
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

# Project name
TARGET=aery32

# MPU (Microprocessor Unit) type
MPART=uc3a1256

# Project's source files
SOURCES=$(wildcard *.cpp) $(wildcard *.c)
EXCLUDE=

# Global project wide settings file. IMPORTANT! Define with absolute path.
SETTINGS=$(CURDIR)/settings.h

# Additional include paths
INCLUDES=aery32


# ----------------------------------------------------------------------
# Some stuff that has to be done
# ----------------------------------------------------------------------

# Grab the name of the Operating System
OS=$(shell uname)

# Remove excluded source files
SOURCES:=$(filter-out $(EXCLUDE),$(SOURCES))

# Resolve object files from source files
OBJECTS=$(SOURCES:.cpp=.o)
OBJECTS:=$(OBJECTS:.c=.o)

# Escape possible space characters in settings path. Needed in Linux.
ifeq (, $(filter $(OS), windows32))
sp:=
sp+=
SETTINGS:=$(subst $(sp),\ ,$(SETTINGS))
endif


# ----------------------------------------------------------------------
# Standard user variables
# ----------------------------------------------------------------------

CC=avr32-gcc
CXX=avr32-g++

CSTANDARD=gnu99
CXXSTANDARD=gnu++98

COPT=-O2 -fdata-sections -ffunction-sections
CXXOPT=$(COPT) -fno-exceptions -fno-rtti

CFLAGS=-mpart=$(MPART) -std=$(CSTANDARD) $(COPT) -Wall
CFLAGS+=$(addprefix -I,$(INCLUDES))
CFLAGS+=-include "$(SETTINGS)"

CXXFLAGS=-mpart=$(MPART) -std=$(CXXSTANDARD) $(CXXOPT) -Wall
CXXFLAGS+=$(addprefix -I,$(INCLUDES))
CXXFLAGS+=-include "$(SETTINGS)"

LDFLAGS=-mpart=$(MPART) -Taery32/ldscripts/avr32elf_$(MPART).x
LDFLAGS+=-Wl,--gc-sections
LDFLAGS+=-Wl,-Map=$(TARGET).map,--cref
#LDFLAGS+=--rodata-writable --direct-data

# Linker relaxing - if gcc is used as a frontend for the linker, this option
# is automaticly passed to the linker when using -O2 or -O3 (AVR32006 p. 4)
#LDFLAGS+=-mrelax


# ----------------------------------------------------------------------
# Build targets
# ----------------------------------------------------------------------

.PHONY: all
all: $(TARGET).hex $(TARGET).lst
	@echo Program size:
	@$(MAKE) -s size

$(TARGET).elf: $(OBJECTS) aery32/libaery32_$(MPART).a
	$(CXX) $(LDFLAGS) $^ -lm   -o $@

$(TARGET).hex: $(TARGET).elf
	avr32-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

aery32/libaery32_$(MPART).a:
	"$(MAKE)" -C aery32 MPART="$(MPART)" CXXOPT="$(CXXOPT)" SETTINGS="$(SETTINGS)"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -MF $(@:%.o=%.d) $<   -c -o $@

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -MF $(@:%.o=%.d) $<   -c -o $@

$(TARGET).lst: $(TARGET).elf
	avr32-objdump -h -S $< > $@

# Add dependency lists, .d files
-include $(OBJECTS:.o=.d)


# ----------------------------------------------------------------------
# Chip programming
# ----------------------------------------------------------------------
.PHONY: program start programs dump-user dump-fuses

# Select the programmer according to OS
ifneq (, $(filter $(OS), windows32))
PROGRAMMER=batchisp
else
PROGRAMMER=dfu
endif

program: $(PROGRAMMER)-program
start: $(PROGRAMMER)-start
programs: $(PROGRAMMER)-programs
dump-user: $(PROGRAMMER)-dump-user
dump-fuses: $(PROGRAMMER)-dump-fuses


# ----------------------------------------------------------------------
# Chip programming targets for batchisp/batchisp (Windows)
# ----------------------------------------------------------------------
.PHONY: batchisp-program batchisp-start batchisp-programs \
	batchisp-dump-userdata batchisp-dump-fuses

BATCHISP=batchisp -device at32$(MPART) -hardware usb

batchisp-program: $(TARGET).hex
	$(BATCHISP) -operation erase f memory flash blankcheck \
	loadbuffer $< program verify

batchisp-start:
	$(BATCHISP) -operation start reset 0

batchisp-programs: $(TARGET).hex
	$(BATCHISP) -operation erase f memory flash blankcheck \
	loadbuffer $< program verify start reset 0

batchisp-dump-user:
	$(BATCHISP) -operation memory user read savebuffer userpage.hex hex386
	cat userpage.hex

batchisp-dump-fuses:
	$(BATCHISP) -operation memory configuration read savebuffer fusebits.hex hex386
	cat fusebits.hex


# ----------------------------------------------------------------------
# Chip programming targets for dfu-programmer (Linux)
# ----------------------------------------------------------------------
.PHONY: dfu-program dfu-start dfu-program-start dfu-dump-user

dfu-program: $(TARGET).hex
	dfu-programmer at32$(MPART) erase
	dfu-programmer at32$(MPART) flash $<

dfu-start:
	dfu-programmer at32$(MPART) start

dfu-programs: dfu-program dfu-start

dfu-dump-user:
	dfu-programmer at32$(MPART) dump-user


# ----------------------------------------------------------------------
# Other supportive tasks
# ----------------------------------------------------------------------
.PHONY: size debug qa clean cleanall re reall

size: $(TARGET).elf $(TARGET).hex
	@avr32-size -B $^
ifneq (, $(filter $(OS), windows32))
	@avr32-size -A aery32.elf | awk "$$0 ~ /.heap/" | awk -F" " "{a=64*1024-$$2; print \"SDRAM usage:\", a, \"bytes,\", 100*a/(64*1024), \"%%\"}"
else
	@avr32-size -A aery32.elf | awk '$$0 ~ /.heap/' | awk -F" " '{a=64*1024-$$2; print "SDRAM usage:", a, "bytes,", 100*a/(64*1024), "%"}'
endif

clean:
	-rm -f $(addprefix $(TARGET),.elf .hex .lst .map)
	-rm -f $(OBJECTS) $(OBJECTS:.o=.d)

cleanall: clean
	-"$(MAKE)" -C aery32 clean
	
re: clean all

reall: cleanall all

debug: reall
debug: COPT+=-g -O0 -DDEBUG
debug: LDFLAGS+=-mrelax

qa: re
qa: CFLAGS+=-pedantic -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -Winline -Wextra
qa: CXXFLAGS+=-pedantic -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -Winline -Wextra

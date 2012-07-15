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

# Project name and version
PROJECT=myaery32

# MCU part name
MPART=uc3a1128

# Project's .cpp source files, grab all under the project root
SOURCES=$(wildcard *.cpp)

# Additional include paths
INCLUDES=aery32

# Where to put .o object files
OBJDIR=obj


# ----------------------------------------------------------------------
# Standard user variables
# ----------------------------------------------------------------------

CXX=avr32-g++
CPPSTANDARD=gnu++98
OPTIMIZATION=-O2 -fdata-sections -ffunction-sections
OPTIMIZATION+=-fno-exceptions -fno-rtti

CPPFLAGS=-mpart=$(MPART) -std=$(CPPSTANDARD) $(OPTIMIZATION) -Wall
CPPFLAGS+=$(addprefix -I,$(INCLUDES))

LDFLAGS=-mpart=$(MPART) -Taery32/ldscripts/avr32elf_$(MPART).x
LDFLAGS+=-Wl,--gc-sections # Discards unused sections
#LDFLAGS+=--rodata-writable --direct-data

# Linker relaxing - if gcc is used as a frontend for the linker, this option
# is automaticly passed to the linker when using -O2 or -O3 (AVR32006 p. 4)
#LDFLAGS+=-mrelax

# Add Math library
#LDFLAGS+=-lm


# ----------------------------------------------------------------------
# Build targets
# ----------------------------------------------------------------------

# Grab the name of the Operating System
OS=$(shell uname)

# Resolve object files from source files
OBJECTS=$(SOURCES:.cpp=.o)

# Append object files with $(OBJDIR)
OBJECTS:=$(addprefix $(OBJDIR)/,$(OBJECTS))

# Resolve the nested object directories that has to be created
OBJDIRS=$(sort $(dir $(OBJECTS)))
OBJDIRS:=$(filter-out ./,$(OBJDIRS)) # Filter the root dir out, that's "./"

.PHONY: all
all: $(PROJECT).hex $(PROJECT).lst
	@echo Program size:
	@make -s size

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CPPFLAGS) -MMD -MP -MF $(@:%.o=%.d) $<   -c -o $@

$(PROJECT).elf: $(OBJECTS) aery32/libaery32_$(MPART).a
	$(CXX) $(LDFLAGS) $^   -o $@

$(PROJECT).hex: $(PROJECT).elf
	avr32-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

aery32/libaery32_$(MPART).a:
	$(MAKE) -C aery32 MPART="$(MPART)" OPTIMIZATION="$(OPTIMIZATION)"

$(PROJECT).lst: $(PROJECT).elf
	avr32-objdump -h -S $< > $@

# Create directories where to place object files
$(OBJECTS): | $(OBJDIRS)
$(OBJDIRS):
ifneq (, $(filter $(OS), windows32))
	-mkdir $(subst /,\,$@)
else
	-mkdir -p $@
endif

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
		batchisp-dump-userdata batchisp-dump-fuses batchisp-program-user 

BATCHISP=batchisp -device at32$(MPART) -hardware usb

batchisp-program: $(PROJECT).hex
	$(BATCHISP) -operation erase f memory flash blankcheck \
	loadbuffer $< program verify

batchisp-start:
	$(BATCHISP) -operation start reset 0

batchisp-programs: $(PROJECT).hex
	$(BATCHISP) -operation erase f memory flash blankcheck \
	loadbuffer $< program verify start reset 0

# Programs ISP_IO_COND_PIN to be PC04, which is GPIO pin number 69,
# which is 0x45 in hex. Thus "fillbuffer 0x45". The 0x94 is CRC for
# the whole userdata which is 0x929E45.
batchisp-program-user:
	$(BATCHISP) -operation memory user addrange 0x1FC 0x1FC fillbuffer 0x92 program
	$(BATCHISP) -operation memory user addrange 0x1FD 0x1FD fillbuffer 0x9E program
	$(BATCHISP) -operation memory user addrange 0x1FE 0x1FE fillbuffer 0x45 program
	$(BATCHISP) -operation memory user addrange 0x1FF 0x1FF fillbuffer 0x94 program

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

dfu-program: $(PROJECT).hex
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
.PHONY: list size debug qa clean cleanall re reall

list: $(PROJECT).lst

size: $(PROJECT).elf $(PROJECT).hex
	avr32-size -B $^

clean:
	-rm -f $(addprefix $(PROJECT),.elf .hex .lst) userpage.hex fusebits.hex
	-rm -rf $(OBJDIR)

cleanall: clean
	-$(MAKE) -C aery32 clean

re: clean all

reall: cleanall all

debug: reall
debug: OPTIMIZATION=-O0 -g -DDEBUG

qa: re
qa: CPPFLAGS+=-pedantic -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -Winline

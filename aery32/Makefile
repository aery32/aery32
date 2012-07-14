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

# Part name
MPART=uc3a1128

# Source files
SOURCES=adc.cpp gpio.cpp intc.cpp pm.cpp rtc.cpp spi.cpp n2str.cpp exception.S


# ----------------------------------------------------------------------
# Standard user variables
# ----------------------------------------------------------------------

CXX=avr32-g++
CPPSTANDARD=gnu++98
OPTIMIZATION=-O2 -fdata-sections -ffunction-sections

CPPFLAGS=-mpart=$(MPART) -std=$(CPPSTANDARD) $(OPTIMIZATION) -Wall
#CPPFLAGS+=-DF_OSC0=12000000UL -DF_OSC1=16000000UL

ASFLAGS=-mpart=$(MPART)


# ----------------------------------------------------------------------
# Build targets
# ----------------------------------------------------------------------

# Resolve object files from source files
OBJECTS=$(SOURCES:.cpp=.o)
OBJECTS:=$(OBJECTS:.S=.o)

all: libaery32_$(MPART).a

libaery32_$(MPART).a: $(OBJECTS)
	avr32-ar rv $@ $^
	avr32-ranlib $@

%.o : %.cpp
	$(CXX) $(CPPFLAGS) -MMD -MP -MF $(@:%.o=%.d) $<   -c -o $@

%.o : %.S
	$(CXX) $(ASFLAGS) -MMD -MP -MF $(@:%.o=%.d) $<   -c -o $@

# Add dependency lists, .d files
-include $(OBJECTS:.o=.d)


# ----------------------------------------------------------------------
# Other supportive tasks
# ----------------------------------------------------------------------
.PHONY: clean re debug qa size

clean:
	-rm -f *.o *.a *.d

re: clean all

qa: re size
qa: CPPFLAGS+=-pedantic -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -Winline

size: libaery32_$(MPART).a
	avr32-size -B $^
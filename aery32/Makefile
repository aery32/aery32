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

# Part name
MPART=uc3a1128

# Source files
SOURCES=exception.S gpio.c intc.c pm.c rtc.c spi.c


# ----------------------------------------------------------------------
# Standard user variables
# ----------------------------------------------------------------------

CC=avr32-gcc
CSTD=gnu99

CFLAGS=-std=$(CSTD) -Wall -O2 -mpart=$(MPART) \
       -fdata-sections -ffunction-sections
# Enables global shortcuts, e.g. porta is a pointer to AVR32_GPIO.port[0]
#CFLAGS+=-DAERY_SHORTCUTS
CFLAGS+=$(CFLAG_OPTS)

ASFLAGS=-mpart=$(MPART)
ASFLAGS+=$(ASFLAG_OPTS)


# ----------------------------------------------------------------------
# Build targets
# ----------------------------------------------------------------------

# Resolve object files from source files
OBJECTS=$(SOURCES:.c=.o)
OBJECTS:=$(OBJECTS:.S=.o)

all: libaery32_$(MPART).a

libaery32_$(MPART).a: $(OBJECTS)
	avr32-ar rv $@ $^
	avr32-ranlib $@

%.o : %.c
	$(CC) $(CFLAGS) -MMD -MP -MF $(<:%.c=%.d) $<   -c -o $@

%.o : %.S
	$(CC) $(ASFLAGS) -MMD -MP -MF $(<:%.S=%.d) $<   -c -o $@

# Add dependency lists, .d files
-include $(OBJECTS:.o=.d)


# ----------------------------------------------------------------------
# Other supportive tasks
# ----------------------------------------------------------------------
.PHONY: clean re debug qa

clean:
	-rm -f *.o *.a *.d

re: clean all

debug: clean all
debug: CFLAGS += -g3 -DDEBUG

qa: clean all
qa: CFLAGS += -pedantic -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings

#
#   This file is part of IridiumCarrierBoard.
#
#   IridiumCarrierBoard is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   IridiumCarrierBoard is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with IridiumCarrierBoard.  If not, see <http://www.gnu.org/licenses/>.
#
#
# Tested using avr-gcc (WinAVR 20100110) 4.3.3
#
# WinAVR makefile written by Eric B. Weddington, Jörg Wunsch, et al.
# Released to the Public Domain
# Please read the make user manual!
#
# Additional material for this makefile was submitted by:
#  Tim Henigan
#  Peter Fleury
#  Reiner Patommel
#  Sander Pool
#  Frederik Rouleau
#  Markus Pfaff
#
# On command line:
#
# make all = Make software.
#
# make clean = Clean out built project files.
#
# make coff = Convert ELF to AVR COFF (for use with AVR Studio 3.x or VMLAB).
#
# make extcoff = Convert ELF to AVR Extended COFF (for use with AVR Studio
#                4.07 or greater).
#
# make program = Download the hex file to the device, using avrdude.  Please
#                customize the avrdude settings below first!
#
# To rebuild project do "make clean" then "make all".
#
#

# MCU name
MCU = atmega162
MCU_PROG = m162

# Output format. (can be srec, ihex, binary)
FORMAT = ihex

# Target file name (without extension).
TARGET_AIR = iridium_air
TARGET_GROUND = iridium_ground


# Optimization level, can be [0, 1, 2, 3, s]. 0 turns off optimization.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = 2

# List C source files here. (C dependencies are automatically generated.)
SRC = iridium.c usart.c

# Optional compiler flags.
#  -g:        generate debugging information (for GDB, or for COFF conversion)
#  -O*:       optimization level
#  -f...:     tuning, see gcc manual and avr-libc documentation
#  -Wall...:  warning level
#  -Wa,...:   tell GCC to pass this to the assembler.
#    -ahlms:  create assembler listing
CFLAGS_COMMON = -g -O$(OPT) \
-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums \
-Wall -Wstrict-prototypes


# Set a "language standard" compiler flag.
#   Unremark just one line below to set the language standard to use.
#   gnu99 = C99 + GNU extensions. See GCC manual for more information.
CFLAGS_COMMON += -std=gnu99
CFLAGS_COMMON += -DF_CPU=7372800UL -DARCH=stm32
CFLAGS_COMMON += -mmcu=$(MCU) -I.
CFLAGS_AIR = $(CFLAGS_COMMON) -DMODEM_TYPE=air
CFLAGS_AIR += -Wa,-adhlns=$(<:%.c=air/%.lst)
CFLAGS_GROUND = $(CFLAGS_COMMON) -DMODEM_TYPE=ground
CFLAGS_GROUND += -Wa,-adhlns=$(<:%.c=ground/%.lst)

# Additional libraries

# Minimalistic printf version
#LDFLAGS += -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires -lm below)
LDFLAGS_COMMON = -Wl,-u,vfprintf -lprintf_flt

# -lm = math library
LDFLAGS_COMMON += -lm

# Optional linker flags.
#  -Wl,...:   tell GCC to pass this to linker.
#  -Map:      create map file
#  --cref:    add cross reference to  map file
LDFLAGS_AIR = $(LDFLAGS_COMMON) -Wl,-Map=$(TARGET_AIR).map,--cref
LDFLAGS_GROUND = $(LDFLAGS_COMMON) -Wl,-Map=$(TARGET_GROUND).map,--cref


# Programming support using avrdude. Settings and variables.

# Programming hardware: alf avr910 avrisp bascom bsd
# dt006 pavr picoweb pony-stk200 sp12 stk200 stk500
#
# Type: avrdude -c ?
# to get a full listing.
#


# ---------------------------------------------------------------------------

# Define directories, if needed.
DIRVENDOR = ../../../../vendor
DIRAVR = c:/winavr
DIRAVRBIN = $(DIRAVR)/bin
DIRAVRUTILS = $(DIRAVR)/utils/bin
DIRINC = .
DIRLIB = $(DIRAVR)/avr/lib
WCREV = SubWCRev.exe

# Define programs and commands.
SHELL = sh

CC = avr-gcc

OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size


# Programming support using avrdude.
AVR_PROG ?= avrispmkII
AVRDUDE = avrdude -c $(AVR_PROG) -p $(MCU_PROG)
REMOVE = rm -f
COPY = cp


# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before:
MSG_SIZE_AFTER = Size after:
MSG_COFF = Converting to AVR COFF:
MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:




# Define all object files.
OBJ_AIR = $(SRC:%.c=air/%.o)
OBJ_GROUND = $(SRC:%.c=ground/%.o)

# Define all listing files.
LST = $(SRC:.c=.lst)

# Default targets
all: begin gccversion $(TARGET_AIR).hex $(TARGET_GROUND).hex finished end

air: begin gccversion $(TARGET_AIR).hex finished end
ground: begin gccversion $(TARGET_GROUND).hex finished end

# Eye candy.
# AVR Studio 3.x does not check make's exit code but relies on
# the following magic strings to be generated by the compile job.
begin:
	@echo
	@echo $(MSG_BEGIN)

finished:
	@echo $(MSG_ERRORS_NONE)

end:
	@echo $(MSG_END)
	@echo


# Display compiler version information.
gccversion :
	@$(CC) --version




# Convert ELF to COFF for use in debugging / simulating in
# AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
	--change-section-address .data-0x800000 \
	--change-section-address .bss-0x800000 \
	--change-section-address .noinit-0x800000 \
	--change-section-address .eeprom-0x810000


%.coff: iridum_%.elf
	@echo
	@echo $(MSG_COFF) iridum_%.cof
	$(COFFCONVERT) -O coff-avr $< iridum_%.cof


%.extcoff: iridum_%.elf
	@echo
	@echo $(MSG_EXTENDED_COFF) iridum_%.cof
	$(COFFCONVERT) -O coff-ext-avr $< iridum_%.cof


# Program the device.
ground_program: $(TARGET_GROUND).hex
	$(AVRDUDE) -U flash:w:$(TARGET_GROUND).hex

air_program: $(TARGET_AIR).hex
	$(AVRDUDE) -U flash:w:$(TARGET_AIR).hex

# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	avr-nm -n $< > $@



# Link: create ELF output file from object files.
.SECONDARY : $(TARGET_AIR).elf $(TARGET_GROUND).elf
.PRECIOUS : $(OBJ_AIR) $(OBJ_GROUND)
$(TARGET_AIR).elf: $(OBJ_AIR)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(CFLAGS_AIR) $(OBJ_AIR) --output $@ $(LDFLAGS_AIR)

$(TARGET_GROUND).elf: $(OBJ_GROUND)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(CFLAGS_GROUND) $(OBJ_GROUND) --output $@ $(LDFLAGS_GROUND)


# Compile: create object files from C source files.
air/%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(CFLAGS_AIR) $< -o $@

ground/%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(CFLAGS_GROUND) $< -o $@


# Target: clean project.
clean: begin clean_list finished end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) *.hex
	$(REMOVE) *.elf
	$(REMOVE) *.map
	$(REMOVE) air/*
	$(REMOVE) ground/*


# Automatically generate C source code dependencies.
# (Code originally taken from the GNU make user manual and modified
# (See README.txt Credits).)
#
# Note that this will work with sh (bash) and sed that is shipped with WinAVR
# (see the SHELL variable defined above).
# This may not work with other shells or other seds.
#
%.d: %.c
	set -e; $(CC) -MM $(ALL_CFLAGS) $< \
	| sed 's,\(.*\)\.o[ :]*,\1.o \1.d : ,g' > $@; \
	[ -s $@ ] || rm -f $@


# Remove the '-' if you want to see the dependency files generated.
ifneq ($(MAKECMDGOALS),clean)
-include $(SRC:%.c=air/%.d)
-include $(SRC:%.c=ground/%.d)
endif


# Listing of phony targets.
.PHONY : all begin finish end gccversion coff extcoff \
	clean clean_list program export


# This file is included in the Makefile rules for each directory of files to compile for PROS
# This file should only be edited by advanced users

DEVICE=VexCortex
# Libraries to include in the link (use -L and -l) e.g. -lm, -lmyLib
LIBRARIES=$(ROOT)/firmware/*.a -lgcc -lm
# Prefix for ARM tools (must be on the path)
MCUPREFIX=arm-none-eabi-
# Flags applied to the assembler, compiler, and linker command lines
MCUAFLAGS=-mthumb -mcpu=cortex-m3 -mlittle-endian
MCUCFLAGS=-mthumb -mcpu=cortex-m3 -mlittle-endian
MCULFLAGS=-nostartfiles -Bfirmware -Wl,-static -Wl,-u,VectorTable -Wl,-T -Xlinker firmware/cortex.ld
# Invokes the ELF to Binary copy utility to compress the elf file down into a flat binary image
MCUPREPARE=$(OBJCOPY) $(OUT) -O binary $(BINDIR)/$(OUTBIN)
SIZEFLAGS=
# Invokes the upload utility
UPLOAD=@java -jar firmware/uniflash.jar vex $(BINDIR)/$(OUTBIN)

# Advanced options to select file names and targets
ASMEXT=s
CEXT=c
CPPEXT=cpp
HEXT=h
INCLUDE=-I$(ROOT)/include -I$(ROOT)/src
OUTBIN=output.bin
OUTNAME=output.elf

# Advanced flags for the compiler and linker specifying optimization, warning, and error options
AFLAGS:=$(MCUAFLAGS)
ARFLAGS:=$(MCUCFLAGS)
CCFLAGS:=-c -Wall $(MCUCFLAGS) -Os -ffunction-sections -fsigned-char -fomit-frame-pointer -fsingle-precision-constant
CFLAGS:=$(CCFLAGS) -std=gnu99 -Werror=implicit-function-declaration
CPPFLAGS:=$(CCFLAGS) -fno-exceptions -fno-rtti -felide-constructors
LDFLAGS:=-Wall $(MCUCFLAGS) $(MCULFLAGS) -Wl,--gc-sections

# Aliases to the tools used
AR:=$(MCUPREFIX)ar
AS:=$(MCUPREFIX)as
CC:=$(MCUPREFIX)gcc
CPPCC:=$(MCUPREFIX)g++
OBJCOPY:=$(MCUPREFIX)objcopy

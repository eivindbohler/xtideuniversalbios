####################################################################################################
# Makefile to build XTIDE Universal BIOS.                                                          #
#                                                                                                  #
# Valid makefile targets are:                                                                      #
# all       Removes existing files and builds binary files in \Build (without checksums)           #
# small     Builds only binaries smaller than or equal to 8 kiB (without checksums)                #
# large     Builds only binaries larger than 8 kiB (without checksums)                             #
# custom    Builds custom binary without checksum (change DEFINES_CUSTOM / BIOS_SIZE_CUSTOM first) #
# clean     Removes all files from \Build                                                          #
# checksum* Same as 'all' but also applies checksums to the binaries                               #
# strings*  Compress src\Strings.asm to src\StringsCompressed.asm                                  #
# unused*   Checks if there are any unused functions that can be removed to save space             #
#                                                                                                  #
# * at the end of target name means that Perl is required for the job.                             #
# Build directory must be created manually if it does not exist.                                   #
#                                                                                                  #
#                                                                                                  #
# Following modules can be included or excluded:                                                   #
# MODULE_8BIT_IDE             Support for 8-BIT IDE cards like XTIDE                               #
# MODULE_8BIT_IDE_ADVANCED    Support for memory mapped and DMA based cards like JRIDE and XTCF    #
# MODULE_ADVANCED_ATA         Native support for some VLB IDE controllers (requires USE_386)       #
# MODULE_COMPATIBLE_TABLES    Support for ill behaving software that tries to access DPT directly  #
# MODULE_BOOT_MENU            Boot Menu for selection of drive to boot from                        #
# MODULE_EBIOS                Enhanced functions for accessing drives over 8.4 GB                  #
# MODULE_HOTKEYS              Hotkey Bar to boot from any drive                                    #
# MODULE_IRQ                  IDE IRQ support                                                      #
# MODULE_SERIAL               Virtual hard disks using serial port                                 #
# MODULE_SERIAL_FLOPPY        Virtual floppy drives using serial port (requires MODULE_SERIAL)     #
# MODULE_STRINGS_COMPRESSED   Use compressed strings to save space                                 #
# MODULE_VERY_LATE_INIT       Initialize on INT 13h if our INT 19h handler is not called           #
# MODULE_POWER_MANAGEMENT     Power Management support                                             #
# MODULE_WIN9X_CMOS_HACK      Hack for Windows 9x compatibility                                    #
# MODULE_MFM_COMPATIBILITY    Restores BDA drive count for MFM/SCSI controllers that expect to be  #
#                             the only hard drive controller in the system                         #
#                                                                                                  #
# Not modules but these affect the assembly:                                                       #
# ELIMINATE_CGA_SNOW          Prevents CGA snowing at the cost of a few bytes                      #
# RELOCATE_INT13H_STACK **    Relocates INT 13h stack to beginning of stolen conventional memory   #
# NO_ATAID_VALIDATION ***     Excludes code that tries to ensure proper communication with drives  #
# NO_ATAID_CORRECTION         Excludes code that corrects illegal CHS values from some CF cards    #
# USE_086                     Applies WORD alignment padding for use with 8086/V30 CPUs only       #
# USE_186                     Use instructions supported by 80188/80186 and V20/V30 and later      #
# USE_286                     Use instructions supported by 286 and later (defines USE_UNDOC_INTEL)#
# USE_386                     Use instructions supported by 386 and later (defines USE_286)        #
# USE_AT                      Use features supported on AT and later systems (not available on XT) #
# USE_PS2                     For use with IBM PS/2 machines only (defines USE_AT)                 #
# USE_UNDOC_INTEL             Optimizations for Intel CPUs - do NOT use on NEC V20/V30/Sony CPUs   #
# USE_NEC_V                   Optimizations for use with NEC V20/V30 CPUs only                     #
# CLD_NEEDED                  Only needed for compatibility with buggy software/BIOSes             #
# EXTRA_LOOP_UNROLLING_SMALL  Improves transfer speed on some CPU + IDE controller combinations    #
# EXTRA_LOOP_UNROLLING_LARGE  Same as above but faster and uses more ROM space                     #
#                                                                                                  #
# ** AT Builds only (when USE_AT is defined)                                                       #
# *** Use this only when certain known good drives are not being detected (eg WD Caviars)          #
####################################################################################################

###########################################
# Source files and destination executable #
###########################################

# Assembly source code file (*.asm):
SRC_ASM = Src/Main.asm

# Program executable file name without extension:
PROG = ide


#######################################
# Destination and include directories #
#######################################

# Directory where binary file will be compiled to
BUILD_DIR = Build

# Subdirectories where included files are:
HEADERS = Inc/
HEADERS += Inc/Controllers/
HEADERS += Src/
HEADERS += Src/Handlers/
HEADERS += Src/Handlers/Int13h/
HEADERS += Src/Handlers/Int13h/EBIOS/
HEADERS += Src/Handlers/Int13h/Tools/
HEADERS += Src/Handlers/Int19h/
HEADERS += Src/Device/
HEADERS += Src/Device/IDE/
HEADERS += Src/Device/MemoryMappedIDE/
HEADERS += Src/Device/Serial/
HEADERS += Src/Initialization/
HEADERS += Src/Initialization/AdvancedAta/
HEADERS += Src/Menus/
HEADERS += Src/Menus/BootMenu/
HEADERS += Src/Libraries/
HEADERS += Src/VariablesAndDPTs/

# Subdirectories where library files are:
LIBS = ../Assembly_Library/Inc/
LIBS += ../Assembly_Library/Src/
LIBS += ../Assembly_Library/Src/Display/
LIBS += ../Assembly_Library/Src/File/
LIBS += ../Assembly_Library/Src/Keyboard/
LIBS += ../Assembly_Library/Src/Menu/
LIBS += ../Assembly_Library/Src/Menu/Dialog/
LIBS += ../Assembly_Library/Src/String/
LIBS += ../Assembly_Library/Src/Time/
LIBS += ../Assembly_Library/Src/Util/
LIBS += ../Assembly_Library/Src/Serial/
LIBS += ../XTIDE_Universal_BIOS/Inc/
HEADERS += $(LIBS)


##################################
# Assembler preprocessor defines #
##################################
DEFINES_COMMON = MODULE_STRINGS_COMPRESSED MODULE_HOTKEYS MODULE_8BIT_IDE MODULE_EBIOS MODULE_SERIAL MODULE_SERIAL_FLOPPY MODULE_POWER_MANAGEMENT NO_ATAID_VALIDATION CLD_NEEDED EXTRA_LOOP_UNROLLING_SMALL
DEFINES_COMMON_LARGE = MODULE_BOOT_MENU MODULE_8BIT_IDE_ADVANCED MODULE_COMPATIBLE_TABLES EXTRA_LOOP_UNROLLING_LARGE

DEFINES_XT = $(DEFINES_COMMON) ELIMINATE_CGA_SNOW MODULE_8BIT_IDE_ADVANCED
DEFINES_XTPLUS = $(DEFINES_XT) USE_186
DEFINES_AT = $(DEFINES_COMMON) USE_AT USE_286 MODULE_IRQ MODULE_COMPATIBLE_TABLES
DEFINES_386 = $(DEFINES_AT) USE_386 MODULE_ADVANCED_ATA MODULE_WIN9X_CMOS_HACK
DEFINES_PS2 = USE_PS2 USE_286 MODULE_STRINGS_COMPRESSED MODULE_HOTKEYS MODULE_BOOT_MENU MODULE_EBIOS MODULE_SERIAL MODULE_SERIAL_FLOPPY MODULE_IRQ MODULE_POWER_MANAGEMENT NO_ATAID_VALIDATION CLD_NEEDED

DEFINES_XT_LARGE = $(DEFINES_XT) $(DEFINES_COMMON_LARGE)
DEFINES_XTPLUS_LARGE = $(DEFINES_XTPLUS) $(DEFINES_COMMON_LARGE)
DEFINES_AT_LARGE = $(DEFINES_AT) $(DEFINES_COMMON_LARGE)
DEFINES_386_LARGE = $(DEFINES_386) $(DEFINES_COMMON_LARGE)
DEFINES_PS2_LARGE = $(DEFINES_PS2) MODULE_COMPATIBLE_TABLES

DEFINES_XT_TINY = MODULE_STRINGS_COMPRESSED MODULE_8BIT_IDE NO_ATAID_VALIDATION NO_ATAID_CORRECTION CLD_NEEDED

DEFINES_CUSTOM = ?


###################
# Other variables #
###################

# Target size of the BIOS, used in Main.asm for number of 512B blocks (CNT_ROM_BLOCKS) and by checksum Perl script below ('make checksum').
# Note! The size must be a multiple of 2 KB for compatibility reasons.
BIOS_SIZE_TINY = 4096
BIOS_SIZE_SMALL = 8192
BIOS_SIZE_LARGE = 12288
BIOS_SIZE_CUSTOM = ?

# Add -D in front of every preprocessor define declaration
DEFS_XT = $(DEFINES_XT:%=-D%) -DBIOS_SIZE=$(BIOS_SIZE_SMALL)
DEFS_XTPLUS = $(DEFINES_XTPLUS:%=-D%) -DBIOS_SIZE=$(BIOS_SIZE_SMALL)
DEFS_AT = $(DEFINES_AT:%=-D%) -DBIOS_SIZE=$(BIOS_SIZE_SMALL)
DEFS_386 = $(DEFINES_386:%=-D%) -DBIOS_SIZE=$(BIOS_SIZE_SMALL)
DEFS_PS2 = $(DEFINES_PS2:%=-D%) -DBIOS_SIZE=$(BIOS_SIZE_SMALL)
DEFS_XT_LARGE = $(DEFINES_XT_LARGE:%=-D%) -DBIOS_SIZE=$(BIOS_SIZE_LARGE)
DEFS_XTPLUS_LARGE = $(DEFINES_XTPLUS_LARGE:%=-D%) -DBIOS_SIZE=$(BIOS_SIZE_LARGE)
DEFS_AT_LARGE = $(DEFINES_AT_LARGE:%=-D%) -DBIOS_SIZE=$(BIOS_SIZE_LARGE)
DEFS_386_LARGE = $(DEFINES_386_LARGE:%=-D%) -DBIOS_SIZE=$(BIOS_SIZE_LARGE)
DEFS_PS2_LARGE = $(DEFINES_PS2_LARGE:%=-D%) -DBIOS_SIZE=$(BIOS_SIZE_LARGE)
DEFS_XT_TINY = $(DEFINES_XT_TINY:%=-D%) -DBIOS_SIZE=$(BIOS_SIZE_TINY)
DEFS_CUSTOM = $(DEFINES_CUSTOM:%=-D%) -DBIOS_SIZE=$(BIOS_SIZE_CUSTOM)

# Add -I in front of all header directories
IHEADERS = $(HEADERS:%=-I%)

# Path + target file to be built
TARGET = $(BUILD_DIR)/$(PROG)


#########################
# Compilers and linkers #
#########################

# Make
MAKE = mingw32-make.exe

# Assembler
AS = nasm.exe

# use this command to erase files.
RM = -del /Q


#############################
# Compiler and linker flags #
#############################

# Assembly compiler flags
ASFLAGS = -f bin				# Produce binary object files
ASFLAGS += $(IHEADERS)			# Set header file directory paths
ASFLAGS += -Worphan-labels		# Warn about labels without colon
ASFLAGS += -Ox					# Optimize operands to their shortest forms


############################################
# Build process. Actual work is done here. #
############################################

all: clean small large
	@echo All done!

small: xt_tiny xt xtplus at 386 ps2
	@echo All small binaries built!

large: xt_large xtplus_large at_large 386_large ps2_large
	@echo All large binaries built!

ps2:
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_PS2) -l"$(TARGET)_ps2.lst" -o"$(TARGET)_ps2.bin"
	@echo * Small PS/2 version "$(TARGET)_ps2.bin" built.

ps2_large:
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_PS2_LARGE) -l"$(TARGET)_ps2l.lst" -o"$(TARGET)_ps2l.bin"
	@echo * Large PS/2 version "$(TARGET)_ps2l.bin" built.

386:
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_386) -l"$(TARGET)_386.lst" -o"$(TARGET)_386.bin"
	@echo * Small 386 version "$(TARGET)_386.bin" built.

386_large:
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_386_LARGE) -l"$(TARGET)_386l.lst" -o"$(TARGET)_386l.bin"
	@echo * Large 386 version "$(TARGET)_386l.bin" built.

at:
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_AT) -l"$(TARGET)_at.lst" -o"$(TARGET)_at.bin"
	@echo * Small AT version "$(TARGET)_at.bin" built.

at_large:
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_AT_LARGE) -l"$(TARGET)_atl.lst" -o"$(TARGET)_atl.bin"
	@echo * Large AT version "$(TARGET)_atl.bin" built.

xtplus:
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XTPLUS) -l"$(TARGET)_xtp.lst" -o"$(TARGET)_xtp.bin"
	@echo * Small XT Plus version "$(TARGET)_xtp.bin" built.

xtplus_large:
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XTPLUS_LARGE) -l"$(TARGET)_xtpl.lst" -o"$(TARGET)_xtpl.bin"
	@echo * Large XT Plus version "$(TARGET)_xtpl.bin" built.

xt:
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XT) -l"$(TARGET)_xt.lst" -o"$(TARGET)_xt.bin"
	@echo * Small XT version "$(TARGET)_xt.bin" built.

xt_large:
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XT_LARGE) -l"$(TARGET)_xtl.lst" -o"$(TARGET)_xtl.bin"
	@echo * Large XT version "$(TARGET)_xtl.bin" built.

xt_tiny:
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XT_TINY) -l"$(TARGET)_tiny.lst" -o"$(TARGET)_tiny.bin"
	@echo * Tiny XT version "$(TARGET)_tiny.bin" built.

custom:
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_CUSTOM) -l"$(TARGET)_cstm.lst" -o"$(TARGET)_cstm.bin"
	@echo * Custom version "$(TARGET)_cstm.bin" built.

strings: src\Strings.asm
	@$(AS) src\Strings.asm $(ASFLAGS) $(DEFS_AT_LARGE) -DCHECK_FOR_UNUSED_ENTRYPOINTS -DMODULE_STRINGS_COMPRESSED_PRECOMPRESS -o build\Strings.bin -l build\StringsPrecompress.lst
	@perl ..\Tools\StringsCompress.pl < build\StringsPrecompress.lst > src\StringsCompressed.asm
	@echo StringsCompressed.asm updated!

clean:
	@$(RM) $(BUILD_DIR)\*.*
	@echo Deleted "(*.*)" from "$(BUILD_DIR)/"

checksum: all
	@perl ..\Tools\checksum.pl $(TARGET)_tiny.bin $(BIOS_SIZE_TINY)
	@perl ..\Tools\checksum.pl $(TARGET)_xt.bin $(BIOS_SIZE_SMALL)
	@perl ..\Tools\checksum.pl $(TARGET)_xtp.bin $(BIOS_SIZE_SMALL)
	@perl ..\Tools\checksum.pl $(TARGET)_at.bin $(BIOS_SIZE_SMALL)
	@perl ..\Tools\checksum.pl $(TARGET)_386.bin $(BIOS_SIZE_SMALL)
	@perl ..\Tools\checksum.pl $(TARGET)_ps2.bin $(BIOS_SIZE_SMALL)
	@perl ..\Tools\checksum.pl $(TARGET)_xtl.bin $(BIOS_SIZE_LARGE)
	@perl ..\Tools\checksum.pl $(TARGET)_xtpl.bin $(BIOS_SIZE_LARGE)
	@perl ..\Tools\checksum.pl $(TARGET)_atl.bin $(BIOS_SIZE_LARGE)
	@perl ..\Tools\checksum.pl $(TARGET)_386l.bin $(BIOS_SIZE_LARGE)
	@perl ..\Tools\checksum.pl $(TARGET)_ps2l.bin $(BIOS_SIZE_LARGE)

unused:
	@echo "XT Tiny"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XT_TINY) -o"$(TARGET)_unused_tiny.asm" -l"$(TARGET)_unused_tiny.lst"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XT_TINY) -o"$(TARGET)_unused_tiny.asm" -E -DCHECK_FOR_UNUSED_ENTRYPOINTS
	@perl ..\Tools\unused.pl $(TARGET)_unused_tiny.lst $(TARGET)_unused_tiny.asm
	@echo "XT Small"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XT) -o"$(TARGET)_unused_xt.asm" -l"$(TARGET)_unused_xt.lst"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XT) -o"$(TARGET)_unused_xt.asm" -E -DCHECK_FOR_UNUSED_ENTRYPOINTS
	@perl ..\Tools\unused.pl $(TARGET)_unused_xt.lst $(TARGET)_unused_xt.asm
	@echo "XT Large"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XT_LARGE) -o"$(TARGET)_unused_xtl.asm" -l"$(TARGET)_unused_xtl.lst"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XT_LARGE) -o"$(TARGET)_unused_xtl.asm" -E -DCHECK_FOR_UNUSED_ENTRYPOINTS
	@perl ..\Tools\unused.pl $(TARGET)_unused_xtl.lst $(TARGET)_unused_xtl.asm
	@echo "XT Plus Small"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XTPLUS) -o"$(TARGET)_unused_xtp.asm" -l"$(TARGET)_unused_xtp.lst"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XTPLUS) -o"$(TARGET)_unused_xtp.asm" -E -DCHECK_FOR_UNUSED_ENTRYPOINTS
	@perl ..\Tools\unused.pl $(TARGET)_unused_xtp.lst $(TARGET)_unused_xtp.asm
	@echo "XT Plus Large"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XTPLUS_LARGE) -o"$(TARGET)_unused_xtpl.asm" -l"$(TARGET)_unused_xtpl.lst"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_XTPLUS_LARGE) -o"$(TARGET)_unused_xtpl.asm" -E -DCHECK_FOR_UNUSED_ENTRYPOINTS
	@perl ..\Tools\unused.pl $(TARGET)_unused_xtpl.lst $(TARGET)_unused_xtpl.asm
	@echo "AT Small"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_AT) -o"$(TARGET)_unused_at.asm" -l"$(TARGET)_unused_at.lst"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_AT) -o"$(TARGET)_unused_at.asm" -E -DCHECK_FOR_UNUSED_ENTRYPOINTS
	@perl ..\Tools\unused.pl $(TARGET)_unused_at.lst $(TARGET)_unused_at.asm
	@echo "AT Large"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_AT_LARGE) -o"$(TARGET)_unused_atl.asm" -l"$(TARGET)_unused_atl.lst"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_AT_LARGE) -o"$(TARGET)_unused_atl.asm" -E -DCHECK_FOR_UNUSED_ENTRYPOINTS
	@perl ..\Tools\unused.pl $(TARGET)_unused_atl.lst $(TARGET)_unused_atl.asm
	@echo "386 Small"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_386) -o"$(TARGET)_unused_386.asm" -l"$(TARGET)_unused_386.lst"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_386) -o"$(TARGET)_unused_386.asm" -E -DCHECK_FOR_UNUSED_ENTRYPOINTS
	@perl ..\Tools\unused.pl $(TARGET)_unused_386.lst $(TARGET)_unused_386.asm
	@echo "386 Large"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_386_LARGE) -o"$(TARGET)_unused_386l.asm" -l"$(TARGET)_unused_386l.lst"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_386_LARGE) -o"$(TARGET)_unused_386l.asm" -E -DCHECK_FOR_UNUSED_ENTRYPOINTS
	@perl ..\Tools\unused.pl $(TARGET)_unused_386l.lst $(TARGET)_unused_386l.asm
	@echo "PS/2 Small"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_PS2) -o"$(TARGET)_unused_ps2.asm" -l"$(TARGET)_unused_ps2.lst"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_PS2) -o"$(TARGET)_unused_ps2.asm" -E -DCHECK_FOR_UNUSED_ENTRYPOINTS
	@perl ..\Tools\unused.pl $(TARGET)_unused_ps2.lst $(TARGET)_unused_ps2.asm
	@echo "PS/2 Large"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_PS2_LARGE) -o"$(TARGET)_unused_ps2l.asm" -l"$(TARGET)_unused_ps2l.lst"
	@$(AS) "$(SRC_ASM)" $(ASFLAGS) $(DEFS_PS2_LARGE) -o"$(TARGET)_unused_ps2l.asm" -E -DCHECK_FOR_UNUSED_ENTRYPOINTS
	@perl ..\Tools\unused.pl $(TARGET)_unused_ps2l.lst $(TARGET)_unused_ps2l.asm


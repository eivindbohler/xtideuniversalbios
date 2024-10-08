; Project name	:	XTIDE Universal BIOS
; Description	:	Defines for ROMVARS struct containing variables stored
;					in BIOS ROM.

;
; XTIDE Universal BIOS and Associated Tools
; Copyright (C) 2009-2010 by Tomi Tilli, 2011-2023 by XTIDE Universal BIOS Team.
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
; Visit http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
;

%ifndef ROMVARS_INC
%define ROMVARS_INC


; Master/Slave drive specific parameters
struc DRVPARAMS
	.wFlags			resb	2	; Drive flags
	.dwMaximumLBA:				; User specified maximum number of sectors
	.wCylinders		resb	2	; User specified cylinders (1...16383)
	.wHeadsAndSectors:
	.bHeads			resb	1	; User specified Heads (1...16)
	.bSect			resb	1	; User specified Sectors per track (1...63)
endstruc

; Bit defines for DRVPARAMS.wFlags - these flags are accessed as bytes so changes here might require changes elsewhere
MASK_DRVPARAMS_WRITECACHE		EQU	(3<<0)	; Bits 0...1, Drive internal write cache settings (must start at bit 0)
	DEFAULT_WRITE_CACHE				EQU	0	; Must be 0
	DISABLE_WRITE_CACHE				EQU	1
	ENABLE_WRITE_CACHE				EQU	2
MASK_DRVPARAMS_TRANSLATEMODE	EQU	(3<<TRANSLATEMODE_FIELD_POSITION)	; Bits 2...3, Position shared with DPT
	TRANSLATEMODE_FIELD_POSITION	EQU	2
	TRANSLATEMODE_NORMAL			EQU	0	; Must be zero
	TRANSLATEMODE_LARGE				EQU	1
	TRANSLATEMODE_ASSISTED_LBA		EQU	2	; 28-bit or 48-bit LBA
	TRANSLATEMODE_AUTO				EQU	3	; Only available in ROMVARS, not in DPTs
FLG_DRVPARAMS_BLOCKMODE			EQU	(1<<4)	; Enable Block mode transfers
FLG_DRVPARAMS_USERCHS			EQU	(1<<5)	; User specified P-CHS values
FLG_DRVPARAMS_USERLBA			EQU	(1<<6)	; User specified LBA value
FLG_DRVPARAMS_DO_NOT_DETECT		EQU	(1<<7)	; Disable detection of drive


; Controller specific variables
struc IDEVARS
;;; Word 0
	.wSerialPortAndBaud:					; Serial connection port (low, divided by 4) and baud rate divisor (high)
	.wBasePort:								; IDE Base Port for Command Block (usual) Registers
	.bSerialPort				resb	1
	.bSerialBaud				resb	1

;;; Word 1
	.wControlBlockPort:
	.bSerialUnused				resb	1	; IDE Base Port for Control Block Registers

	.wSerialCOMPortCharAndDevice:			; In DetectPrint, we grab the COM Port char and Device at the same time
	.bSerialCOMPortChar			resb	1	; Serial connection COM port number/letter

;;; Word 2
	.bDevice					resb	1	; Device type
	.bIRQ						resb	1	; Interrupt Request Number

;;; And more...
	.drvParamsMaster			resb	DRVPARAMS_size
	.drvParamsSlave				resb	DRVPARAMS_size
endstruc

%ifndef CHECK_FOR_UNUSED_ENTRYPOINTS
	%if IDEVARS.bSerialCOMPortChar+1 != IDEVARS.bDevice
		%error "IDEVARS.bSerialCOMPortChar needs to come immediately before IDEVARS.bDevice so that both bytes can be fetched at the same time inside DetectPrint.asm"
	%endif
%endif

STANDARD_CONTROL_BLOCK_OFFSET			EQU		200h
XTIDE_CONTROL_BLOCK_OFFSET				EQU		8h		; For XTIDE, A3 is used to control selected register (CS0 vs CS1)...
XTCF_CONTROL_BLOCK_OFFSET				EQU		10h		; ...and for XT-CF (all variants), it's A4
ADP50L_CONTROL_BLOCK_OFFSET				EQU		10h

; Default values for Port and PortCtrl, shared with the configurator
;
DEVICE_XTIDE_DEFAULT_PORT				EQU		300h	; Also the default port for XT-CF
DEVICE_XTIDE_DEFAULT_PORTCTRL			EQU		(DEVICE_XTIDE_DEFAULT_PORT + XTIDE_CONTROL_BLOCK_OFFSET)
; Note XT-CF control port is SHL 1 relative to XTIDE, and coded that way hence no need for specific definition like...
; DEVICE_XTCF_DEFAULT_PORTCTRL			EQU		(DEVICE_XTIDE_DEFAULT_PORT + XTCF_CONTROL_BLOCK_OFFSET)

DEVICE_ATA_PRIMARY_PORT					EQU		1F0h
DEVICE_ATA_PRIMARY_PORTCTRL				EQU		(DEVICE_ATA_PRIMARY_PORT + STANDARD_CONTROL_BLOCK_OFFSET)

DEVICE_ATA_SECONDARY_PORT				EQU		170h
DEVICE_ATA_SECONDARY_PORTCTRL			EQU		(DEVICE_ATA_SECONDARY_PORT + STANDARD_CONTROL_BLOCK_OFFSET)

DEVICE_ATA_TERTIARY_PORT				EQU		1E8h
DEVICE_ATA_TERTIARY_PORTCTRL			EQU		(DEVICE_ATA_TERTIARY_PORT + STANDARD_CONTROL_BLOCK_OFFSET)

DEVICE_ATA_QUATERNARY_PORT				EQU		168h
DEVICE_ATA_QUATERNARY_PORTCTRL			EQU		(DEVICE_ATA_QUATERNARY_PORT + STANDARD_CONTROL_BLOCK_OFFSET)


; Device types for IDEVARS.bDevice
; IDE Devices are grouped so device numbers cannot be changed without modifying code elsewhere!
COUNT_OF_STANDARD_IDE_DEVICES			EQU	2	; 16- and 32-bit controllers
COUNT_OF_8BIT_IDE_DEVICES				EQU	10
COUNT_OF_ALL_IDE_DEVICES				EQU	(COUNT_OF_8BIT_IDE_DEVICES + COUNT_OF_STANDARD_IDE_DEVICES)
; Standard port mapped I/O
DEVICE_16BIT_ATA						EQU	(0<<1)
DEVICE_32BIT_ATA						EQU	(1<<1)
DEVICE_8BIT_ATA							EQU	((COUNT_OF_STANDARD_IDE_DEVICES+0)<<1)	; 16- or 32-bit controller in 8-bit mode
DEVICE_8BIT_XTIDE_REV1					EQU	((COUNT_OF_STANDARD_IDE_DEVICES+1)<<1)
; Address lines A0 and A3 are swapped
DEVICE_8BIT_XTIDE_REV2					EQU	((COUNT_OF_STANDARD_IDE_DEVICES+2)<<1)	; Or rev 1 with swapped A0 and A3...
DEVICE_8BIT_XTIDE_REV2_OLIVETTI			EQU	((COUNT_OF_STANDARD_IDE_DEVICES+3)<<1)	; ...in Olivetti M24 and derivatives
; IDE Register offsets are SHL 1
DEVICE_8BIT_XTCF_PIO8					EQU	((COUNT_OF_STANDARD_IDE_DEVICES+4)<<1)	; XT-CF using 8-bit PIO mode
DEVICE_8BIT_XTCF_PIO8_WITH_BIU_OFFLOAD	EQU	((COUNT_OF_STANDARD_IDE_DEVICES+5)<<1)	; XT-CF using 8-bit PIO mode, but with 16-bit instructions
DEVICE_8BIT_XTCF_PIO16_WITH_BIU_OFFLOAD	EQU	((COUNT_OF_STANDARD_IDE_DEVICES+6)<<1)	; Lo-tech 8-bit IDE Adapter
DEVICE_8BIT_XTCF_DMA					EQU	((COUNT_OF_STANDARD_IDE_DEVICES+7)<<1)	; XT-CFv3 using DMA
; Memory Mapped I/O
DEVICE_8BIT_JRIDE_ISA					EQU	((COUNT_OF_STANDARD_IDE_DEVICES+8)<<1)	; JR-IDE/ISA (Memory Mapped I/O)
DEVICE_8BIT_ADP50L						EQU	((COUNT_OF_STANDARD_IDE_DEVICES+9)<<1)	; SVC ADP50L (Memory Mapped I/O)
; Virtual devices
DEVICE_SERIAL_PORT						EQU	(COUNT_OF_ALL_IDE_DEVICES<<1)

FIRST_XTCF_DEVICE						EQU	DEVICE_8BIT_XTCF_PIO8
LAST_XTCF_DEVICE						EQU	DEVICE_8BIT_XTCF_DMA
XTCF_DEVICE_OFFSET						EQU	FIRST_XTCF_DEVICE						; Used for XT-CF device <--> mode conversion

; Segment when RAMVARS is stored to top of interrupt vectors.
LITE_MODE_RAMVARS_SEGMENT				EQU	30h

; ROM Variables. Written to the ROM image before flashing.
; The ROMVARS version in Version.inc must be incremented whenever this struc changes.
struc ROMVARS
	.wRomSign			resb	2	; ROM Signature (AA55h)
	.bRomSize			resb	1	; ROM size in 512 byte blocks
	.rgbJump			resb	3	; First instruction to ROM init (jmp)

	.rgbSign			resb	6	; Signature for XTIDE Configurator Program (must be even length)
	.szTitle			resb	32	; BIOS title string
	.szVersion			resb	18	; BIOS version string (supports up to r999)

	.pColorTheme		resb	2	; Ptr to the color attribute struc used by the boot menu and hotkey bar
	.wFlags				resb	2	; Word for ROM flags
	; Note! Any additional ROMVARS must be added below if they are supposed to be copied
	; over when doing "Load old settings from EEPROM" to a new BIOS loaded from file.
	.wRamVars			resb	2	; Segment address for RamVars
	.wDisplayMode		resb	2	; Display mode for boot menu
	.wBootTimeout		resb	2	; Boot Menu selection timeout in system timer ticks
	.bIdeCnt			resb	1	; Number of available IDE controllers
	.bBootDrv			resb	1	; Default drive to boot from
	.bMinFddCnt			resb	1	; Minimum number of Floppy Drives
	.bStealSize			resb	1	; Number of 1kB blocks stolen from 640kB base RAM
	.bIdleTimeout		resb	1	; Standby timer value

						alignb	2	; WORD align the IDEVARS structures
	.ideVarsBegin:
	.ideVars0			resb	IDEVARS_size
	.ideVars1			resb	IDEVARS_size
	.ideVars2			resb	IDEVARS_size
	.ideVars3			resb	IDEVARS_size

%ifdef MODULE_SERIAL
	.ideVarsSerialAuto	resb	IDEVARS_size
%endif

	.ideVarsEnd:
endstruc

%ifndef CHECK_FOR_UNUSED_ENTRYPOINTS
	%if ROMVARS.ideVarsEnd & 0xff00 <> 0
		%error ".ideVars structures must fit within the first 256 bytes of the ROM image"
	%endif
	%if (ROMVARS.ideVarsEnd - ROMVARS.ideVarsBegin) = 0
		%error "there must be at least one .ideVars structure, it would be bizarre if this were not true, but it is assumed in the ah0h reset code."
	%endif
%endif

NUMBER_OF_IDEVARS					EQU	((ROMVARS.ideVarsEnd - ROMVARS.ideVarsBegin) / IDEVARS_size)

; Bit defines for ROMVARS.wFlags
FLG_ROMVARS_FULLMODE					EQU	(1<<0)	; Full operating mode (steals base RAM, supports EBIOS etc.)
FLG_ROMVARS_CLEAR_BDA_HD_COUNT			EQU	(1<<1)	; This flag has two purposes; * Removes the system BIOS "dummy" drive
													; so that Windows 9x protected mode drivers can be used with XUB drives.
													; * Some computers (Zenith Z-171 and Z-161) do not clear the BDA hard
													; drive count which causes it to increment on each warm boot.
FLG_ROMVARS_SERIAL_SCANDETECT 			EQU	(1<<3)	; Scan COM ports at the end of drive detection.  Can also be invoked
													; by holding down the ALT key at the end of drive detection.
													; (Conveniently, this is 8, a fact we exploit when testing the bit)

; Here in case the configuration needs to know functionality is present. Note! Changing the order/location of these flags
; also requires changes elsewhere as they are usually tested using byte-accesses for efficiency.
FLG_ROMVARS_MODULE_POWER_MANAGEMENT		EQU	(1<<5)
FLG_ROMVARS_MODULE_8BIT_IDE				EQU	(1<<6)
FLG_ROMVARS_MODULE_8BIT_IDE_ADVANCED	EQU (1<<7)
FLG_ROMVARS_MODULE_ADVANCED_ATA			EQU	(1<<8)
FLG_ROMVARS_MODULE_BOOT_MENU			EQU	(1<<9)
FLG_ROMVARS_MODULE_EBIOS				EQU	(1<<10)
FLG_ROMVARS_MODULE_HOTKEYS				EQU	(1<<11)
FLG_ROMVARS_MODULE_IRQ					EQU	(1<<12)
FLG_ROMVARS_MODULE_SERIAL				EQU (1<<13)
FLG_ROMVARS_MODULE_SERIAL_FLOPPY		EQU (1<<14)
FLG_ROMVARS_MODULE_STRINGS_COMPRESSED	EQU (1<<15)


; Boot Menu Display Modes (see Assembly Library Display.inc for standard modes)
DEFAULT_TEXT_MODE		EQU	4


%endif ; ROMVARS_INC
